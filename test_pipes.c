#include<unistd.h>
// #include "execute.h"
// #include "using_execvp.h"
#include <unistd.h>
#include <sys/wait.h>
#include<sys/stat.h>
#include <stdio.h>
#include <string.h>
#include<stdlib.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char const *argv[])
{
    char *myargs[2];
    myargs[1] = NULL;
    myargs[0] = strdup("ls");

    char *myargsaaa[3];
    myargsaaa[2] = NULL;
    myargsaaa[0] = strdup("grep");
    myargsaaa[1] = strdup("u");

    int fd[2];
    pid_t pidC;
    char buf[10];
    int num;

   

    pipe(fd);
    pidC = fork();
    if(pidC==0){
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        execvp(myargs[0], myargs);
        // exit(0);
        // break;
    }
    else if(pidC==-1){
        // break;
        printf("Error\n");
    }
    else{
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        num = read(fd[0], buf, sizeof(buf));
        printf("%d\n",num);
        close(fd[0]);
        execvp(myargsaaa[0], myargsaaa);
        // break;
    }
    // int fd[2];
    // pipe(fd);
    // fork();

    // dup2(stdout, filestdin)
    // printf()
    // exe
    

    // char *myargsaaa[3];
    // myargsaaa[2] = NULL;
    // myargsaaa[0] = strdup("grep");
    // myargsaaa[1] = strdup("u");

    // int fd1[2];
    // int status,pid;

    // pipe(fd1);
    // pid=fork();
    // if(pid==0)
    // {
    //     dup2(fd1[READ_END],STDIN_FILENO);
    //     close(fd1[READ_END]);

    //     dup2(fd1[WRITE_END],STDOUT_FILENO);
    //     close(fd1[WRITE_END]);
    //     execvp(myargs[0], myargs);// return Execute(first_cmd,PIPE_node->previous);
    //     printf("child 1\n");
    // }
    // else
    // {
    //     pid=fork();
    //     if(pid==0)
    //     {
    //         close(fd1[WRITE_END]);
    //         dup2(fd1[READ_END],STDIN_FILENO);
    //         close(fd1[READ_END]);
    //         execvp(myargsaaa[0], myargsaaa);
    //         printf("child 2\n");
    //     }
    //     wait(NULL);
    // }
    // wait(NULL);
    // printf("found a Pipe...\n");

    return 0;
}
