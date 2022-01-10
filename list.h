#include <stdio.h>
#include <stdlib.h>
#include <string.h> 



#define __ReservedMemoryforNode (node*)malloc(sizeof(node));
#define __SafeMemoryReserved(x) if(x == NULL) exit(1);
#define __SafeMemoryFree(x) if(x!=NULL) free(x);

typedef struct node
{
    void * value;
    struct node * previous;
    struct node * next;
} node;

typedef struct list
{
    node* head;
    node* tail;
    unsigned int size;
} list;


list* init_list(void* value);
void free_list(list*);
void push_back(list*, void*);
void push_front(list*, void*);
node* pop_back(list*);
node* pop_front(list*);
node* getAt(list*, int);
void insert(list*, void*,int);
void removeNode(list*, node*);
void print_list(list*);
