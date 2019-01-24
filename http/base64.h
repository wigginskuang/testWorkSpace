#ifndef BASE64
#define BASE64


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// bindata待编码数据buffer  base64 编码后数据buffer  binlength 待编码数据大小
char *base64_encode( const unsigned char * bindata, char * base64, int binlength);
// base64编码字符串 bindata 解码后buffer
int base64_decode( const char * base64, unsigned char * bindata);

char *pic_to_base64();


#endif