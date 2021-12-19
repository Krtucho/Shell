#include "execute.h"

int main(int argc, char const *argv[])
{
    

    Expression * c = (Expression*)malloc(sizeof(Expression));
    c->name=strdup("echo");
    c->operators = SIMPLE_EXPRESSION;

    Expression * a = (Expression*)malloc(sizeof(Expression));
    a->name = strdup("a");
    a->operators = LEAF;

    list * l = init_list(c);

    // push_back(l, c);
    push_back(l, a);


    Expression * n = l->head->value;//getAt(l, 0)->value;
    Expression * n_one = l->tail->value;//getAt(l, 1)->value;
    printf("Valor de c: %s\n",n->name);
    printf("Valor de a: %s\n", n_one->name);
    // printf(list)

    node * temp = Execute(l->head, l->tail);

    free(a);
    free(c);
    free(temp);
    free(n);
    free(n_one);
    free_list(l);

    return 0;
}
