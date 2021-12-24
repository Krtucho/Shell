#include "execute.h"

int main(int argc, char const *argv[])
{

    Expression * c = (Expression*)malloc(sizeof(Expression));
    c->name=strdup("ls"); 
    c->operators = SIMPLE_EXPRESSION;
    c->std_in=NULL;

    Expression * a = (Expression*)malloc(sizeof(Expression));
    a->name = strdup("algo.txt");
    a->operators = ARGS;

    Expression * b = (Expression*)malloc(sizeof(Expression));
    b->name = strdup("-i");
    b->operators = ARGS;

    // Expression * d = (Expression*)malloc(sizeof(Expression));
    // d->name = strdup("a.txt");
    // d->operators = ARCHIVE;

    Expression * e = (Expression*)malloc(sizeof(Expression));
    e->name = strdup("&&");
    e->operators = AND;

    Expression * f = (Expression*)malloc(sizeof(Expression));
    f->name = strdup("echo");
    f->operators = SIMPLE_EXPRESSION;

    Expression * g = (Expression*)malloc(sizeof(Expression));
    g->name = strdup("u");
    g->operators = ARGS;


    // Testing ls -i | grep u | wc
    Expression * h = (Expression*)malloc(sizeof(Expression));
    h->name = strdup("|");
    h->operators = PIPE;

    Expression * i = (Expression*)malloc(sizeof(Expression));
    i->name = strdup("cat");
    i->operators = SIMPLE_EXPRESSION;
    list * l = init_list(c);

    Expression * j = (Expression*)malloc(sizeof(Expression));
    j->name = strdup("|");
    j->operators = PIPE;

    Expression * k = (Expression*)malloc(sizeof(Expression));
    k->name = strdup("exit");
    k->operators = SIMPLE_EXPRESSION;


   // push_back(l, c);
    // push_back(l, a);
    push_back(l, b);
    push_back(l, e);
    push_back(l, f);
    

    push_back(l, g);
    // push_back(l, h);
    // push_back(l, i);
    // push_back(l, j);
    // push_back(l, k);
// push_back(l, d);
    // &&
    

    Expression * n = l->head->value;//getAt(l, 0)->value;
    Expression * n_one = l->tail->value;//getAt(l, 1)->value;
    printf("Valor de c: %s\n",n->name);
    printf("Valor de a: %s\n", n_one->name);
    // printf(list)

    int temp = Execute(l->head, l->tail);
    printf("%d ", temp);
    printf("Llegue sl final, ya tengo hambre, Carlos te voy a golpear!!!!!\n");

    // free(a);
    // free(c);
    // free(b);
    // free(e);
    // free(f);
    // free(g);
    // free(h);
    // free(i);
    // free(temp);
    // free(n);
    // free(n_one);
    free_list(l);

    return 0;
}
