// #include <stdio.h>
// #include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
// #include <string.h>
#include "using_execvp.h"

void Run(node * com){
    Command * com_to_exec = com->value;
    if(com_to_exec->operators != SIMPLE_COMMAND)
        return;
        
    // list * temp_args = init_list(com->next);

    node * last = com->next;
    Command * last_com = last->value;
    int count = 0;
    while(last_com->operators == LEAF){// Con leaf me refiero a los argumentos lo q aun no se bien como ponerle
        count++;
        last = last->next;
    }

    char *myargs[count+2];
    myargs[count+1] = NULL;
    myargs[0] = strdup(com_to_exec->name);

    int temp = 1; // Para ir ubicando argumenta a argumento en el arreglo
    node * current = com->next;
    while (temp < count+1)
    {
        Command * current_com = current->value;
        myargs[temp] = strdup(current_com->name);
        temp++;
        current=current->next;
    }
    
    execvp(myargs[0], myargs);
}