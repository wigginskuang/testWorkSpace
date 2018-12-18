#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include"mysocket.h"

int main()
{
	//创建一个socket
	int socketfd = socket(AF_INET,SOCK_STREAM,0);
	printf("create socket succ，socketfd=%d\n",socketfd);

	/*
	头文件：netinet/in.h
	*/
	struct sockaddr_in clientaddr;
	memset(&clientaddr,0,sizeof(clientaddr));
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = 5188;
	clientaddr.sin_addr.s_addr = inet_addr("192.168.1.106");
	
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



    while(1)
    {
		struct packet send_buffer;
		struct packet recv_buffer;
		memset(&send_buffer,0,sizeof(send_buffer));
		memset(&recv_buffer,0,sizeof(recv_buffer));
	
		//初始化数据包
		//初始化包体
	    printf("send:");
	    fgets(send_buffer.body,sizeof(send_buffer.body),stdin);
		/*初始化包头*/
		int n = strlen(send_buffer.body);//n代表数据包中包体的长度
		send_buffer.head = htonl(n);//网络中发送的数据要变成网络字节序



	    //write_n
	    ssize_t written_bytes = write_n(socketfd,&send_buffer,4+n);//把数据包发送出去，包括包头和包体
	    printf("written_bytes=%ld\n",written_bytes);

	    
	   

		
					/*recv*/
			        //int recv_bytes = recv(socketfd,buffer_read,sizeof(buffer_read),0);
					/*先接收包头，进而确认包体的长度*/
					int recv_bytes = read_n(socketfd,&recv_buffer.head,4);//先接收四个字节，既是先接收数据包的包头
			        if(recv_bytes == -1)//说明对方退出
			        {
				        perror("");
			        }
			        else if(recv_bytes < 4)//如果接收<4，说明对方中途中断了
			        {
				        printf("[child close  client disconnect]\n");
				        break;
			        }
			        
					n=ntohl(recv_buffer.head);//由于包头接收的是网络字节序，所以要转换成主机字节序；

					int ret=read_n(socketfd,recv_buffer.body,n);
					if(ret == -1)//说明对方退出
			        {
				        perror("");
			        }
			        else if(ret < n)//如果接收<n，说明对方中途中断了
			        {
				        printf("[child close  client disconnect]\n");
				        break;
			        }

			        
			        //打印接收到的字符串
			        printf("recv:%s\n",recv_buffer.body);
					//fputs(recv_buffer.body,stdout);
    }
	/*close
	*/
	close(socketfd);
	
	return 0;
}
