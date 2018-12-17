#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>



int main()
{
	/*creat socket
	返回值：成功，socket描述字。失败，INVALID_SOCKET错误，WSAGetLastError()获取相应错误代码。	
	*/
	int listenfd = socket(AF_INET,SOCK_STREAM,0);
	printf("creat socket succ,listenfd = %d\n",listenfd);

	/*设置struct sockaddr_in*/
	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = 5188;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//接受的是一个网络字节序
	//servaddr.sin_addr.s_addr = inet_addr("192.168.1.105");
	//inet_aton("192.168.1.105",&servaddr.sin_addr)


	/*开启端口重复利用*/
	int on = 1;
	if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0)
	{
		printf("setsockopt\n");
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

	
	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);//socklen_t是一种数据类型
	int i=0;

    while(1)
    {
	    /*accept 默认阻塞程序，等待连接,当接收到连接会把客户端的ip填到peeraddr中*/
	    int socketfd = accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen);
	    if(socketfd == -1)
	    {
		    printf("accept failed\n");
	    }
	    else if(socketfd>0)
	    {
	         printf("accept succ socket=%d ip=%s port=%d\n",socketfd,inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));
	         i++;
	    }
	   
	    
		    
	    /*当有新连接，创建一个子进程*/
	    pid_t pid = fork();//成功返回两次，第一次在父进程返回子进程的进程号，第二次在子进程返回0，失败返回-1
	    if(pid == -1)
	    {
		    printf("fork failed\n");		
	    }
	    if(pid == 0)//子进程
	    {
	            //如果没有这个while(1),server只接收一次
	            while(1)
	            {
			        /*recv*/
			        char buffer_read[1024];
			        int recv_bytes = recv(socketfd,buffer_read,sizeof(buffer_read),0);
			        if(recv_bytes == -1)
			        {
				        perror("");
			        }
			        else if(recv_bytes == 0)
			        {
				        printf("[child close  client disconnect]\n");
				        close(listenfd);
				        exit(EXIT_SUCCESS);
				        break;
			        }
			        //printf("recv_bytes=%d\n",recv_bytes);
			        
			        //打印接收到的字符串
			        printf("client[%d]:%s\n",i,buffer_read);
			        
			        
			        //send
			        int sent_bytes = send(socketfd,buffer_read,sizeof(buffer_read),0);
	                //printf("written_bytes=%d\n",sent_bytes);
	                memset(buffer_read,0,sizeof(buffer_read));
	            }
	            exit(EXIT_SUCCESS);				
	    }
	    //父进程
	    printf("pid=%d\n",pid);
	    /*close*/
	    close(socketfd);
	    
	}
	return 0;
}
