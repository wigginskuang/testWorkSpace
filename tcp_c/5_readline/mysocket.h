#include<unistd.h>
#include<errno.h>
#include<stdlib.h>


/*数据包*/
struct packet
{
	int head;//包头,代表包体的长度
	char body[1024];//包体
};


/*
功能：规定接收多少字节（处理粘包问题）*/
//ssize_t 有符号的整数 %ld
//参数：fd 套接字 | 指向一个buffer的指针 | 我要读取的字节数
ssize_t read_n(int fd , void *buffer , size_t count)
{
        size_t nleft = count;    //剩余接收的字节数
        size_t nread =0;    //已接收的字节数
        char *p = (char*)buffer;   //定义一个char指针指向buffer
        //只要nleft大于0，说明还有字节数没有接受完
        while(nleft>0)
        {
                nread = read(fd , p , nleft);
                if(nread<0) //读到的字节数小于0，报错返回
                {
                    return -1;
                }
                else if(nread == 0)//说明对方关闭连接
                {
                    break;
                }
                //读到数据，指针偏移
                p += nread;
                nleft -= nread;
				//printf("nread=%ld\n",nread);
				//printf("nleft=%ld\n",nleft);
        }
        return nread;
}


/*write_n
功能：规定发送多少字节数据，
头文件：unistd.h
把buffer中的数据复制到socket的发送缓冲区*/
ssize_t write_n(int fd,void *buffer,size_t count)
{
        size_t nleft = count;    //剩余发送的字节数
        size_t nwritten =0;    //已发送的字节数
        char *p = (char*)buffer;   //定义一个char指针指向buffer
        //如果还有数据没有发送完
        while(nleft>0)
        {
                nwritten = write(fd , p , nleft);
                if(nwritten<0) //读到的字节数小于0，报错返回
                {
                    return -1;
                }
                else if(nwritten == 0)//说明对方关闭连接
                {
                    break;
                }
                //如果nwritten>0,说明写入成功，返回写入的字节数
				printf("nwritten=%ld\n",nwritten);

                p += nwritten;//写了多少，buffer_write数组制针偏移多少
                nleft -= nwritten;

				printf("nleft=%ld\n",nleft);
        }
        return count;  
}

//偷窥接收缓冲区的数据，但不移除,偷看到数据就返回数据字节数，如果没有就阻塞
ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
	while(1)
	{
		int ret = recv(sockfd,buf,len,MSG_PEEK);
		if(ret == -1 && errno== EINTR)//-1代表接收失败，enter代表信号中断
		{
			continue;//继续循环做一遍
		}
		return ret;
	}

}

/*接收一行数据‘
实现遇到\n或\t等的消息边界
size_t maxline一行最大的字节数
*/
ssize_t recv_line(int sockfd, void *buf, size_t maxline)
{
	int ret;
	int nread;//已读取字节数
	char *bufp = buf;
	int nleft = maxline;//需要读取的字节数
	while(1)
	{
		ret = recv_peek(sockfd,bufp,nleft);//先偷窥一下有多少数据
		if(ret<0)
		{
			return ret;
		}
		else if(ret == 0 )//表示对方关闭套接口
		{
			return ret;
		}

		nread = ret;
		int i;
		for(i=0;i<nread;i++)//足个字符读取判断是否遇到\n
		{
			if(bufp[i]=='\n')//如果遇到\n说明缓冲区可以移除
			{
				ret = read_n(sockfd,bufp,i+1);
				if(ret != i+1)//说明接收失败
					exit(EXIT_FAILURE);
				return ret;
			}
		}

		//有\n说明满足一条消息，把它读走
		if(nread > nleft)//如果偷窥到缓冲区的字节数大于总共剩余要读取的字节数，说明有问题
			exit(EXIT_FAILURE);

		nleft -= nread;

		
		ret= read_n(sockfd,bufp,nread);
		if(ret != nread)
			exit(EXIT_FAILURE);

		bufp += nread;	
	}
	
	return -1;
}

