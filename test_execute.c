#include "execute.h"

int main(int argc, char const *argv[])
{
    

    Expression * c = (Expression*)malloc(sizeof(Expression));
    c->name="echo";
    c->operators = SIMPLE_EXPRESSION;

    Expression * a = (Expression*)malloc(sizeof(Expression));
    c->name = "a";
    c->operators = LEAF;

    list * l = init_list(c);

    push_back(l, c);
    push_back(l, a);

    Execute(l->head, l->tail);

    return 0;
}
