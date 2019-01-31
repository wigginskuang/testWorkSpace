#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <string.h>
#include <unistd.h>

#define die(m) do{ perror(m); exit(EXIT_FAILURE);}while(0)

int main(){
    int ret;

    //初始化mosquito库
    mosquitto_lib_init();

    struct mosquitto *mosq = NULL;

    //创建一个mosquitto客户端
    char client_id[] = "kwj2";
    bool session = true;
    mosq = mosquitto_new(client_id, session, NULL);
    if(!mosq){
        die("mosquitto_new");
    }

    char *username = "kwj3";
    char *password = "123456";
    mosquitto_username_pw_set(mosq,username,password);

    //连接服务器
    ret = mosquitto_connect(mosq, "117.48.206.233", 1883, 60);
    if(ret != 0){
        die("mosquitto_connect");
    }

    //发布主题
    int i = 0;
    char topic[] = "/zdc/dg/003";
    int qos = 2;
    char mess[256];
    while(1)
    {
        sprintf(mess,"message[%d]",i);
        mosquitto_publish(mosq, NULL, topic, strlen(mess), mess, qos, false);
        printf("%d\n",i);
        i++;
        sleep(1);
    }
    

    //释放
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    

    return 0;
}