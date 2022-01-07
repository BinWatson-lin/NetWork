.o:.c
	gcc -o $@ $^


client: client.o
	gcc -o $@ $^


server: readn.o read_message.o server.c
	gcc -o $@ $^

clean:
	rm -rf *.o
