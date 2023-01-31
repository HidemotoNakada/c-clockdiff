ALL: server client

server: server.c
	gcc -O3 -o $@ $<

client: client.c
	gcc -O3 -o $@ $<
