CC=g++
LIBS= `pkg-config --libs wayland-client++ wayland-client-extra++ wayland-cursor++ cairomm-1.0` -lfmt -lpng
CFLAGS=`pkg-config --cflags wayland-client++ wayland-client-extra++ wayland-cursor++ cairomm-1.0`
CPPFILES=main.cpp Client.cpp OnInit.cpp OnEvent.cpp Board.cpp BoardView.cpp SharedMemory.cpp
OBJECTS=main.o Client.o OnInit.o OnEvent.o Board.o BoardView.o SharedMemory.o
BINARY=main

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ $(LIBS)

%.o:%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS)
