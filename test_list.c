#include "list.h"


int main(int argc, char const *argv[])
{

    // char* a = 'a';
    list* l = init_list("init");
    push_back(l, strdup("a"));
    print_list(l);


    // free(a);
    /* code */
    return 0;
}


// list* init_list(void* v)
// {
//     list* l = (list*)malloc(sizeof(list));
//     __SafeMemoryReserved(l)
//     node *first = __ReservedMemoryforNode
//     __SafeMemoryReserved(first)
//     first->previous = NULL;
//     first->next = NULL;
//     first->value = v;
//     l->head = first;
//     l->tail = first;
//     l->size = 1;
//     return l;
// }