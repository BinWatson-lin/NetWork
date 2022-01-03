.SUFFIXES: .cpp .c
.cpp.o:
	g++ -c $<

.c.o:
	gcc -c $<

CC=gcc
# CFLAGS=-I


# include文件的路径
IDIR=../include
# 存放生成的.o文件
ODIR=obj

# 当前路径下的头文件
_DEPS=un.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
unixstreamserver = unixstreamserver.o
unixstreamclient = unixstreamclient.o
unixdataserver = unixdataserver.o
unixdataclient = unixdataclient.o

OBJ = $(patsubst %,$(unixstreamserver),$(unixstreamclient))

# 参数 $@ 和 $< 分别指代规则第一行中的冒号的左右两边的内容
# 将点c全部编译成点o文件
$(ODIR)*.o: *.c $(DEPS)
	$(CC) -c -o $@ $< 

unixstreamserver:$(unixstreamserver)
	$(CC) -o $@ $? 

unixstreamclient:$(unixstreamclient)
	$(CC) -o $@ $? 

unixdataserver:$(unixdataserver)
	$(CC) -o $@ $? 

unixdataclient:$(unixdataclient)
	$(CC) -o $@ $? 
	
.PHONY: clean
	
clean:
	rm -f *.o