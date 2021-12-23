// #include "execute.h"
// #include "using_execvp.h"
#include <unistd.h>
#include <sys/wait.h>
#include<sys/stat.h>
#include<stdbool.h>// Booleanos
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "bin_tree.h"
// #include "list.h"
// #include<stdbool.h>// Booleanos

#pragma region List
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

list* init_list(void* v)
{
    list* l = (list*)malloc(sizeof(list));
    __SafeMemoryReserved(l)
    node *first = __ReservedMemoryforNode
    __SafeMemoryReserved(first)
    first->previous = NULL;
    first->next = NULL;
    first->value = v;
    l->head = first;
    l->tail = first;
    l->size = 1;
    return l;
}
void push_back(list* l, void* v)
{
    node* insert = __ReservedMemoryforNode
    __SafeMemoryReserved(insert)
    if(l->size==0)
    {
        insert->previous = NULL;
        insert->next = NULL;
        insert->value = v;
        l->head = insert;
        l->tail = insert;
        l->size = 1;
        return;
    }
    insert->value = v;
    insert->previous = l->tail;
    insert->next = NULL;
    l->tail->next = insert;
    l->tail = insert;
    l->size++;
}
void push_front(list* l, void* v)
{
    node* insert = __ReservedMemoryforNode
    __SafeMemoryReserved(insert)
    if(l->size==0)
    {
        insert->previous = NULL;
        insert->next = NULL;
        insert->value = v;
        l->head = insert;
        l->tail = insert;
        l->size = 1;
        return;
    }
    insert->value = v;
    insert->next = l->head;
    insert->previous = NULL;
    l->head->previous = insert;
    l->head = insert;
    l->size++; 
}
node* pop_back(list* l)
{
    if(l->size == 0) return NULL;
    node* last = l->tail;
    l->tail = l->tail->previous;
    if(l->tail != NULL)
    {
        l->tail->next = NULL;
    }
    l->size--;
    return last;
}
node* pop_front(list* l)
{
    if(l->size == 0) return NULL;
    node* first = l->head;
    l->head = l->head->next;
    if(l->head != NULL)
    {
        l->head->previous = NULL;
    }
    l->size--;
    return first;
}
node* getAt(list* l, int ind)
{
    if(ind<0 || ind>=l->size) 
    {
        printf("Index out of range."); 
        exit(1);
    }
    node* current = l->head;
    for (size_t i = 0; i < ind; i++, current = current->next);
    return current;   
}
void insert(list* l, void* v, int ind)
{
    if(ind<0 || ind>=l->size) 
    {
        printf("Index out of range."); 
        return;
    }
    if(ind==0) 
    {
        push_front(l, v); 
        return;
    }
    if(ind==l->size-1)
    {
        push_back(l,v); 
        return;
    }
    node* insert = __ReservedMemoryforNode
    __SafeMemoryReserved(insert)
    insert->value = v;
    node* lnode = l->head;
    node* rnode = l->head->next;
    for(size_t i = 1; i < ind; i++)
    {
        lnode = rnode;
        rnode = rnode->next;
    }
    lnode->next = insert;
    insert->previous = lnode;
    insert->next = rnode;
    rnode->previous = insert;
    l->size++;
}
void removeNode(list* l, node* item)
{
    if(l->size == 0) return;
    if(item->previous == NULL) pop_front(l);
    else if (item->next == NULL) pop_back(l);
    else
    {
        node* prev = item->previous;
        node* next = item->next; 
        prev->next = next;
        next->previous = prev;
        l->size--;   
    }
}
void free_list(list* l)
{
    node* temp = l->head->next;
    node* current = l->head;
    while(current!=NULL)
    {
        free(current);
        current = temp;
        if(temp != NULL)
        {
            temp = temp->next;
        }
    }
    free(l);
}

void print_list(list * t) {
    node * current = t->head;
    printf("Imprimiendo lista...\n");
    while (current != NULL) {
        printf("%s ",(char*) current->value);
        current = current->next;
    }
    printf("\n");
}
#pragma endregion

