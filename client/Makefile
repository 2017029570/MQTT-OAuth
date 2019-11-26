include ../config.mk

.PHONY: all install uninstall reallyclean clean static static_pub static_sub static_rr

ifeq ($(WITH_SHARED_LIBRARIES),yes)
SHARED_DEP:=../lib/libmosquitto.so.${SOVERSION}
endif

ifeq ($(WITH_SHARED_LIBRARIES),yes)
ALL_DEPS:= mosquitto_pub mosquitto_sub mosquitto_rr
else
ifeq ($(WITH_STATIC_LIBRARIES),yes)
ALL_DEPS:= static_pub static_sub static_rr
endif
endif

all : ${ALL_DEPS}

static : static_pub static_sub static_rr
	# This makes mosquitto_pub/sub/rr versions that are statically linked with
	# libmosquitto only.

static_pub : pub_client.o pub_shared.o client_props.o client_shared.o ../lib/libmosquitto.a
	${CROSS_COMPILE}${CC} $^ -o mosquitto_pub ${CLIENT_LDFLAGS} ${STATIC_LIB_DEPS} -lcurl

static_sub : sub_client.o sub_client_output.o client_props.o client_shared.o ../lib/libmosquitto.a
	${CROSS_COMPILE}${CC} $^ -o mosquitto_sub ${CLIENT_LDFLAGS} ${STATIC_LIB_DEPS} -lcurl

static_rr : rr_client.o client_props.o client_shared.o pub_shared.o sub_client_output.o ../lib/libmosquitto.a
	${CROSS_COMPILE}${CC} $^ -o mosquitto_rr ${CLIENT_LDFLAGS} ${STATIC_LIB_DEPS} -lcurl

mosquitto_pub : pub_client.o pub_shared.o client_shared.o client_props.o
	${CROSS_COMPILE}${CC} $(CLIENT_LDFLAGS) $^ -o $@ $(CLIENT_LDADD) -lcurl

mosquitto_sub : sub_client.o sub_client_output.o client_shared.o client_props.o
	${CROSS_COMPILE}${CC} $(CLIENT_LDFLAGS) $^ -o $@ $(CLIENT_LDADD) -lcurl

mosquitto_rr : rr_client.o client_shared.o client_props.o pub_shared.o sub_client_output.o
	${CROSS_COMPILE}${CC} $(CLIENT_LDFLAGS) $^ -o $@ $(CLIENT_LDADD) -lcurl

pub_client.o : pub_client.c ${SHARED_DEP}
	${CROSS_COMPILE}${CC} $(CLIENT_CPPFLAGS) $(CLIENT_CFLAGS) -c $< -o $@ -lcurl

pub_shared.o : pub_shared.c ${SHARED_DEP}
	${CROSS_COMPILE}${CC} $(CLIENT_CPPFLAGS) $(CLIENT_CFLAGS) -c $< -o $@ -lcurl

sub_client.o : sub_client.c ${SHARED_DEP}
	${CROSS_COMPILE}${CC} $(CLIENT_CPPFLAGS) $(CLIENT_CFLAGS) -c $< -o $@ -lcurl

sub_client_output.o : sub_client_output.c ${SHARED_DEP}
	${CROSS_COMPILE}${CC} $(CLIENT_CPPFLAGS) $(CLIENT_CFLAGS) -c $< -o $@ -lcurl

rr_client.o : rr_client.c ${SHARED_DEP}
	${CROSS_COMPILE}${CC} $(CLIENT_CPPFLAGS) $(CLIENT_CFLAGS) -c $< -o $@ -lcurl

client_shared.o : client_shared.c client_shared.h
	${CROSS_COMPILE}${CC} $(CLIENT_CPPFLAGS) $(CLIENT_CFLAGS) -c $< -o $@ -lcurl

client_props.o : client_props.c client_shared.h
	${CROSS_COMPILE}${CC} $(CLIENT_CPPFLAGS) $(CLIENT_CFLAGS) -c $< -o $@ -lcurl

../lib/libmosquitto.so.${SOVERSION} :
	$(MAKE) -C ../lib -lcurl

../lib/libmosquitto.a :
	$(MAKE) -C ../lib libmosquitto.a -lcurl

install : all
	$(INSTALL) -d "${DESTDIR}$(prefix)/bin" 
	$(INSTALL) ${STRIP_OPTS} mosquitto_pub "${DESTDIR}${prefix}/bin/mosquitto_pub"
	$(INSTALL) ${STRIP_OPTS} mosquitto_sub "${DESTDIR}${prefix}/bin/mosquitto_sub"
	$(INSTALL) ${STRIP_OPTS} mosquitto_rr "${DESTDIR}${prefix}/bin/mosquitto_rr" 

uninstall :
	-rm -f "${DESTDIR}${prefix}/bin/mosquitto_pub"
	-rm -f "${DESTDIR}${prefix}/bin/mosquitto_sub"
	-rm -f "${DESTDIR}${prefix}/bin/mosquitto_rr" 

reallyclean : clean

clean : 
	-rm -f *.o mosquitto_pub mosquitto_sub mosquitto_rr *.gcda *.gcno
