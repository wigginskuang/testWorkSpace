#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pwd.h>
#include <signal.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <shadow.h>
#include <crypt.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <linux/capability.h>
#include <sys/syscall.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <malloc.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdarg.h>
#include <iconv.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <mosquitto.h>
#include <math.h>
#include <termios.h>
#include "cJSON.h"


int main(char* plateno,int order_id) {
    while(1)
    {
        mosquitto_lib_init();

        struct mosquitto *mosq = NULL;

    //创建一个mosquitto客户端
        char client_id[] = "441903/111022684/ReturnOpen";
        bool session = true;
        mosq = mosquitto_new(client_id, session, NULL);
        if(!mosq){
            perror("mosquitto_new");
        }

    //创建用户密码
        char *username = "test_of_kuaichuang";
        char *password = "123456";
        mosquitto_username_pw_set(mosq,username,password);

    //连接服务器
        int ret = mosquitto_connect(mosq, "117.48.206.233", 1883, 60);
        if(ret != 0){
           perror("mosquitto_connect");
       }

    //使用cJSON创建json对象
       cJSON *o_json = cJSON_CreateObject();
       if(o_json == NULL){
         perror("create json object failed");
     }
     cJSON_AddStringToObject(o_json, "plateNumber", plateno);
     cJSON_AddNumberToObject(o_json, "order_id",order_id);
     cJSON_AddStringToObject(o_json, "equipmentCode", "441903/111022684");
     char* str = cJSON_Print(o_json);

    //发布主题
     char topic[] = "/ReturnOpen";
     int qos = 2;
     mosquitto_publish(mosq, NULL, topic, strlen(str), str, qos, false);
   // mosquitto_loop_forever(mosq, -1, 1);

     cJSON_Delete(o_json);
     mosquitto_destroy(mosq);
     mosquitto_lib_cleanup();


    /*开闸*/
    // open_barrier();

 }
}