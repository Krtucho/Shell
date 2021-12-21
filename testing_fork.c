#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    pid_t a = fork();
    printf("%d",a);
    return 0;
}
