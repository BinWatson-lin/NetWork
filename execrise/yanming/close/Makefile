.SUFFIXES: .cpp .c
.cpp.o:
	g++ -c $<

.c.o:
	gcc -c $<

graceclient:graceclient.o
	gcc -o $@ $?

graceserver:graceserver.o
	gcc -o $@ $?
	
	
clean:
	rm -f *.o