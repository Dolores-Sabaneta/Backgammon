output: main.o OnExecute.o OnInit.o OnEvent.o OnLoop.o OnRender.o OnCleanup.o
	g++ main.o OnExecute.o OnInit.o OnEvent.o OnLoop.o OnRender.o OnCleanup.o -lfmt -lSDL2 -o main
	
main.o: main.cpp Backgammon.h
	g++ -c main.cpp
	
OnExecute.o: OnExecute.cpp
	g++ -c OnExecute.cpp
	
OnInit.o: OnInit.cpp
	g++ -c OnInit.cpp
	
OnEvent.o: OnEvent.cpp
	g++ -c OnEvent.cpp
	
OnLoop.o: OnLoop.cpp
	g++ -c OnLoop.cpp
	
OnRender.o: OnRender.cpp
	g++ -c OnRender.cpp
	
OnCleanup.o: OnCleanup.cpp
	g++ -c OnCleanup.cpp
	
open:
	open main.cpp
	open Backgammon.h
	open OnExecute.cpp
	open OnInit.cpp
	open OnEvent.cpp
	open OnLoop.cpp
	open OnRender.cpp
	open OnCleanup.cpp
	
clean:	
	rm *.o