#pragma region Expression
enum OPERATORS{
    AND = 0,
    OR = 13,
    PIPE = 2,
    IF = 3,
    REDIRBIG=4,
    THEN=5,
    ELSE=6,
    END=7,
    Com=8,
    ARGS=9,
    IF_ELSE=10,
    REDIRLESS=11,
    ARCHIVE=12,
    DOUBLEREDIRBIG=14,
    SIMPLE_EXPRESSION=1
}OPERATORS;

typedef struct Expression// TODO: Cambiar nombre a: Expression
{
    char * name;
    list * args;
    char * std_in;
    char * std_out;
    bool calculated;
    enum OPERATORS operators;
}Expression;

#pragma endregion

void Run(node * com){
    printf("Inside Run Method...\n");
    Expression * com_to_exec = com->value;
    if(com_to_exec->operators != SIMPLE_EXPRESSION)
        return;
        
    // list * temp_args = init_list(com->next);

    node * last = com->next;
    Expression * last_com;

    if(last != NULL)
        last_com = last->value;
    else
        last_com = NULL;

    int count = 0;
    while(last_com != NULL && (last_com->operators == ARGS || last_com->operators== ARCHIVE)){// Con ARGS me refiero a los argumentos lo q aun no se bien como ponerle
        count++;
        last = last->next;
        if(last != NULL)
            last_com = last->value;
        else
            last_com = NULL;
    }
    printf("Out of first while...\n");

    char *myargs[count+2];
    myargs[count+1] = NULL;
    myargs[0] = strdup(com_to_exec->name);

    int temp = 1; // Para ir ubicando argumenta a argumento en el arreglo
    node * current = com->next;
    while (temp < count+1)
    {
        Expression * current_com = current->value;
        myargs[temp] = strdup(current_com->name);
        temp++;
        current=current->next;
    }

    // printf("%s \n", com_to_exec->std_in);
    // int fd[2];
    //int status,pid;

    // pipe(f)

    // if(count>0)
    // {
    //     //metodo para buscar un argumento distinto de -
    // }
    // else
    // {
    //     if(com_to_exec->std_in!=NULL)
    //     {
    //         char * myargs_temp[3];
    //         myargs_temp[0]=com_to_exec->name;
    //         myargs_temp[1]=com_to_exec->std_in;
    //         myargs_temp[2]=NULL;
    //         int e = execvp(myargs_temp[0], myargs_temp);

    //     }
    //     else
    //     {
    //         char * myargs_temp[2];
    //         myargs_temp[0]=com_to_exec->name;
    //         myargs_temp[1]=NULL;
    //         int e = execvp(myargs_temp[0], myargs_temp);
    //     }
    // }
    
    // char * temp_out = com_to_exec->std_in;

    // printf("Executing command...\n");
    // printf("%s\n", myargs[0]);

    int rc = fork();
    if (rc < 0) { // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) { // child (new process)
            // printf("com_to_exec->std_in != NULL..\n");
        // if(strcmp(com_to_exec->name, "aa")){
        //     freopen ("1.txt", "r", stdin);
        //     fclose(stdin);    
        // }
        
        if(com_to_exec->std_in != NULL){
            // printf("com_to_exec->std_in != NULL..\n");

            freopen(com_to_exec->std_in,"r", stdin);
        }
        // char buf[10];
        // int num = read(STDIN_FILENO, buf, sizeof(buf));
        // printf("%d\n",num);
        int e = execvp(myargs[0], myargs);

        if(com_to_exec->std_in != NULL){
            fclose(stdin);
        }

        // printf("I'm in child process...\n");
    }
    else { // parent goes down this path (main)
        int wc = wait(NULL);
    }
    // printf("%d", e);
}

void SIMPLE_Expression_CODE(node * com){
    // int rc = fork();
    // if (rc < 0) { // fork failed; exit
    //     fprintf(stderr, "fork failed\n");
    //     exit(1);
    // } else if (rc == 0) { // child (new process)
    //     printf("I'm in child process...\n");
        Run(com);
    // } else { // parent goes down this path (main)
    //     int wc = wait(NULL);
    // }
}

