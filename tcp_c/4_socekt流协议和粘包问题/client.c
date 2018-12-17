#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>

int main()
{
	//创建一个socket
	int socketfd = socket(AF_INET,SOCK_STREAM,0);
	printf("create socket succ，socketfd=%d\n",socketfd);

	/*
	头文件：#include<netinet/in.h>
	*/
	struct sockaddr_in clientaddr;
	memset(&clientaddr,0,sizeof(clientaddr));
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = 5188;
	clientaddr.sin_addr.s_addr = inet_addr("192.168.1.146");
	
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
	    /*write
	    ssize_t write(int sockfd, const void buf, size_t nbytes);
	    说明：把buffer中的n bytes字节内容写入socketfd中
	    头文件：unistd.h
	    int sockfd：套接字描述符
	    const void buf:写入数据的缓冲区
	    size_t nbytes:写入的大小
	    返回值：成功，返回写入buf的字节数；失败，小于0*/
	    printf("send:");
	    char buffer_write[1024]="\0";
	    fgets(buffer_write,1024,stdin);

	    
	    //ssize_t written_bytes = write(socketfd,buffer_write,sizeof(buffer_write));
	    //printf("written_bytes=%ld\n",written_bytes);

	    
	    /*send
	    int send(int sockfd,void *buf,int len,int flags)
	    说明：把buffer的数据copy到socket的发送缓冲区
	    头文件：sys/socket.h 
	    int sockfd：套接字描述符
	    const void buf:自定义的发送数据的缓冲区
	    size_t nbytes:发送多少字节
	    int flags:一般为0，MSG_DONTROUTE 勿将数据路由出本地网络|MSG_DONTWAIT 允许非阻塞操作（等价于使用O_NONBLOCK）|MSG_EOR 如果协议支持，此为记录结束|MSG_OOB 如果协议支持，发送带外数据|MSG_NOSIGNAL 禁止向系统发送异常信息
	    返回值：send 不包含传送失败的提示信息，如果检测到本地错误将返回-1。因此，如果send 成功返回，并不必然表示连接另一端的进程接收数据。所保证的仅是当send 成功返回时，数据已经无错误地发送到网络上。*/
	    int sent_bytes = send(socketfd,buffer_write,sizeof(buffer_write),0);
	    //printf("written_bytes=%d\n",sent_bytes);
	    memset(buffer_write,0,sizeof(buffer_write));
	    
	    
        /*recv*/
	    char buffer_read[1024];
	    int recv_bytes = recv(socketfd,buffer_read,sizeof(buffer_read),0);
	    if(recv_bytes == -1)
	    {
		    perror("");
	    }
	    else if(recv_bytes == 0)
	    {
		    printf("error:disconnect\n");
		    break;
	    }
	    //printf("recv_bytes=%d\n",recv_bytes);
			    
	    //打印接收到的字符串
	    printf("receive:%s\n",buffer_read);
	    memset(buffer_read,0,sizeof(buffer_read));
    }
	/*close
	*/
	close(socketfd);
	
	return 0;
}
