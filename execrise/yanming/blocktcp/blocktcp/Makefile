.SUFFIXES: .cpp .c
.cpp.o:
	g++ -c $<

.c.o:
	gcc -c $<

tcpserver:readn.o tcpserver.o
	gcc -o $@ $?

tcpclient:tcpclient.o
	gcc -o $@ $?
	
	
clean:
	rm -f *.o