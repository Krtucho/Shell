#include <unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

int main(int argc, char const *argv[])
{
    int fd;
    int refd;
    char *buf="Hola belsai y carlitos, lauren y daniela";
    fd=open("test.txt", O_RDWR|O_CREAT,0644);
    if(fd==-1)
    {
        printf("open file error");
        exit(-1);
    }
    printf("stdout: %d \n", fileno(stdout));
    printf("stdin: %d \n", fileno(stdin));
    refd=dup2(fd,fileno(stdout));
    if(refd==-1)
    {
        printf("error");
        exit(-1);
    }
    printf("Valor de retorno de dup2:%d\n", refd);
    write(fileno(stdout),buf,strlen(buf));
    close(fd);
    exit(0);
    return 0;
}
