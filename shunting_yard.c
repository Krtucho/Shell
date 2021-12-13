#include "shunting_yard.h"
#include "list.h"
#include<stdbool.h>// Booleanos

bool IsCommand(node* current){

}

bool IsOperator(node* current){

}

void sy(){
    // list * l = init_list("");
    // printf("%d \n", l->size);
    // print_list(l);
    // free_list(l);
    // pop_front(l);
    // printf("%d \n", l->size);
    // print_list(l);

    list * tokens = init_list("token");
    push_back(tokens, "a");
    push_back(tokens, "b");
    print_list(tokens);


    list * exit_queue = init_list("a");
    pop_front(exit_queue);
    list * op_stack = init_list("a");
    pop_front(op_stack);

    int count = 0;
    node * current;
    current = tokens->head;
    while(count < tokens->size){
        if(!IsOperator(current)){
            push_front(exit_queue, current);
        }
        else{
            if(op_stack->size != 0 && EPrioriy()){
                if()
            }
            else{

            }
        }

        current = current->next;

    }

    free_list(tokens);

}