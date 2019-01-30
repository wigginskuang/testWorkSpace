#include <stdio.h>
#include <stdlib.h>

#include "post_base64.h"

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



/*******************************************************
*
* Function name     ：pic_turn_base64
* Description       : 图片转成base64编码
* Parameter         ：
*       image_path  需要转码的图片的路径
*       imageBase64 输出后base64编码字符串指针       
* Return            :成功返回一个char指针，失败则指针为null
**********************************************************/
char *pic_turn_base64(char *image_path,char *imageBase64)
{   
	FILE *fp = NULL;
    unsigned int imageSize;        //图片字节数
    char *imageBin;               
    size_t result;
    char *ret; 
    unsigned int base64StrLength;

    fp = fopen(image_path,"rb");   //待编码图片
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
    	perror("malloc failed");
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
    //printf("base64 str length:%d\n", base64StrLength);
    // printf("%s\n\n", imageBase64);

    free(imageBin);
    // free(imageBase64);
    return imageBase64;
}



void post_base64()
{
	/***获取图片***/
	system("ffmpeg -y -i rtsp://192.168.2.20:554/mpeg4cif -ss 00:00:01 -vframes 1 -f image2 -vcodec png image.png");


	/***转码成base64编码***/
	char *image_base64 = NULL;
	image_base64 = pic_turn_base64("./image.png",image_base64);


	/***通过post传输base64数据***/
	int sockfd, ret, i, h;
	struct sockaddr_in servaddr;
	char *str1, *str2, buf[BUFSIZE], *str;
	socklen_t len;
	fd_set   t_set1;
	struct timeval  tv;

	struct hostent *hp;
	char ip[25];


    //创建socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		printf("创建网络连接失败,本线程即将终止---socket error!\n");
		exit(0);
	};

    //把域名地址转成主机地址
	if((hp = gethostbyname(DOMAIN)) == NULL){
		perror("gethostbyname");
		exit(0);
	}
	strcpy(ip , inet_ntoa(*(struct in_addr *)hp->h_addr_list[0]));

    //定义sockaddr
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0 ){
		printf("创建网络连接失败,本线程即将终止--inet_pton error!\n");
		exit(0);
	};

    //连接到服务器
	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		printf("连接到服务器失败,connect error!\n");
		exit(0);
	}
	printf("与远端建立了连接\n");

    //post报文
    //使用cJSON创建json对象
	cJSON *o_json = cJSON_CreateObject();
	if(o_json == NULL){
		perror("create json object failed");
		free(imageBase64);
	}
	cJSON_AddStringToObject(o_json,"picture",image_base64);
	char* str_json = cJSON_Print(o_json);
	if(str_json==NULL)
	{
		perror("cJSON_Print\n");
		free(imageBase64);
	}
        //printf("%s\n",str_json);
	printf("%ld\n",strlen(str_json));


	str=(char *)malloc(128);
	if(str == NULL){
		perror("str malloc fialed\n");
	}
	len = strlen(str_json);
	sprintf(str, "%d", len);

	str1 = (char *)malloc(2048+strlen(str_json));
	if(str1 == NULL){
		perror("str1 malloc");
		free(image_base64);
	}

	strcat(str1, "POST /consumption/test HTTP/1.1\n");
	strcat(str1, "Host:apizhiduoc.kuaichuangkeji.com\n");
	strcat(str1, "Content-Type: application/json\n");
	strcat(str1, "Content-Length: ");
	strcat(str1, str);
	strcat(str1, "\n\n");
	printf("%s\n",str1);
        // printf("%ld\n", strlen(str1));
        // str1 = (char *)malloc(strlen(str1)+strlen(str_json)+1);

        //post请求报文的body
	strcat(str1, str_json);
	strcat(str1, "\r\n\r\n");
	printf("---------------------\n");
        //printf("%s\n",str1);


        // 发送post请求报文
	ret = write(sockfd,str1,strlen(str1));
	if (ret < 0) {
		printf("发送失败！错误代码是%d，错误信息是'%s'\n",errno, strerror(errno));
		exit(0);
	}else{
		printf("消息发送成功，共发送了%d个字节！\n\n", ret);
	}


	free(imageBase64);
	free(str);
	free(str1);
	free(image_base64m);
	cJSON_Delete(o_json);


    /*×××××××××接收响应报文*********************/ 
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
}