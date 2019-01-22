#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "my_recv.h"

#define SERV_PORT 6666  //port
#define LISTENQ 12  //the max request deque

#define VALID_USERINFO "y"  //the flag of valid user
#define INVALID_USERINFO "n"    //the flag of invalid user

#define USERNAME 0  //user name
#define PASSWORD 1  //password

//userinfo
typedef struct userinfo {
    char username[32];
    char password[32];
}userInfo;

//users
userInfo users[] = {
    {"zhangsan", "123456"},
    {"linux", "unix"}
};

/**
 * Function:find_name
 * Description:find the num of the name in the list of users
 * Input: user name
 * Output:the num of the user name
 */
int find_name(const char *name) {
    int i = 0;

    if (NULL == name) {
        printf("name is NULL");
        return -2;
    }
    while (users[i].username[0] != ' ') {
        if (strcmp(users[i].username, name) == 0) {
            return i;
        }
        ++i;
    }
    return -1;
}
//send data to client
void send_data(int conn_fd, const char *string) {
    if (send(conn_fd, string, strlen(string), 0) < 0) {
        my_err("send", __LINE__);
    }
}

int main() 
{

    int sock_fd, conn_fd;   //the descriptor of socket
    int optval; //option value
    int flag_recv = USERNAME;   //the flag of name or password
    int ret;    //
    int name_num;   //the num of user
    pid_t pid;  //the id of the process
    socklen_t cli_len;  
    struct sockaddr_in cli_addr, serv_addr;
    char recv_buf[128]; //the buffer of the receive data

/**
 * int socket(int domain, int type, int protocol);
 * Input:
 *      domain: this selects the protocol family
 *      type: the type of socket
 *      0:the protocol is decided by the first and the second argument
*Output::On success, a file descriptor for the new socket,
         On error, -1 is returned, and errno is set appropriately.
*/
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);  //create a new socket 
    if (sock_fd < 0) {
        my_err("socket", __LINE__);
    }
    optval = 1;
    //set the socket so that we can use it again
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int) ) < 0) {
        my_err("setsockopt", __LINE__);
    }

    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind the socket to the local port
    if (bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0) {
        my_err("bind", __LINE__);
    }
    //set the socket to listen status
    if (listen(sock_fd, LISTENQ) < 0) {
        my_err("listen", __LINE__);
    }

    cli_len = sizeof(struct sockaddr_in);

    while (1) {
        //wait a request
        conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
        if (conn_fd < 0) {
            my_err("accept", __LINE__);
        }

        printf("accept a new client, ip is %s\n",inet_ntoa(cli_addr.sin_addr));

        if ((pid =fork()) == 0) {
            while (1) {
                if ((ret = recv(conn_fd, recv_buf, sizeof(recv_buf), 0)) < 0) {
                    perror("recv");
                }
                //the flag of end
                recv_buf[ret - 1] = '\0';
                if (flag_recv == USERNAME) {
                    name_num = find_name(recv_buf);
                    switch(name_num) {
                        case -1:
                            send_data(conn_fd, "n\n");
                            break;
                        case -2: 
                            exit(1);
                            break;
                        default:
                            send_data(conn_fd, "y\n");
                            flag_recv = PASSWORD;
                    }
                }
                else if (flag_recv == PASSWORD) {
                    if (strcmp(users[name_num].password, recv_buf) == 0) {
                        //send data to client
                        send_data(conn_fd, "y\n");
                        send_data(conn_fd, "Welcome to my server\n");
                        printf("%s login\n", users[name_num].username);
                    }
                    else {
                        send_data(conn_fd, "n\n");
                    }
                }
            }
            close(sock_fd);
            close(conn_fd);
            exit(0);
        }
        else {
            close(conn_fd);
        }
    }

    return 0;
}