CC = g++
OBJ = $(wildcard *.cpp)

all: server

server: $(OBJ)
	$(CC) $(OBJ) -o server

clean:
	rm server