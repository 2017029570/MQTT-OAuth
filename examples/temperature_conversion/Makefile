CFLAGS=-Wall -ggdb -I../../lib -I../../lib/cpp
LDFLAGS=-L../../lib ../../lib/cpp/libmosquittopp.so.1 ../../lib/libmosquitto.so.1

.PHONY: all clean

all : mqtt_temperature_conversion

mqtt_temperature_conversion : main.o temperature_conversion.o
	${CXX} $^ -o $@ ${LDFLAGS} -lcurl

main.o : main.cpp
	${CXX} -c $^ -o $@ ${CFLAGS} -lcurl

temperature_conversion.o : temperature_conversion.cpp
	${CXX} -c $^ -o $@ ${CFLAGS} -lcurl

clean : 
	-rm -f *.o mqtt_temperature_conversion
