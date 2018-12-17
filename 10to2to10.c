#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int alter_binary(int decimal , int position , int binary_0or1)
{
	//printf("decimal=%d\nposition=%d\nbinary_0or1=%d\n",decimal,position,binary_0or1);

	int remainder;
	int binary[256];
	int i=0 , j ;
                 int decimal_out=0;
	
	while(decimal>1)
	{
		remainder = decimal % 2;
		binary [i] = remainder;
		i++;
		decimal = decimal / 2;
	}
	remainder = decimal;
	binary[i] = remainder;

//	printf("binary=");
//	for(j=0;j<=i;j++)
//	{
//		printf("%d",binary[i-j]);
//	}
//	printf("\n");	

                 
        binary[position-1]=binary_0or1;
	
//	printf("binary_2=");
//	for(j=0;j<=i;j++)
//	{
//		printf("%d",binary[i-j]);
//	}
//	printf("\n");
	
                    
         for(j=0;j<=i;j++)
         {
		//printf("%d * 2^%d\t",binary[j],j);
                decimal_out += binary[j] * pow(2,j);
		//printf("decimal_out=%d\n",decimal_out);
          }


	return decimal_out;
}

int main()
{
	printf("decimal_out=%d\n",alter_binary(7,2,1));
	return 0;
}
