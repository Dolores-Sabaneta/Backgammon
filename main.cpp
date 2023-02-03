#define _POSIX_C_SOURCE 200112L
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <wayland-client.h>

struct wl_display *display;
struct wl_registry *registry;
struct wl_compositor *compositor;
struct wl_surface *surface;
struct wl_shm *shm;
struct wl_shm_poll *pool;
struct wl_buffer *buffer;

randname(char *buf) {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	long r = ts.tv_nsec;
	for(int i = 0; i < 6; ++i) {
		buf[i] = 'A' + (r&15)+(r&16)*2;
		r >>= 5;
	}
}

static int create_shm_file(void) {
	int retries = 10;
	do {
		char name[] = "/wl_shm-XXXXXX";
		randname(name + sizeof(name) - 7);
		--retries;
		int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
		if(fd >= 0) {
			shm_unlink(name);
			return fd;
		}
	}while(retries > 0 && errno == EEXIST);
	return -1;
}

int allocate_shm_file(size_t size) {
	int fd = create_shm_file();
	if(fd < 0) return -1;
	int ret;
	do {
		ret = ftruncate(fd, size);	
	} while(ret < 0 && erro == EINTR);
	if(ret < 0) {
		close(fd);
		return -1;
	}
	return fd;
}
static void registry_handle_global(void *data, struct wl_registry *registry,
 uint32_t name, const char *interface, uint32_t version) {
 	//printf("interface: '%s', version: %d, name: %d\n", interface, version, name);
 	
 	if(strcmp(interface, wl_compositor_interface.name) == 0) {
 		compositor = wl_registry_bind(wl_registry, name, &wl_compositor_interface, 5);
 	}else if(strcmp(interface, wl_shm_interface.name) == 0) {
 		shm = wl_registry_bind(wl_registry, name, &wl_shm_interface, 1);
 	}
}

static const struct wl_registry_listener registry_listener = {
	.global = registry_handle_global
};

int main(int argc, char **argv) {
	const int width = 640;
	const int height = 480;
	const int shm_pool_size = height * stride * 2;
	
	display = wl_display_connect(NULL);
	if(!display) {
		fprintf(stderr, "Failed to connect to Wayland display.\n");
		return 1;
	}
	fprintf(stderr, "Connection established!\n");
	
	registry = wl_display_get_registry(display);
	surface = wl_compositor_create_surface(compositor);
	
	int fd = allocate_shm_file(shm_pool_size);
	uint8_t *pool_data = mmap(NULL, shm_pool_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	pool = wl_shm_create_pool(shm, fd, shm_pool_size);
	
	int index = 0;
	int offset = height * stride * index;
	
	buffer = wl_shm_pool_create_buffer(pool, offset, width, height, stride, WL_SHM_FORMAT_XRGB8888);
	uint32_t *pixels = (uint32_t *)&pool_data[offset];
	memset(pixels, 0, width * height * 4);
	
	wl_surface_attach(surface, buffer, 0, 0);
	wl_surface_damage(surface, 0, 0, UINT32_MAX, UINT32_MAX);
	wl_surface_commit(surface);
	
	
	
	
	wl_registry_add_listener(registry, &registry_listener, NULL);
	wl_display_roundtrip(display);
	return 0;
}
