#include "input_output.h"
int only_write(char * file){ // Metodo para escribir el contenido que salga del stdout a un archivo
    int fd = open(file, O_WRONLY | O_CREAT);

    if(fd == -1)
        return -1;

    if(file == NULL)
        fd = STDOUT_FILENO;

    int tempdup = dup2(fd, STDOUT_FILENO);
    if(tempdup == -1)
        return -1;

    close(fd);
    return 0;
}
int only_append(char * file, char buf[1000], int num){ // Metodo para concatenar el contenido que esta guardado en buf hacia un archivo, num sera la cantidad de caracteres que se van a escribir

    int fd=open(file, O_WRONLY | O_APPEND | O_CREAT , 0);

    if(fd == -1)
        return -1;

    if(file == NULL)
        fd = STDOUT_FILENO;

    write(fd, buf, num);

    close(fd);
    return 0;
}
int input_read(char * file){ // Metodo para leer el contenido de un archivo y enviarlo al stdin
    int fd = open(file, O_RDONLY | O_CREAT);

    if(fd==-1)
        return -1;

    int tempdup = dup2(fd, STDIN_FILENO);
    if(tempdup == -1)
        return -1;

    close(fd);
    return 0;
}