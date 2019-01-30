#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "cJSON.h"
#include <netdb.h>


#define DOMAIN  "apizhiduoc.kuaichuangkeji.com"
#define PORT 80
#define BUFSIZE 1024

const char * base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// bindata待编码数据buffer  base64 编码后数据buffer  binlength 待编码数据大小
char *base64_encode( const unsigned char * bindata, char * base64, int binlength);
// base64编码字符串 bindata 解码后buffer
int base64_decode( const char * base64, unsigned char * bindata);



/*******************************************************
* Function name     ：pic_turn_base64
* Description       : 图片转成base64编码
* Parameter         ：
*       image_path  需要转码的图片的路径
*       imageBase64 输出后base64编码字符串指针       
* Return            :成功返回一个char指针，失败则指针为null
**********************************************************/
char *pic_turn_base64(char *image_path,char *imageBase64);



/*******************************************************
* Function name     ：post_base64
* Description       ：通过post64发送base64编码
* Parameter         ：无     
* Return            ：无
**********************************************************/
void post_base64();

