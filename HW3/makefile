compile: server client

server: gameInfo.h gameServer.c
	gcc -o server gameServer.c -lpthread

client: gameInfo.h gameClient.c
	gcc -o client gameClient.c

clean:
	rm -f server && rm -f client