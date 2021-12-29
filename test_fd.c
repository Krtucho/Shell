// #include <wait.h>
#include <unistd.h>
#include <sys/wait.h>
#include<sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include<signal.h>

int out_overwrite(char * file){
    int fd = open(file, O_WRONLY | O_CREAT);

    if(fd == -1)
        return -1;

    if(file == NULL)
        fd = STDOUT_FILENO;

    int dup = dup2(fd, STDOUT_FILENO);
    if(dup == -1)
        return -1;

    close(fd);
    return 0;
}

int input_read(char * file){
    int fd = open(file, O_RDONLY | O_CREAT);

    if(fd==-1)
        return -1;

    int dup = dup2(fd, STDIN_FILENO);
    if(dup == -1)
        return -1;

    close(fd);
    return 0;
}

int out_append(char * file){
    int fd=open(file, O_APPEND | O_CREAT);

    if(fd == -1)
        return -1;

    // char a = getchar();
    int dup = dup2(fd, STDOUT_FILENO);
    // FILE * fp;
    // fp = freopen("a.txt","w", stdin);
    // fclose(fp);
    // char string[80];
    // File * fp;
    // scanf(string);
   
    // fgets(string, 10, string);
    if(dup == -1)
        return -1;

    close(fd);
    return 0;
}

int main(int argc, char const *argv[])
{
    
    char * testing = strdup("a.txt");
    int out = input_read(testing);
    // out_overwrite(strdup("d.txt"));

    char buf[1000];
    int num = read(STDIN_FILENO, buf, sizeof(buf));
    // printf("%d\n",num);
    // printf("%d ABCDE", out);

    out_append("d.txt");
    printf("%s\n", strndup(buf,num));


    return 0;
}
