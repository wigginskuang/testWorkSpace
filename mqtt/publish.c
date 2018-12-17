#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <string.h>

#define die(m) do{ perror(m); exit(EXIT_FAILURE);}while(0)


int main(){

	//初始化mosquito库
	mosquitto_lib_init();

	struct mosquitto *mosq = NULL;

	//创建一个mosquitto客户端
	char client_id[] = "myid002";
	bool session = true;
	mosq = mosquitto_new(client_id, session, NULL);
	if(!mosq){
		die("mosquitto_new");		
	}

	//连接服务器
	int ret = mosquitto_connect(mosq, "192.168.1.146", 1883, 60);
	printf("ret=%d\n",ret);
	if(ret != 0){
		die("mosquitto_connect");
	}

 	int loop=mosquitto_loop_start(mosq);
	if(loop!=MOSQ_ERR_SUCCESS)
	{
		die("loop");
		return 1;
	}


	//发布主题	
	char topic[] = "test";
	int qos = 2;
	char mess[] = "o world";
	int ret_pub=mosquitto_publish(mosq, NULL, topic, strlen(mess), mess, qos, false);
	if(ret!=0)
	{
		die("mosquitto_publish");
	}
	

	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;
}
