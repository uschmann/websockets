all: client.c
	g++ -c client.c
	g++ client.o -o client

run: all
	./client