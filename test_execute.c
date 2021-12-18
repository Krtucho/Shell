#include "execute.h"

int main(int argc, char const *argv[])
{
    

    Command * c = (Command*)malloc(sizeof(Command));
    c->name="echo";
    c->operators = SIMPLE_COMMAND;

    Command * a = (Command*)malloc(sizeof(Command));
    c->name = "a";
    c->operators = LEAF;

    list * l = init_list(c);

    push_back(l, c);
    push_back(l, a);

    Execute(l->head, l->tail);

    return 0;
}
