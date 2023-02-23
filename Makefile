all:
	g++ ./src/*.cpp -I./include -o main -lSDL2
	./main