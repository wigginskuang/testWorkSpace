#include<stdio.h>
#include<time.h>

int main()
{
	time_t t;
	char buf[1024];
	time(&t);
	ctime_r(&t,buf);
	printf("%s\n",buf);
	return 0;
}
