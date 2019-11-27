#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include <stdio.h>

#define MQTT_HOSTNAME "localhost"
#define MQTT_PORT 1883
#define MQTT_TOPIC "topic"

int main(int argc, char **argv) {
	struct mosquitto *mosq = NULL;
	mosquitto_lib_init();
	mosq = mosquitto_new(NULL, true, NULL);

	if(!mosq) {
		printf("Cant initiallize mosquitto library\n");
		exit(-1);
	}
	mosquitto_username_pw_set(mosq, "admin", "admin");
	int ret = mosquitto_connect(mosq, MQTT_HOSTNAME, MQTT_PORT, 0);
	if(ret) {
		printf("Cant connect to mosquitto server\n");
		exit(-1);
	}

	char text[20] = "Nice to meet u!\n";

	ret = mosquitto_publish(mosq, NULL, MQTT_TOPIC, strlen(text), text, 0, false);
	if(ret) {
		printf("Cant connect to mosquitto server\n");
		exit(-1);
	}

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;
}
