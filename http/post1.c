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
#include "cJSON.h"
#include <netdb.h>


#define DOMAIN  "apizhiduoc.kuaichuangkeji.com"
#define IPSTR "39.108.161.21"
#define PORT 80
#define BUFSIZE 1024






#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// bindata待编码数据buffer  base64 编码后数据buffer  binlength 待编码数据大小
char *base64_encode( const unsigned char * bindata, char * base64, int binlength);
// base64编码字符串 bindata 解码后buffer
int base64_decode( const char * base64, unsigned char * bindata);




const char * base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


/*base64编码：图片转base64*/
char *base64_encode( const unsigned char * bindata, char * base64, int binlength )
{
    int i, j;
    unsigned char current;
 
    for ( i = 0, j = 0 ; i < binlength ; i += 3 )
    {
        current = (bindata[i] >> 2) ;
        current &= (unsigned char)0x3F;
        base64[j++] = base64char[(int)current];
 
        current = ( (unsigned char)(bindata[i] << 4 ) ) & ( (unsigned char)0x30 ) ;
        if ( i + 1 >= binlength )
        {
            base64[j++] = base64char[(int)current];
            base64[j++] = '=';
            base64[j++] = '=';
            break;
        }
        current |= ( (unsigned char)(bindata[i+1] >> 4) ) & ( (unsigned char) 0x0F );
        base64[j++] = base64char[(int)current];
 
        current = ( (unsigned char)(bindata[i+1] << 2) ) & ( (unsigned char)0x3C ) ;
        if ( i + 2 >= binlength )
        {
            base64[j++] = base64char[(int)current];
            base64[j++] = '=';
            break;
        }
        current |= ( (unsigned char)(bindata[i+2] >> 6) ) & ( (unsigned char) 0x03 );
        base64[j++] = base64char[(int)current];
 
        current = ( (unsigned char)bindata[i+2] ) & ( (unsigned char)0x3F ) ;
        base64[j++] = base64char[(int)current];
    }
    base64[j] = '\0';
    return 0;
}

/*base64解码：base64转图片*/ 
int base64_decode( const char * base64, unsigned char * bindata )
{
    int i, j;
    unsigned char k;
    unsigned char temp[4];
    for ( i = 0, j = 0; base64[i] != '\0' ; i += 4 )
    {
        memset( temp, 0xFF, sizeof(temp) );
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i] )
                temp[0]= k;
        }
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i+1] )
                temp[1]= k;
        }
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i+2] )
                temp[2]= k;
        }
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i+3] )
                temp[3]= k;
        }
 
        bindata[j++] = ((unsigned char)(((unsigned char)(temp[0] << 2))&0xFC)) |
                ((unsigned char)((unsigned char)(temp[1]>>4)&0x03));
        if ( base64[i+2] == '=' )
            break;
 
        bindata[j++] = ((unsigned char)(((unsigned char)(temp[1] << 4))&0xF0)) |
                ((unsigned char)((unsigned char)(temp[2]>>2)&0x0F));
        if ( base64[i+3] == '=' )
            break;
 
        bindata[j++] = ((unsigned char)(((unsigned char)(temp[2] << 6))&0xF0)) |
                ((unsigned char)(temp[3]&0x3F));
    }
    return j;
}


char *imageBase64;

