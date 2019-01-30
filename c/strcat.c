#include <stdio.h>
#include <string.h>

int main ()
{
   char src[50], dest[20];

   strcpy(src,  "This is source");

   strcpy(dest, "This is destination");
   printf("%ld\n",strlen(dest) );

   strcat(dest, src);

   printf("最终的目标字符串： |%s|\n", dest);
   
   return(0);
}