.o:.c
	gcc -c $@ $^



client: client.o read_message.o tcp_client.o
	gcc -o $@ $^


server: server.o read_message.o signal_hander.o
	gcc -o $@ $^
	
clean:
	rm -rf *.o

chmod:
	chmod -x *.c *.h
