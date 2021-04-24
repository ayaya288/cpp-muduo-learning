CC = g++

all: server

server: TcpServer.o
	$(CC) TcpServer.o main.cpp -o server

TcpServer.o: TcpServer.cpp
	$(CC) -c TcpServer.cpp

clean:
	rm *.o server