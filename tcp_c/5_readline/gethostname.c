#include<stdio.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ERR_EXIT(m)\
do{\
    perror(m);\
    exit(EXIT_FAILURE);\  
}while(0);

int main()
{
        char hostname[256];
        gethostname(hostname,sizeof(hostname));
        printf("%s\n",hostname);
        
        
        struct hostent *hp;
        hp = gethostbyname(hostname);
        if(hp == NULL)
        {
                perror("gethostbyname");
        }
        
        int i=0;
        while(hp->h_addr_list[i]!=NULL){
                printf("hostname:%s\n",hp->h_name);
                printf("%d ip:%s\n",i,inet_ntoa(*(struct in_addr*)hp->h_addr_list[i]));
                i++;
        }
        
        return 0;
}
