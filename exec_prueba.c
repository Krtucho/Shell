#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdbool.h> // Booleanos
#include <stdio.h>
#include <stdlib.h>


int main()
{
    char *myargs[1]={"aaaa"};
    pid_t rc=fork();
    int e;
    if(rc==0)
    {
        e=execvp(myargs[0],myargs);
        if(e==-1)
        {
            printf("%s: command not found","holaaa");
            //return;
        }
    }
    else
    {
        // if(e!=-1)
        // {
        int *status = 0;
        pid_t p = waitpid(rc, status, 0);
        kill(rc, SIGKILL);
        //}
    }
    
    printf("%d ",e);
}