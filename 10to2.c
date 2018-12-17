#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main()
{
	int a;
	int b;
	int c[256],d[256];
	int i=0,j;

	scanf("%d",&a);

	//余数排列
	while(a>1)
	{
		b=a%2;
		c[i]=b;
		i++;
		a=a/2;
	}
	b=a;
	c[i]=b;
	
	//余数反排列
	printf("c=");
	for(j=0;j<i+1;j++)
	{
		printf("%d",c[j]);
	}
	printf("\n");
	printf("i=%d\n",i);
	
	printf("binary=");
	int s=i;	
	for(j=0;j<s+1;j++)
	{
		d[j]=c[i];
		i--;
		printf("%d",d[j]);
	}
	printf("\n");

	printf("\nd[%d]=%d\n",s-1,d[s-1]);
	d[s-1]=1;

	int m=s;
	printf("binary2=");	
	for(j=0;j<=m;j++)
	{
		printf("%d",d[j]);
	}
	printf("\n");
	
	int decimalNumber=0;
		
	for(j=0;m>=0;j++)
	{
		printf("m=%d\t",m);
		printf("%d * 2^%d\t",d[m],j);
		decimalNumber += d[m] * pow(2,j);
		m--;
		printf("decimalNumber=%d\n",decimalNumber);		
	}
	printf("decimalNumber=%d",decimalNumber);


	return 0;
}
