#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include"mysocket.h"
#include<arpa/inet.h>

int main()
{
	//创建一个socket
	int socketfd = socket(AF_INET,SOCK_STREAM,0);
	printf("create socket succ，socketfd=%d\n",socketfd);
	
	

	/*服务器ip信息*/
	struct sockaddr_in clientaddr;
	memset(&clientaddr,0,sizeof(clientaddr));
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = 5188;
	clientaddr.sin_addr.s_addr = inet_addr("192.168.1.106");
	
	
	/*本地ip信息*/
	struct sockaddr_in localaddr;
	socklen_t addrlen = sizeof(localaddr);
	int socketname = getsockname(socketfd,(struct sockaddr*)&localaddr,&addrlen);
	printf("accept succ socket=%d ip=%s port=%d\n",socketfd,inet_ntoa(localaddr.sin_addr),ntohs(localaddr.sin_port));
	
	/*connect
	说明：
	参数：	
	*/	
	int ret_connect = connect(socketfd,(struct sockaddr*)&clientaddr,sizeof(clientaddr));
	if(ret_connect !=0)
	{
		printf("connect failed\n");
	}
	printf("connect succ\n");


		//定义buffer
		char send_buffer[1024];
		char recv_buffer[1024];
		memset(&send_buffer,0,sizeof(send_buffer));
		memset(&recv_buffer,0,sizeof(recv_buffer));	


    while(1)
    {
                //初始化send_buffer
                printf("send:");
                fgets(send_buffer,sizeof(send_buffer),stdin);
        

	    //write_n
	    ssize_t written_bytes = write_n(socketfd,send_buffer,strlen(send_buffer));
	    printf("written_bytes=%ld\n",written_bytes);

	    
	   

		
		/*recv接收server回射回来的数据*/
		int recv_bytes = recv_line(socketfd,recv_buffer,sizeof(recv_buffer));//按行读取
		if(recv_bytes == -1)//说明对方退出
		{
				        perror("");
		}
			        else if(recv_bytes == 0)//如果接收<4，说明对方中途中断了
			        {
				        printf("[child close  client disconnect]\n");
				        break;
			        }
		

			        
		//打印接收到的字符串
		fputs(recv_buffer,stdout);
		
		memset(&send_buffer,0,sizeof(send_buffer));
		memset(&recv_buffer,0,sizeof(recv_buffer));
		
    }
	/*close*/
	close(socketfd);
	
	return 0;
}
