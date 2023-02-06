output: main.o Client.o Board.o OnInit.o OnEvent.o
	g++ main.o Client.o Board.o OnInit.o OnEvent.o `pkg-config --libs wayland-client++ wayland-client-extra++` -lfmt -lpng -o main
	
main.o: main.cpp
	g++ -c main.cpp `pkg-config --cflags wayland-client++ wayland-client-extra++`

Client.o: Client.cpp 
	g++ -c Client.cpp `pkg-config --cflags wayland-client++ wayland-client-extra++`
	
OnInit.o: OnInit.cpp
	g++ -c OnInit.cpp `pkg-config --cflags wayland-client++ wayland-client-extra++`
	
OnEvent.o: OnEvent.cpp
	g++ -c OnEvent.cpp `pkg-config --cflags wayland-client++ wayland-client-extra++`
	
Board.o: Board.cpp 
	g++ -c Board.cpp `pkg-config --cflags wayland-client++ wayland-client-extra++`

	
clean: 
	rm *.o


