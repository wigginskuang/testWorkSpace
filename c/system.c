#include<stdlib.h>
int main(){
	system("ffmpeg -y -i rtsp://192.168.2.20:554/mpeg4cif -ss 00:00:01 -vframes 1 -f image2 -vcodec png image.png");
	return 0;
}