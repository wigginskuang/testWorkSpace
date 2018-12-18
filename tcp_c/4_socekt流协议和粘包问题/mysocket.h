#include<unistd.h>


/*数据包*/
struct packet
{
	int head;//包头,代表包体的长度
	char body[1024];//包体
};


/*
功能：规定接收多少字节（处理粘包问题）*/
//ssize_t 有符号的整数
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