void pic_to_base64()
{   
    FILE *fp = NULL;
    unsigned int imageSize;        //图片字节数
    char *imageBin;               
    
    char *imageOutput;
    size_t result;
    char *ret; 
    unsigned int base64StrLength;

    fp = fopen("1.png","rb");   //待编码图片
    if (NULL == fp)
    {
        perror("file open file");
    }
    //获取图片大小
    fseek(fp, 0L, SEEK_END);
    imageSize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
 
    //分配内存存储整个图片
    imageBin = (char *)malloc(sizeof(char)*imageSize);
    if (NULL == imageBin)
    {
        printf("malloc failed");
    }
 
    //读取图片
    result = fread(imageBin, 1, imageSize, fp);
     if (result != imageSize)  
    {  
        printf("file read failed");  
    }  
    fclose(fp);
    
    //分配编码后图片所在buffer
    imageBase64 = (char *)malloc(sizeof(char)*imageSize*2);//因为编码一版会比源数据大1/3的样子，这里直接申请源文件一倍的空间
    if (NULL == imageBase64)
    {
        printf("malloc failed");
    }

    //图片转base64编码
    base64_encode(imageBin, imageBase64, imageSize);
    base64StrLength = strlen(imageBase64);
    printf("base64 str length:%d\n", base64StrLength);
    // printf("%s\n\n", imageBase64);
 
    
    free(imageBin);
    // free(imageBase64);
    // return imageBase64;
}




 
int main(int argc, char **argv)
{
        int sockfd, ret, i, h;
        struct sockaddr_in servaddr;
        char str1[2048], *str2, buf[BUFSIZE], *str;
        socklen_t len;
        fd_set   t_set1;
        struct timeval  tv;

        struct hostent *hp;
        char ip[25];


        pic_to_base64();
        //printf("%s\n",imageBase64);
        char *test;
        test = "test";
        printf("%s\n",test);

        //把域名地址转成主机地址
        if((hp = gethostbyname(DOMAIN)) == NULL){
            perror("gethostbyname");
            exit(0);
        }

        strcpy(ip , inet_ntoa(*(struct in_addr *)hp->h_addr_list[0]));
        printf("%s\n",ip );

 
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
                printf("创建网络连接失败,本线程即将终止---socket error!\n");
                exit(0);
        };
 
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0 ){
                printf("创建网络连接失败,本线程即将终止--inet_pton error!\n");
                exit(0);
        };
 
        if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
                printf("连接到服务器失败,connect error!\n");
                exit(0);
        }
        printf("与远端建立了连接\n");


        //使用cJSON创建json对象
        cJSON *o_json = cJSON_CreateObject();
        if(o_json == NULL){
            perror("create json object failed");
            free(imageBase64);
        }

        cJSON_AddStringToObject(o_json,"picture",imageBase64);
        char* str_json = cJSON_Print(o_json);
        if(str_json==NULL)
        {
            perror("cJSON_Print\n");
            free(imageBase64);
        }
        printf("%s\n",str_json);
        printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");


        /* ×××××××××××××××拼接字符串×××××××××××××××× */  
        //strcat(str2,str_json);
        printf("%ld\n",strlen(str_json));
        str2 = (char *)malloc(strlen(str_json));
        if(str2 == NULL)
        {
            perror("str2\n");
            free(str2);
        }



        

        str=(char *)malloc(128);
        len = strlen(str_json);
        printf("%d\n",len );
        sprintf(str, "%d", len);
 
        strcat(str1, "POST /consumption/test HTTP/1.1\n");
        strcat(str1, "Host:apizhiduoc.kuaichuangkeji.com\n");
        strcat(str1, "Content-Type: application/json\n");
        strcat(str1, "Content-Length: ");
        strcat(str1, str);
        strcat(str1, "\n\n");
        //str2的值为post的数据
        strcat(str1, str2);
        strcat(str1, "\r\n\r\n");
        printf("---------------------\n");
        printf("%s\n",str1);
 
        ret = write(sockfd,str1,1024+2641030);
        if (ret < 0) {
                printf("发送失败！错误代码是%d，错误信息是'%s'\n",errno, strerror(errno));
                exit(0);
        }else{
                printf("消息发送成功，共发送了%d个字节！\n\n", ret);
        }


        free(imageBase64);

 
        FD_ZERO(&t_set1);
        FD_SET(sockfd, &t_set1);
 
        tv.tv_sec= 0;
        tv.tv_usec= 0;
        h= 0;

        while(1){
                sleep(2);
                printf("--------------->1");
                h = select(sockfd +1, &t_set1, NULL, NULL, &tv);
                printf("--------------->2");
                //if (h == 0) continue;
                if (h == -1) {
                        close(sockfd);
                        printf("在读取数据报文时SELECT检测到异常，该异常导致线程终止！\n");
                        return -1;
                };
                if ( FD_ISSET(sockfd, &t_set1) ){
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