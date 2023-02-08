output: main.o Client.o Board.o OnInit.o OnEvent.o
	g++ main.o Client.o Board.o OnInit.o OnEvent.o `pkg-config --libs wayland-client++ wayland-client-extra++ wayland-cursor++ cairomm-1.0` -lfmt -lpng -o main
	
main.o: main.cpp
	g++ -c main.cpp `pkg-config --cflags wayland-client++ wayland-client-extra++ wayland-cursor++ cairomm-1.0`

Client.o: Client.cpp Client.hpp
	g++ -c Client.cpp `pkg-config --cflags wayland-client++ wayland-client-extra++ wayland-cursor++ cairomm-1.0`
	
OnInit.o: OnInit.cpp
	g++ -c OnInit.cpp `pkg-config --cflags wayland-client++ wayland-client-extra++ wayland-cursor++ cairomm-1.0`
	
OnEvent.o: OnEvent.cpp
	g++ -c OnEvent.cpp `pkg-config --cflags wayland-client++ wayland-client-extra++ wayland-cursor++ cairomm-1.0`
	
Board.o: Board.cpp Board.hpp
	g++ -c Board.cpp `pkg-config --cflags wayland-client++ wayland-client-extra++ wayland-cursor++ cairomm-1.0`

	
clean: 
	rm *.o


