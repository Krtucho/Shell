#include "shunting_yard.h"
#include "list.h"
#include<stdbool.h>// Booleanos

bool IsCommand(node* current){

}

bool IsOperator(node* current){

}

int Priority(char * op){
    return 0;
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
            InsertCommandsArgs();
            // push_front(exit_queue, current);
        }
        else{
            if(IsIf(current)){//If
            } 
            else if(IsBoolean(current)){// && ||

            }
            else if(IsThenElse(current)){//then else-

            }

            else if(op_stack->size != 0 && Priority(current) - Priority(op_stack->head->value) <= 0){
                node * temp = pop_back(op_stack);
                push_front(exit_queue, temp);
                free(temp);
                push_back(op_stack, current);
            }
            else{
                push_back(op_stack, current);
            }
        }

        current = current->next;

    }

    free_list(tokens);

}