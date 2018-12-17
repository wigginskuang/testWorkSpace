#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <string.h>

#define die(m) do{ perror(m); exit(EXIT_FAILURE);}while(0)

void handle(struct mosquitto *mosq, void *data, const struct mosquitto_message *message){
	printf("msg coming\n");
	if(message->payloadlen){
		printf("topic:%s, mess:%s\n", message->topic, (char*)message->payload);
	}
	fflush(stdout);
}

int main(){

	//初始化mosquito库
	mosquitto_lib_init();

	struct mosquitto *mosq = NULL;

	//创建一个mosquitto客户端
	char client_id[] = "myid";
	bool session = true;
	mosq = mosquitto_new(client_id, session, NULL);
	if(!mosq){
		die("mosquitto_new");		
	}

	//连接服务器
	int ret = mosquitto_connect(mosq, "192.168.1.146", 1883, 60);
	printf("sub ret=%d\n",ret);	
	die("mosquitto_connect");


	//订阅主题	
	mosquitto_subscribe(mosq, NULL, "test", 2);
	die("subscribe");

	//当有信息从broker过来就会调用这个函数
	mosquitto_message_callback_set(mosq, handle);

	//开启一个线程，在线程里不停的调用 mosquitto_loop() 来处理网络信息环处理网络消息
	mosquitto_loop_forever(mosq, -1, 1);

	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;
}
