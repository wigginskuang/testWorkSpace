#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
 
#define IPSTR "apizhiduoc.kuaichuangkeji.com/consumption/test"
#define URL "apizhiduoc.kuaichuangkeji.com/consumption/test"
#define PORT 80
#define BUFSIZE 1024
 
int main(int argc, char **argv)
{
        int sockfd, ret, i, h;
        struct sockaddr_in servaddr;
        char *hostname = URL;
        char ip[20]={0};
        struct hostent *hp;
        char str1[4096], str2[4096], buf[BUFSIZE], *str;
        socklen_t len;
        fd_set   t_set1;
        struct timeval  tv;
 
        //create socket
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
                perror("创建网络连接失败,本线程即将终止---socket error!\n");
                exit(0);
        };
 
        //把域名地址转成主机地址       
        if((hp = gethostbyname(hostname)) == NULL){
            perror("gethostbyname\n");
            exit(0);
        }
        printf("ip:%s\n",ip);

        strcpy(ip , inet_ntoa(*(struct in_addr *)hp->h_addr_list[0]));

        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0 ){
                perror("创建网络连接失败,本线程即将终止--inet_pton error!\n");
                exit(0);
        };
 
        if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
                perror("连接到服务器失败,connect error!\n");
                exit(0);
        }
        printf("与远端建立了连接\n");
 
        memset(str2, 0, 4096);
        strcat(str2, "qqCode=287101329");
        str=(char *)malloc(128);
        len = strlen(str2);
        sprintf(str, "%d", len);
 
        memset(str1, 0, 4096);
        strcat(str1, "POST /webservices/qqOnlineWebService.asmx/qqCheckOnline HTTP/1.1\n");
        strcat(str1, "Host: www.webxml.com.cn\n");
        strcat(str1, "Content-Type: application/x-www-form-urlencoded\n");
        strcat(str1, "Content-Length: ");
        strcat(str1, str);
        strcat(str1, "\n\n");
        //str2的值为post的数据
        strcat(str1, str2);
        strcat(str1, "\r\n\r\n");
        printf("%s\n",str1);
 
        ret = write(sockfd,str1,strlen(str1));
        if (ret < 0) {
                printf("发送失败！错误代码是%d，错误信息是'%s'\n",errno, strerror(errno));
                exit(0);
        }else{
                printf("消息发送成功，共发送了%d个字节！\n\n", ret);
        }
 
        FD_ZERO(&t_set1);
        FD_SET(sockfd, &t_set1);
 
        while(1){
                sleep(2);
                tv.tv_sec= 0;
                tv.tv_usec= 0;
                h= 0;
                printf("--------------->1");
                h= select(sockfd +1, &t_set1, NULL, NULL, &tv);
                printf("--------------->2");
 
                //if (h == 0) continue;
                if (h < 0) {
                        close(sockfd);
                        printf("在读取数据报文时SELECT检测到异常，该异常导致线程终止！\n");
                        return -1;
                };
 
                if (h > 0){
                        memset(buf, 0, 4096);
                        i= read(sockfd, buf, 4095);
                        if (i==0){
                                close(sockfd);
                                printf("读取数据报文时发现远端关闭，该线程终止！\n");
                                return -1;
                        }
 
                        printf("%s\n", buf);
                }
        }
        close(sockfd);
 
 
        return 0;
}
