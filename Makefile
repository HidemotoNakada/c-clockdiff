ALL: server client

server: server.c
	gcc -Wno-unused-result -O3 -o $@ $<

client: client.c
	gcc -Wno-unused-result -O3 -o $@ $<
