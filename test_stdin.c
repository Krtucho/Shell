#include<stdio.h>
#include <unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char const *argv[])
{
    // char * console_cmd="hello";
    // char * tty=ttyname(STDIN_FILENO);
    // int fd=open(tty.c_str(),O_WRONLY);
    freopen("a.txt","r", stdin);

    char *myargs[3];
    myargs[2] = NULL;
    myargs[0] = strdup("cat");
    myargs[1] = strdup("algo.txt");


    execvp(myargs[0], myargs);
    fclose(stdin);

    return 0;
}
