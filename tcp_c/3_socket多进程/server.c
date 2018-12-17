#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>



int main()
{
	/*creat socket
	返回值：成功，socket描述字。失败，INVALID_SOCKET错误，WSAGetLastError()获取相应错误代码。	
	*/
	int listenfd = socket(AF_INET,SOCK_STREAM,0);
	printf("creat socket succ,listenfd = %d\n",listenfd);


	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = 5188;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//接受的是一个网络字节序
	//servaddr.sin_addr.s_addr = inet_addr("192.168.1.105");
	//inet_aton("192.168.1.105",&servaddr.sin_addr)


	/*开启端口重复利用*/
	if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0)
	{
		EER_EXIT("setsockopt");
	}

	/*bind*/
	int bindfd = bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	if(bindfd!=0)
	{
		printf("bind faild\n");
	}
	printf("bind succ\n");

	/*listen*/
	int ret_listen = listen(listenfd,3);
	if(ret_listen!=0)
	{
		printf("listen failed\n");	
	}
	printf("listen succ\n");

	
	

	/*accept
	原形：int accept(int sockfd,struct sockaddr* addr,socklen_t* addrlen);
	说明：从已连接队列中返回第一个的连接
	参数：struct sockaddr* addr：对方的套接字结构
	返回值：成功 新的套接字（主动套接字）；失败 -1
	*/
	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);//socklen_t是一种数据类型
	int socketfd = accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen);
	if(socketfd == -1)
	{
		printf("accept failed\n");
	}
	printf("accept succ  socket=%d\n",socketfd);

	
	/*read
	Ssize_t read(int fd,void *buf,size_t nbyte)
	说明：从socketfd中读取内容到缓冲区buf
	头文件：unistd.h
	原型：*/
	char buffer_read[1024];
	//ssize_t read_bytes = read(socketfd,buffer_read,sizeof(buffer_read));
	//printf("read_bytes=%ld\n",read_bytes);

	/*printf("receive data:");
	for(int i=0;i<1024;i++)
	{
		printf("%c",buffer_read[i]);
	}
	printf("\n");
	*/	

	/*recv
	int recv(int sockfd,void *buf,int len,int flags);
	说明：等socket的接受缓冲区接受完数据后，把数据copy到自定义的buffer里面
	头文件：sys/socket.h 
	int sockfd：套接字描述符
	const void buf:自定义的接受数据的缓冲区
	size_t nbytes:一次多少字节
	int flags:一般为0，MSG_DONTROUTE 绕过路由表查找|MSG_DONTWAIT 仅本操作非阻塞|MSG_OOB 发送或接收带外数据|MSG_PEEK 窥看外来消息|MSG_WAITALL 等待所有数据。
	返回值：若无错误发生，recv()返回读入的字节数。如果连接已中止，返回0。如果发生错误，返回-1，应用程序可通过perror()获取相应错误信息*/
	int recv_bytes = recv(socketfd,buffer_read,sizeof(buffer_read),0);
	if(recv_bytes == -1)
	{
		perror("");
	}
	else if(recv_bytes == 0)
	{
		printf("error:disconnect\n");
	}
	printf("recv_bytes=%d\n",recv_bytes);

	printf("receive data:");
	for(int i=0;i<1024;i++)
	{
		printf("%c",buffer_read[i]);
	}
	printf("\n");

	/*close
	头文件：unistd.h
	说明：如果不关闭套接字的话，内核会一直维护这个套接字，造成浪费资源 
	*/
	close(socketfd);
	close(listenfd);

	return 0;
}
