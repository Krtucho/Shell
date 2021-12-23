#include<unistd.h>

int main(int argc, char const *argv[])
{
    int fd[2];
    pipe(fd);
    fork();

    dup2(stdout, filestdin)
    printf()
    exe
    return 0;
}
