.SUFFIXES: .cpp .c
.cpp.o:
	g++ -c $<

.c.o:
	gcc -c $<

udpserver:udpserver.o
	gcc -o $@ $?

udpclient:udpclient.o
	gcc -o $@ $?
	
	
clean:
	rm -f *.o