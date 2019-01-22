#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

static pid_t process_make(void(*func)()) {
    pid_t pid;
    pid = fork();
    if (pid > 0) {
        return pid;
    }

    
    func();
    return -1;
}


void child_process()
{
    int i=0;
    while(1)
    {
        printf("child_process[%d]\n",i);
        i++;
        sleep(1);
    }
}
    


int main()
{
    process_make(child_process);
}
