all: example.c
	g++ -c ws.c example.c
	g++ ws.o example.o -o client

run: all
	./client

clean:
	rm example.o ws.o client