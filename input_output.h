#include <unistd.h>
#include <sys/wait.h>
#include<sys/stat.h>
#include <search.h>
#include <fcntl.h>



int only_write(char * file);
int only_append(char * file, char buf[1000], int num);
int input_read(char * file);