#include<stdio.h>
#include<arpa/inet.h>
int main()
{
	int x = 0x12345678;//十六进制
	char *p=(char *)&x;//强制转换
	printf("%0x %0x %0x %0x\n",p[0],p[1],p[2],p[3]);

	int y = htonl(x);//host to network (return long)
	
	p=(char *)&y;
	printf("%0x %0x %0x %0x",p[0],p[1],p[2],p[3]);
}
