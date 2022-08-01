#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    int ftoc[2];
    int ctof[2];
    char buf[10];

    if(pipe(ftoc) < 0 || pipe(ctof) < 0)
    {
        printf("fail");
        exit(-1);
    }
    int pid = fork();
    if(pid == 0) //child
    {
        read(ftoc[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);
        write(ctof[1], "pong", strlen("pong"));
    }
    else //father
    {
        write(ftoc[1], "ping", strlen("ping"));
        read(ctof[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);
    }

    exit(0);
}
