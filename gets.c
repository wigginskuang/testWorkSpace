#include <stdio.h>
int main(void)
{
        char str[20] = "\0";  //字符数组初始化\0
        printf("请输入字符串：");
        fgets(str,20,stdin);
        printf("%s\n", str);
        return 0;
}
