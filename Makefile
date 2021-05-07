CC = g++
OBJ = $(wildcard *.cpp)

all: server

server: $(OBJ)
	$(CC) $(OBJ) -pthread -o server

clean:
	rm server