void AND_CODE(node * com){

}

void (*testing[])(node*) = {
        AND_CODE,
        SIMPLE_Expression_CODE,
        0
	};

node * Search_AND_OR(node * first_cmd, node * last_cmd){

    bool is_if = false;
    node * current = first_cmd;
    Expression * current_temp = current->value;
    while(current != last_cmd){
        current_temp = current->value;
        if(current_temp->operators == IF){
            is_if = true;
            continue;
            }
        if(current_temp->operators == END){
            is_if = false;
            continue;
            }
        if((current_temp->operators == AND || current_temp->operators == OR) && !is_if){
            // free(current_temp);
            return current;
        }

        current = current->next;
    }

    if(current == NULL)
        printf("current is NULL");
    if(current_temp == NULL)
        printf("current_temp is NULL");
    // free(current_temp);
    // free(current);
    return NULL;
}

node * Search_PIPE(node * first_cmd, node * last_cmd){
    printf("In Search PIPE method...\n");
    bool is_if = false;
    node * current = first_cmd;
    Expression * current_temp;// = current->value;
    while(current != last_cmd){
        printf("In while searching pipes...\n");
        current_temp = current->value;
        // current_temp->operators
        if(current_temp->operators == IF){
            is_if = true;
            continue;
            }
        if(current_temp->operators == END){
            is_if = false;
            continue;
            }
        if(current_temp->operators == PIPE && !is_if){
            // free(current_temp);
            return current;
        }

        printf("Changing current variable...\n");
        current = current->next;
        if(current == NULL)
            printf("current is NULL");
        printf("current variable changed...\n");
        // Expression * temp = current->value;
        // printf("current->value: %s", temp->name);
        // free(temp);
    }
    // if(current_temp != NULL)
        // free(current_temp);
    // if(current != NULL)
        // free(current);
    return NULL;
}

node * Search_IF_THEN_ELSE(node * first_cmd, node * last_cmd, enum OPERATORS operator){
    printf("I'm in IF THEN ELSE method...\n");
    node * current = first_cmd;
    Expression * current_temp = current->value;
    while(current != last_cmd){
        current_temp = current->value;
        if(current_temp->operators == operator){
            return current;
        }
        current = current->next;
    }
    // free(current);
    // free(current_temp);
    printf("End of Search IF_THEN_ELSE method...\n");
    return NULL;
}

// Leaves
int GetIndex(Expression * op){
    return (int)op->operators;
}

void ExecuteExpression(node * cmd_to_exec){
    Expression * node_com = cmd_to_exec->value;
    // enum OPERATORS op = node_com->operators;
    printf("Getting index_method...\n");
    // printf("%d", node_com->operators);
    int index = node_com->operators;
    printf("Index: %d \n", index);
    testing[index](cmd_to_exec);
    // free(node_com);
}


node * SolveExpressions(node * first_cmd, node * last_cmd){
    node * current = first_cmd;
    // while (current != NULL && &current != &last_cmd)
    // {
        printf("Start ExecuteExpression method\n");

        ExecuteExpression(current);
        printf("Command Executed...\n");
        // current = current->next;
    // }
    return current;
}


