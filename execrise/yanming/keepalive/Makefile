.SUFFIXES: .cpp .c
.cpp.o:
	g++ -c $<

.c.o:
	gcc -c $<

tcpclient:tcpclient.o
	gcc -o $@ $?

tcpserver:tcpserver.o
	gcc -o $@ $?
	
	
clean:
	rm -f *.o