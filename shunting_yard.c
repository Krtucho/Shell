#include "shunting_yard.h"
#include "list.h"

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


    free_list(tokens);

    while(tokens->size>0){
        if()
    }
}