node * SolveBiggerRedir(node * first_cmd, node * last_cmd){
    Expression * output;
    node * current=first_cmd;
    FILE *fp;

    Expression * current_exp;
    while (current!=NULL)
    {
        current_exp=current->value;
        if(current_exp->operators==REDIRBIG)
        {
            Expression *current_next=current->next->value;
            fp = freopen(current_next->name, "w", stdout);
            fclose(fp);
            output=current_next;

            continue;

        }
        if(current_exp->operators==DOUBLEREDIRBIG)
        {
            Expression *current_next=current->next->value;
            fp = freopen(current_next->name, "a", stdout);
            fclose(fp);
            output=current_next;
            continue;

        }
        current=current->next;
    }
    if(output!=NULL){
    
        fp=freopen(output->name,"a",stdout);
        Expression * exp=first_cmd->value;
        printf("%s \n", exp->std_out);
        fclose(fp);

        exp->std_out="1";
    }
   return first_cmd;
    
    // while ()
    // {
    //     /* code */
    // }
    


    // node * current;
    // node * BIGGER_REDIR = SearchBiggerRedir(last_cmd);

    // printf("")
    // if(BIGGER_REDIR == NULL)
    //     return first_cmd;

    // current = BIGGER_REDIR->previous;

    // Expression * prev_temp = current->previous->value;
    // Expression * current_temp = current->value;


    // while(current_temp->operators==REDIRBIG ||  // >
    //       current_temp->operators==REDIRLESS || // <
    //       (current_temp->operators==ARCHIVE && //  < archivo o....... > archivo
    //        (prev_temp->operators==REDIRLESS || prev_temp->operators==REDIRBIG)
    //       )|| // < comando o ............. > comando 
    //       (current_temp->operators==SIMPLE_EXPRESSION && //  < comando o....... > comando
    //        (prev_temp->operators==REDIRLESS || prev_temp->operators==REDIRBIG)
    //       )
    //       ){
    //     // Expression * prev_temp = current->previous->value;
    //     // Expression * current_temp = current->value;
    //     // current->previous->value 
    //     prev_temp->std_in = current_temp->std_out;
    //     current = current->previous;

    //     if(current == NULL){
    //         return NULL;
    //     }

    //     prev_temp = current->previous->value;
    //     current_temp = current->value;
    // }

    //return first_cmd;
}

node * SearchBiggerRedir(node * last_cmd){
    node * current = last_cmd;
    Expression * current_temp = current->value;

    while(current != NULL && &current != &last_cmd){
        if(current_temp->operators == REDIRBIG){
            return current;
        }

        current = current->next;
    }
    return NULL;
}

node * SolveLessRedir(node * first_cmd, node * last_cmd){
    node * current = last_cmd;
    node * input=NULL;
    //FILE * fp;
    Expression * current_exp;

    printf("Llegamos a < wiiiiiii!!!!\n");
    while (current != NULL)
    {
        current_exp = current->value;
        if(current_exp->operators == REDIRLESS){
            printf("Inside current REDIRLESS if...\n");
            printf("current_exp.name = %s", current_exp->operators);
            input=current->next;
            break;
        }
        current = current->previous;
    }
    if(current_exp != NULL)
        printf("%s\n", current_exp->name);
    else
        printf("current_exp = NULL");
    
    if(input!=NULL)
    {
        printf("Input was not equal NULL...\n");
        Expression * input_exp=input->value;
        printf("%s \n", input_exp->name);
        // fp=fopen(input_exp->name,"r");
        // struct stat sb;
        Expression * fisrt_exp=first_cmd->value;
        // if(stat(input_exp->name,&sb)==-1)
        // {
        //     fisrt_exp->std_out=0;
        //     return first_cmd;
        // }
        // char * file_contents=malloc(sb.st_size);
        //fread(file_contents,sb.st_size,1,fp);
        fisrt_exp->std_in=input_exp->name;
        // printf("%s", file_contents);
        // fclose(fp);
        printf("%s \n", fisrt_exp->std_in);
    }
    printf("End of Less Redir...\n");
    return first_cmd;
}


node * Solve_Leaves(node * first_cmd, node * last_cmd){
    SolveLessRedir(first_cmd, last_cmd);
    printf("Starting to solve Expressions...\n");
    SolveExpressions(first_cmd, last_cmd);
    return SolveBiggerRedir(first_cmd, last_cmd);

}

node *Solve_Pipe(node * first_cmd, node * last_cmd)
{
    // int rc = fork();
    // if (rc < 0) { // fork failed; exit
    //     fprintf(stderr, "fork failed\n");
    //     exit(1);
    // } else if (rc == 0) { // child (new process)
    //     printf("I'm in child process...\n");
    // } else { // parent goes down this path (main)
    //     int wc = wait(NULL);
    // }
}
// Leaves
#define READ_END 0
#define WRITE_END 1

