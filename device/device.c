#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

#ifndef WIN32
	#include <unistd.h>
#else
	#include <process.h>
	#define snprintf sprintf_s
#endif

#include <mosquitto.h>

#define mqtt_host "localhost"
#define mqtt_port 1883
#define MQTT_TOPIC "topic"
#define DEVICE_ID "uid"
#define DEVICE_PASSWD "passwd"

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

//struct mosquitto_message* device_read(struct mosquitto *mosq) {
//	struct mosquitto_message *buffer;
//	buffer = (struct mosquitto_message *)malloc(sizeof(struct mosquitto_message));
	
//	read(mosq->sock, buffer, sizeof(buffer));
//	return buffer;
//}

int main(int argc, char *argv[]) {
	uint8_t reconnect = true;
	struct mosquitto *mosq = NULL;
	//mosq = (struct mosquitto *)malloc(sizeof(struct mosquitto));
//	struct mosquitto_message *buffer;
	char* rbuffer = (char*)malloc(sizeof(200));
	char* wbuffer = (char*)malloc(sizeof(100));
//	buffer = mosquitto__calloc(1, sizeof(struct mosquitto_message));
	int rc = 0;

	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);

	mosquitto_lib_init();
	mosq = mosquitto_new("oauth", true, 0);
	if(mosq) {
//		mosquitto_connect_callback_set(mosq, connect_callback);
		mosquitto_message_callback_set(mosq, message_callback);

		rc = mosquitto_connect(mosq, mqtt_host, mqtt_port, 0);
	//	strcpy(wbuffer, "fuck");
	//	oauth_write(mosq, wbuffer, strlen(wbuffer)+1);
		

		rbuffer = oauth_read(mosq);

		char* auth_code = (char*)malloc(22);
		strcpy(auth_code, server_login(rbuffer, DEVICE_ID, DEVICE_PASSWD));

		printf("%s\n", auth_code);	
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
