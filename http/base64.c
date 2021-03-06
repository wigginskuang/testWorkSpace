#include "base64.h"


const char * base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


/*base64编码：图片转base64*/
char *base64_encode( const unsigned char * bindata, char * base64, int binlength )
{
    int i, j;
    unsigned char current;
 
    for ( i = 0, j = 0 ; i < binlength ; i += 3 )
    {
        current = (bindata[i] >> 2) ;
        current &= (unsigned char)0x3F;
        base64[j++] = base64char[(int)current];
 
        current = ( (unsigned char)(bindata[i] << 4 ) ) & ( (unsigned char)0x30 ) ;
        if ( i + 1 >= binlength )
        {
            base64[j++] = base64char[(int)current];
            base64[j++] = '=';
            base64[j++] = '=';
            break;
        }
        current |= ( (unsigned char)(bindata[i+1] >> 4) ) & ( (unsigned char) 0x0F );
        base64[j++] = base64char[(int)current];
 
        current = ( (unsigned char)(bindata[i+1] << 2) ) & ( (unsigned char)0x3C ) ;
        if ( i + 2 >= binlength )
        {
            base64[j++] = base64char[(int)current];
            base64[j++] = '=';
            break;
        }
        current |= ( (unsigned char)(bindata[i+2] >> 6) ) & ( (unsigned char) 0x03 );
        base64[j++] = base64char[(int)current];
 
        current = ( (unsigned char)bindata[i+2] ) & ( (unsigned char)0x3F ) ;
        base64[j++] = base64char[(int)current];
    }
    base64[j] = '\0';
    return 0;
}

/*base64解码：base64转图片*/ 
int base64_decode( const char * base64, unsigned char * bindata )
{
    int i, j;
    unsigned char k;
    unsigned char temp[4];
    for ( i = 0, j = 0; base64[i] != '\0' ; i += 4 )
    {
        memset( temp, 0xFF, sizeof(temp) );
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i] )
                temp[0]= k;
        }
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i+1] )
                temp[1]= k;
        }
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i+2] )
                temp[2]= k;
        }
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i+3] )
                temp[3]= k;
        }
 
        bindata[j++] = ((unsigned char)(((unsigned char)(temp[0] << 2))&0xFC)) |
                ((unsigned char)((unsigned char)(temp[1]>>4)&0x03));
        if ( base64[i+2] == '=' )
            break;
 
        bindata[j++] = ((unsigned char)(((unsigned char)(temp[1] << 4))&0xF0)) |
                ((unsigned char)((unsigned char)(temp[2]>>2)&0x0F));
        if ( base64[i+3] == '=' )
            break;
 
        bindata[j++] = ((unsigned char)(((unsigned char)(temp[2] << 6))&0xF0)) |
                ((unsigned char)(temp[3]&0x3F));
    }
    return j;
}


/*******************************************************
*
* Function name     ：pic_turn_base64
* Description       : 图片转成base64编码
* Parameter         ：
*       image_path  需要转码的图片的路径
*       imageBase64 输出后base64编码字符串指针       
* Return            :成功返回一个char指针，失败则指针为null
**********************************************************/
char *pic_turn_base64(char *image_path,char *imageBase64)
{	
	FILE *fp = NULL;
	unsigned int imageSize;        //图片字节数
	char *imageBin;               
    char *imageOutput;
	size_t result;
	char *ret; 
	unsigned int base64StrLength;

	fp = fopen(image_path,"rb");   //待编码图片
	if (NULL == fp)
    {
	    perror("file open file");
    }
	//获取图片大小
	fseek(fp, 0L, SEEK_END);
	imageSize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
 
	//分配内存存储整个图片
	imageBin = (char *)malloc(sizeof(char)*imageSize);
	if (NULL == imageBin)
    {
	    printf("malloc failed");
    }
 
	//读取图片
	result = fread(imageBin, 1, imageSize, fp);
	 if (result != imageSize)  
    {  
        printf("file read failed");  
    }  
    fclose(fp);
    
    //分配编码后图片所在buffer
	imageBase64 = (char *)malloc(sizeof(char)*imageSize*2);//因为编码一版会比源数据大1/3的样子，这里直接申请源文件一倍的空间
	if (NULL == imageBase64)
    {
	    printf("malloc failed");
    }

    //图片转base64编码
    base64_encode(imageBin, imageBase64, imageSize);
    base64StrLength = strlen(imageBase64);
    // printf("base64 str length:%d\n", base64StrLength);
    // printf("%s\n\n", imageBase64);
 
	free(imageBin);
    //free(imageBase64);
	return imageBase64;
}