node * Execute(node * first_cmd, node * last_cmd){
    if(first_cmd == last_cmd){
        return Solve_Leaves(first_cmd, last_cmd);
    }

    printf("Into Execute Method...\n");

    // AND OR
    node * AND_OR = Search_AND_OR(first_cmd, last_cmd); // Buscando And u Or (&& u ||) sin que se encuentren dentro de un if
    Expression * AND_OR_com;
    if(AND_OR != NULL)
        AND_OR_com = AND_OR->value;
    if(AND_OR != NULL){

        node * output = Execute(first_cmd, AND_OR->previous);
        Expression * output_com = output->value;
        if(AND_OR_com->operators == AND){
            if(output_com->std_out == "true") // Si && retorna true devuelve el codigo a la derecha
                return Execute (AND_OR->next, last_cmd);
            return output;
        }else if(AND_OR_com->operators == OR){
            if(output_com->std_out == "false") // Si || retorna false devuelve el codigo a la derecha
                return Execute(AND_OR->next, last_cmd);
            return output;
        }
        // free(output);
        // free(output_com);
        // return NULL;

        // free(AND_OR);
        // free(AND_OR_com);
    }
    
    printf("Start searching Pipes...\n");
    // Pipes
    node * PIPE_node = Search_PIPE(first_cmd, last_cmd);
    printf("Out of found PIPE method...\n");
    Expression * PIPE_node_com;
    if(PIPE_node != NULL)
        PIPE_node_com = PIPE_node->value;

    if(PIPE_node!= NULL){ // 3 casos
        // int fd1[2];
        // int status,pid;

        // pipe(fd1);
        // pid=fork();
        // if(pid==0)
        // {
        //     dup2(fd1[READ_END],STDIN_FILENO);
        //     close(fd1[READ_END]);

        //     dup2(fd1[WRITE_END],STDOUT_FILENO);
        //     close(fd1[WRITE_END]);
        //     return Execute(first_cmd,PIPE_node->previous);
        // }
        // else
        // {
        //     pid=fork();
        //     if(pid==0)
        //     {
        //         dup2(fd1[READ_END],STDIN_FILENO);
        //         close(fd1[WRITE_END]);
        //         return Execute(PIPE_node->next,last_cmd);

        //     }
        //     wait(NULL);
            
        int fd[2];
        pid_t pidC;
        // char buf[10];
        // int num;

    

        pipe(fd);
        pidC = fork();
        if(pidC==0){
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            // printf("I'm in execute 1...\n");
            Execute(first_cmd, PIPE_node->previous);
            // execvp(myargs[0], myargs);
            // exit(0);
            // break;
        }
        else if(pidC==-1){
            // break;
            printf("Error\n");
        }
        else{

            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            // num = read(fd[0], buf, sizeof(buf));
            // printf("%d\n",num);
            close(fd[0]);

            // printf("I'm in execute 2...\n");
            Execute(PIPE_node->next, last_cmd);
            // execvp(myargsaaa[0], myargsaaa);
            // break;
        }
            // free(fd);

            wait(NULL);

        // }
        printf("found a Pipe...\n");

       // node * output;
        // Creo las variables de tipo Expression auxiliares, necesarias en este if para pedirles su -> operators
        //Expression * first_cmd_next_com = first_cmd->next->value;
        //Expression * first_cmd_next_next_com = first_cmd->next->next->value;



        // if(first_cmd_next_com->operators == ARGS){ // Caso en q sea una hoja a la derecha

            
        //     node * pipe_left = Solve_Leaves(first_cmd, PIPE_node->previous);
        //     Expression* pipe_left_com = pipe_left->value;
        //     PIPE_node_com->std_in = pipe_left_com->std_out;
        //     //node * pipe_right = 
        //     SolveBiggerRedir(PIPE_node->next, last_cmd);
        //     output = Execute(PIPE_node->next, last_cmd);
        // }
        // else if(first_cmd->next != NULL && first_cmd->next->next != NULL && first_cmd->next->next == PIPE_node){ // Caso 2: Luego del primer PIPE que encontramos viene otro PIPE | Expression | ...
        //     node * pipe_left = Solve_Leaves(first_cmd, PIPE_node->previous);
        //     Expression* pipe_left_com = pipe_left->value;
        //     PIPE_node_com->std_in = pipe_left_com->std_out;
        //     // node * pipe_right = Solve_Leaves(PIPE_node, )
        //     if(PIPE_node_com->std_in == NULL)
        //         PIPE_node_com->std_in = pipe_left_com->std_out;
        //     Execute(PIPE_node->next, last_cmd);
        // }
        // else if(first_cmd->next != NULL && first_cmd_next_com->operators == IF){ // 3er caso: Le sigue un if a la derecha | if()then()else()...
        //     node * pipe_left = Solve_Leaves(first_cmd, PIPE_node->previous);
        //     Expression* pipe_left_com = pipe_left->value;
        //     PIPE_node_com->std_in = pipe_left_com->std_out;
        //     Execute(PIPE_node->next, last_cmd);
        // }

        // free(PIPE_node);
        // free(PIPE_node_com);

        // free(first_cmd_next_com);
        // free(first_cmd_next_next_com);
       // return output;
        // free(output);
    }

    
    printf("Starting If Else part...\n");
    // If Else
    // Si se llego hasta aca fue debido a que no se encontraron ni PIPES, ni || o && en la lista de Comandos que tenemos desde el inicio hasta este punto
    // Luego tomamos el If que nos queda y ejecutamos el codigo de la siguiente forma: Buscamos hasta que encontremos un Then, ejecutamos el If y luego dependiendo 
    // de la salida ejecutamos el Then o el Else llegando asi a encontrar un End
    // ***If Else***
    // En caso de existir un If Else, ejecutamos hasta el codigo donde comienza el If else o Then y verificamos, si es If Else evaluamos el codigo y de ser True entramos al Then,
    // sino, buscamos el Else
    node * IF_ELSE_node = Search_IF_THEN_ELSE(first_cmd, last_cmd, IF);
    if(IF_ELSE_node != NULL){ // 2 casos
        //Buscando para IF_ELSE o simplemente THEN
        
        node * THEN_node = Search_IF_THEN_ELSE(IF_ELSE_node, last_cmd, THEN);
        node * END_node = Search_IF_THEN_ELSE(THEN_node, last_cmd, END);
        node * ELSE_node = Search_IF_THEN_ELSE(THEN_node, last_cmd, ELSE);

        //Caso 1 para IF ELSE
        node * IF_output = Execute(IF_ELSE_node, THEN_node->previous); // Aca se supone que se modifique la variable first_cmd, para que luego apunte hacia la instruccion siguiente al 
        if(IF_output){ // Si devuelve True es xq tiene q ir hacia el THEN
            // Ejecuta el Then
            Execute(THEN_node, ELSE_node);
        }
        else{
            // Ejecuta el Else
            Execute(ELSE_node, END_node);
        }

        // Si encuentra un IF_ELSE entra en el ciclo
        // IF_ELSE_node = Search_IF_THEN_ELSE(first_cmd, last_cmd, IF_ELSE);

        //Caso 2: Buscando para IF THEN ELSE
        // while (IF_ELSE_node == IF_ELSE)
        // {
        //     node * THEN = 
        // }
        // free(THEN_node);
        // free(END_node);
        // free(ELSE_node);
        // free(IF_output);
    }
    // if(IF_ELSE_node != NULL)
    //     free(IF_ELSE_node);

    printf("Starting to solve leaves...\n");

    // Leaves
    return Solve_Leaves(first_cmd, last_cmd);
}

int main(int argc, char const *argv[])
{
    

    Expression * c = (Expression*)malloc(sizeof(Expression));
    c->name=strdup("ls"); 
    c->operators = SIMPLE_EXPRESSION;
    c->std_in=NULL;

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


    // Testing ls -i | grep u | wc
    Expression * h = (Expression*)malloc(sizeof(Expression));
    h->name = strdup("|");
    h->operators = PIPE;

    Expression * i = (Expression*)malloc(sizeof(Expression));
    i->name = strdup("wc");
    i->operators = SIMPLE_EXPRESSION;
    list * l = init_list(c);

    //push_back(l, c);
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

