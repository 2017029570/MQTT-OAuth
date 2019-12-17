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
	if(result == 0)
		printf("connect callback, Success\n");
	else printf("connect callback, Fail\n");
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
	char* rbuffer = (char*)malloc(sizeof(200));
	char* wbuffer = (char*)malloc(sizeof(100));
	int rc = 0;

	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);
	
	mosquitto_lib_init();
	mosq = mosquitto_new(NULL, true, 0, 1);
	
	if(mosq) {
		mosquitto_connect_callback_set(mosq, connect_callback);
		mosquitto_message_callback_set(mosq, message_callback);

		rc = mosquitto_connect(mosq, mqtt_host, mqtt_port, 0);
		
		printf("Trying to connect...\n");
		sleep(3);

		rbuffer = oauth_read(mosq);

		char* auth_code = (char*)malloc(22);

		strcpy(auth_code, server_login(rbuffer, DEVICE_ID, DEVICE_PASSWD));
		sleep(3);
		printf("Sign in the Authorization Server...\n");
		sleep(3);
		printf("auth_code : %s\n", auth_code);
		rc = oauth_write(mosq, auth_code, strlen(auth_code)+1);

//		while(run) {
//			rc = mosquitto_loop(mosq, -1, 1);
//			if(run && rc) {
//				printf("connection error!\n");
//				sleep(10);
//				mosquitto_reconnect(mosq);
//			}
//			sleep(100);
//			break;
			int ret = mosquitto_subscribe(mosq, NULL, "test", 0);
			while(run) {
				rc = mosquitto_loop(mosq, -1, 1);
				if(run && rc) {
					sleep(10);
					mosquitto_reconnect(mosq);
				}
			}
		mosquitto_disconnect(mosq);
		return 0;
		rc = mosquitto_connect(mosq, mqtt_host, mqtt_port, 0);
		rbuffer = oauth_read(mosq);

		strcpy(auth_code, server_login(rbuffer, DEVICE_ID, DEVICE_PASSWD));
		rc = oauth_write(mosq, auth_code, strlen(auth_code)+1);


		while(run) {
			rc = mosquitto_loop(mosq, -1, 1);
			if(run && rc) {
				printf("connection error!\n");
				sleep(10);
				mosquitto_reconnect(mosq);
			}
			sleep(60);
			break;
		}
		

		mosquitto_destroy(mosq);
	}	

	mosquitto_lib_cleanup();

	return rc;
}
