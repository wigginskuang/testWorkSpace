#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <string.h>

#define die(m) do{ perror(m); exit(EXIT_FAILURE);}while(0)

int i;

void connect_handle(struct mosquitto *mosq, void *userdata, int result)
{
    int i;
    if(!result){
        printf("connect ok!\n");

        //订阅主题
        char topic[] = "/zdc/dg/002";
        int qos = 2;
        mosquitto_subscribe(mosq, NULL, topic, qos);
    }else{
        printf("disconnect!\n");
        mosquitto_reconnect(mosq);
    }
}

void handle(struct mosquitto *mosq, void *data, const struct mosquitto_message *message){
        if(message->payloadlen){
                printf("topic:%s, mess:%s[%d]\n", message->topic, (char*)message->payload ,i);
                i++;
        }
        fflush(stdout);
}

int main(){
    i=0;

        //初始化mosquito库
        mosquitto_lib_init();

        struct mosquitto *mosq = NULL;

        //创建一个mosquitto客户端
        char client_id[] = "kwj1";
        bool session = true;
        mosq = mosquitto_new(client_id, session, NULL);
        if(!mosq){
                die("mosquitto_new");
        }

        char *username = "kwj";
                    char *password = "123456";
                    mosquitto_username_pw_set(mosq,username,password);

	//连接服务器
	int ret = mosquitto_connect(mosq, "117.48.206.233", 1883, 60);
    perror("mosquitto_connect\n");
	if(ret != 0){
		die("mosquitto_connect");
	}

	//订阅主题	
	char topic[] = "/zdc/dg/003";
	int qos = 2;
	mosquitto_subscribe(mosq, NULL, topic, qos);
    perror("mosquitto_subscribe");

	//设置回调函数
    mosquitto_connect_callback_set(mosq,connect_handle);
	mosquitto_message_callback_set(mosq, handle);

	//循环处理网络消息
	mosquitto_loop_forever(mosq, -1, 1);

	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;
}
