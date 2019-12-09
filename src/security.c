
int mosquitto_oauth_flow(struct mosquitto* context, char* username, char* password)
{
	CURL *curl;
	CURLcode res;
	struct MemoryStruct chunk;

	chunk.memory = malloc(1);
	chunk.size = 0;
	
	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();

	struct curl_slist *list = NULL;


	if(curl) {
		//Authenticate Broker's information from Server.
			curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/client_checkInfo.php");
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "cid=cid&passwd=passwd");
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
			res = curl_easy_perform(curl);
		

			if(strcmp(chunk.memory,"Wrong Info")) {
				int payloadlen = strlen(chunk.memory) + strlen(CID);
				char* message = (char*)malloc(payloadlen);
				sprintf(message, "uri=\"%s\" cid=\"%s\"", chunk.memory, CID);
			
				//Send login URL to Device.
				int rc1 = write(context->sock, message, strlen(message)+1);
				char* auth_code = (char*)malloc(24);
				if(rc1>0) {
					//Receive auth_code from Deivce.
					int rc2;
					while(rc2 = read(context->sock, auth_code, 22)==-1) {
						if(errno == EINTR || errno == 11) continue;
						else {
							printf("recv error : %d\n", errno);
							break;
						}
					}
					chunk.memory = realloc(chunk.memory, 1);
					chunk.size = 0;
				
					//Exchange auth_code to access_token.
					if(strlen(auth_code) == 20) {
						curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/fetch_token.php");
						char post[100] = "cid=";
						strcat(post, CID);
						strcat(post,"&passwd=");
						strcat(post, PASSWD);
						strcat(post, "&auth_code=");
						strcat(post, auth_code);
					
						curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
						curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
						curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
						res = curl_easy_perform(curl);
						chunk.memory[strlen(chunk.memory)-1] = '\0';
						if(strlen(chunk.memory) == 40) {
							context->password = mosquitto__strdup(chunk.memory);	
							strcpy(post, "");
							strcpy(post, "cid=");
							strcat(post, CID);
							strcat(post, "&passwd=");
							strcat(post, PASSWD);
							strcat(post, "&access_token=");
							char *ptr = strtok(chunk.memory,'/');
							strcat(post, ptr);

							chunk.memory = realloc(chunk.memory, 1);
							chunk.size = 0;
						
							curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/get_data.php");
							curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
							curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
							curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
							res = curl_easy_perform(curl);

							if(!strcmp(chunk.memory, "Wrong access token") || !strcmp(chunk.memory, "Wrong client")) 
								return MQTT_RC_BAD_USERNAME_OR_PASSWORD;
							 
							
							if(chunk.memory[strlen(chunk.memory)-1] == '\n')
								chunk.memory[strlen(chunk.memory)-1] = '0';
							context->username = mosquitto__strdup(chunk.memory);
	
							return MQTT_RC_SUCCESS;
						}
						else return MQTT_RC_BAD_USERNAME_OR_PASSWORD;
					}
					else return MQTT_RC_BAD_USERNAME_OR_PASSWORD;
				}
			}
			else return MQTT_RC_BAD_USERNAME_OR_PASSWORD;
	}
	else return MQTT_RC_CURL_ERROR; 
}
