#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>

int main()
{
    int socketfd = socket(AF_INET,SOCK_DGRAM,0);
    if(socketfd == -1)
    {
        printf("create socketfd failed\n");
    }
    printf("socektfd:%d\n",socektfd);

    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port=5818;


    int bind = bind(socektfd,)

}
