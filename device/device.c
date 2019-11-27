#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifndef WIN32
	#include <unistd.h>
#else
	#include <process.h>
	#define snprintf sprintf_s
#endif

#include "/home/nam98/MQTT-OAuth/lib/mosquitto.h"

#define mqtt_host "localhost"
#define mqtt_port 1883
#define MQTT_TOPIC "topic"

static int run = 1;

void handle_signal(int s)
{
	run = 0;
}

void connect_callback(struct mosquitto *mosq, void *obj, int result)
{
	printf("connect callback, rc=%d\n", result);
}

void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) 
{
	bool match = 0;

	printf("receive message(%s) : %s", message->topic, message->payload);
	if (match) {
		printf("got message for ADC topic\n");
	}
}

int main(int argc, char *argv[]) {
	uint8_t reconnect = true;
	struct mosquitto *mosq;
	int rc = 0;

	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);

	mosquitto_lib_init();

	mosq = mosquitto_new("oauth", true, 0);
	
	if(mosq) {
//		mosquitto_connect_callback_set(mosq, connect_callback);
		mosquitto_message_callback_set(mosq, message_callback);
		rc = mosquitto_connect(mosq, mqtt_host, mqtt_port, 0);
		printf("%d\n", rc);	
		while(run) {
			rc = mosquitto_loop(mosq, -1, 1);
			if(run && rc) {
				printf("connection error!\n");
				sleep(10);
				mosquitto_reconnect(mosq);
			}
		}

		mosquitto_destroy(mosq);
	}	

	mosquitto_lib_cleanup();

	return rc;
}
