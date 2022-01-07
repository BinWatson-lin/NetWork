.o:.c
	gcc -c $@ $^

client: tcp_client.o client.o
	gcc -o $@ $^

client2: tcp_client.o client2.o
	gcc -o $@ $^

server: tcp_server.o server.o
	gcc -o $@ $^

server2: tcp_server.o server2.o
	gcc -o $@ $^

clean:
	rm -rf *.o
