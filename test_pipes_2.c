#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<errno.h>
// #include<

#define READ_END 0
#define WRITE_END 1

int main(int argc, char const *argv[])
{
    int fd1[2], fd2[2];
    int status, pid;

    pipe(fd1);

    pid=fork();

    if(pid==0){
        close(fd1[READ_END]);
        
        dup2(fd1[WRITE_END],STDOUT_FILENO);
        close(fd1[READ_END]);

        execlp("/bin/grep", "grep", "u", NULL);
    }
    else{
        close(fd1[READ_END]);
        pipe(fd2);
        pid=fork();
        if(pid==0){
            close(fd2[READ_END]);

            dup2(fd1[READ_END], STDIN_FILENO)
        }
    }
    return 0;
}
