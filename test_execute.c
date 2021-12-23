#include "execute.h"

int main(int argc, char const *argv[])
{
    

    Expression * c = (Expression*)malloc(sizeof(Expression));
    c->name=strdup("ls"); 
    c->operators = SIMPLE_EXPRESSION;

    // Expression * a = (Expression*)malloc(sizeof(Expression));
    // a->name = strdup("algo.txt");
    // a->operators = LEAF;

    Expression * b = (Expression*)malloc(sizeof(Expression));
    b->name = strdup("-i");
    b->operators = ARGS;

    // Expression * d = (Expression*)malloc(sizeof(Expression));
    // d->name = strdup("a.txt");
    // d->operators = ARCHIVE;

    Expression * e = (Expression*)malloc(sizeof(Expression));
    e->name = strdup("|");
    e->operators = PIPE;

    Expression * f = (Expression*)malloc(sizeof(Expression));
    f->name = strdup("grep");
    f->operators = SIMPLE_EXPRESSION;

    Expression * g = (Expression*)malloc(sizeof(Expression));
    g->name = strdup("u");
    g->operators = ARGS;


    //Testing ls -i | grep u | wc
    Expression * h = (Expression*)malloc(sizeof(Expression));
    h->name = strdup("|");
    h->operators = PIPE;

    Expression * i = (Expression*)malloc(sizeof(Expression));
    i->name = strdup("wc");
    i->operators = SIMPLE_EXPRESSION;
    list * l = init_list(c);

    // push_back(l, c);
    // push_back(l, a);
    push_back(l, b);
    push_back(l, e);
    push_back(l, f);
    

    push_back(l, g);
    push_back(l, h);
    push_back(l, i);
// push_back(l, d);
    // &&
    

    Expression * n = l->head->value;//getAt(l, 0)->value;
    Expression * n_one = l->tail->value;//getAt(l, 1)->value;
    printf("Valor de c: %s\n",n->name);
    printf("Valor de a: %s\n", n_one->name);
    // printf(list)

    node * temp = Execute(l->head, l->tail);

    // free(a);
    free(c);
    free(b);
    free(e);
    free(f);
    free(g);
    free(h);
    free(i);
    free(temp);
    free(n);
    free(n_one);
    free_list(l);

    return 0;
}
