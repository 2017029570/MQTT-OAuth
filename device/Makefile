CFLAGS=-Wall -ggdb
LDFLAGS=../lib/libmosquitto.so.1

.PHONY: all clean

all : device

device : device.o 
	${CC} $^ -o $@ ${LDFLAGS} -lcurl

device.o : device.c 
	${CC} -c $^ -o $@ ${CFLAGS} -lcurl

clean:
	-rm -f *.o device
