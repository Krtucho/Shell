#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <search.h>
#include <unistd.h>
#include <sys/wait.h>
#include<sys/stat.h>
//#include<stdbool.h>// Booleanos
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include<signal.h>

#pragma region Lista

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

void print_list(list * t) 
{
    node * current = t->head;
    printf("Imprimiendo lista...\n");
    while (current != NULL) {
        printf("%s ", (char*)current->value);
        current = current->next;
    }
    printf("\n");
}


#pragma endregion

#pragma region Expression
enum OPERATORS{
    TRUE = 0,
    FALSE = 1,
    SIMPLE_EXPRESSION=2,
    EXIT=3,
    CD=4,
    HISTORY=5,
    HELP=6,
    GET=7,
    UNSET=8,
    SET=9,
    //GET=10,
    SET_CHARACTER=19,
    AND = 20,
    OR = 21,
    PIPE = 22,
    IF = 23,
    REDIRBIG=24,
    THEN=25,
    ELSE=26,
    END=27,
    ARGS=28,
    IF_ELSE=29,
    REDIRLESS=30,
    ARCHIVE=31,
    DOUBLEREDIRBIG=32
    
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

#pragma region Utiles

static char *special_strings[] = {"true","false","exit","cd","history","help","set","`","get","unset","&&","||",
                        "|","if",">","then","else","end","<",">>" };

static int specials_operators[] = { TRUE, FALSE, EXIT,CD, HISTORY, HELP,SET,SET_CHARACTER, GET, UNSET,AND , OR,
                PIPE,IF,REDIRBIG,THEN, ELSE,END,REDIRLESS,DOUBLEREDIRBIG};

void ConcatChar(char c, char *chain)
{
    char temp[2];
    temp[0] = c;
    temp[1] = '\0';
    strcat(chain, temp);
}

int GetIndexOF(char* word)
{
    for (int i = 0; i < 19/*sizeof(special_strings)*/; i++)
    {
        if(strcmp(special_strings[i],word)==0)return i;
    }
    return -1;
}

int GetOperator(char* word)
{
    int index=GetIndexOF(word);
    if(index==-1)
    {
        return SIMPLE_EXPRESSION;
    }
    return specials_operators[index];
}

char GetOneChar(char* strline,bool history)
{
    char c=getchar();
    if(history)ConcatChar(c,strline);
    return c;
}

void EndReadLine(char* strline,bool history)
{
    char c;//=GetOneChar();
    while(c!= '\n')    
    {
        c=GetOneChar(strline,history);
    }
}

Expression* GetStructExpression(char* name, int op)
{
    Expression * exp = (Expression*)malloc(sizeof(Expression));//para guardar la primera expresion
    exp->name=name;
    exp->operators=op;
    return exp;
}


#pragma endregion

#pragma region Clasificacion de Caracteres

int SpecialCaracter(char c)//devuelve si es un caracter especial simple
{
    //if(c=='#')return 1;
    //if(c==';')return 1;
    if(c=='&')return 1;
    if(c=='|')return 1;
    if(c=='<')return 1;
    if(c=='>')return 1;
    if(c=='`')return 1;
    return 0;
}

int SpecialCaracters(char* c)//devuelve si es un caracter especial en general
{
    //if(c=='#')return 1;
    //if(c==';')return 1;
    //if(strcmp(c,"&")==0)return 1;///////////not implemented
    if(strcmp(c,"|")==0)return 1;
    if(strcmp(c,"<")==0)return 1;
    if(strcmp(c,">")==0)return 1;
    if(strcmp(c,"`")==0)return 1;
    if(strcmp(c,"&&")==0)return 1;
    if(strcmp(c,"||")==0)return 1;
    if(strcmp(c,">>")==0)return 1;
    // if(c=="|")return 1;
    // if(c=="<")return 1;
    // if(c==">")return 1; 
    // if(c=="&&")return 1;
    // if(c=="||")return 1;
    // if(c==">>")return 1; 
    return 0;
}

bool RedirCaracter(char* c)//devuelve si es un caracter especial de los que puede ir sin un comando delante
{
    if(strcmp(c,"<")==0||strcmp(c,">")==0||strcmp(c,">>")==0)return true;
    return false;

}

bool IfCommand(char* c)//devuelve si es una expresion del tipo if, else then o end
{
    if(strcmp(c,"if")==0||strcmp(c,"then")==0||strcmp(c,"else")==0||strcmp(c,"end")==0)return true;
    return false;
}


#pragma endregion


#pragma region Comandos Especiales

void Help(Expression argument)
{

}
bool did_ctrl_c=false;
int pid;
void CtrlC()
{
    if(did_ctrl_c)
    {

    }
    else
    {
        did_ctrl_c=true;
    }
}

void History()
{

}
#pragma endregion



#pragma region Ejecucion

void Run(node * com){
    // printf("Inside Run Method...\n");
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
    // printf("Out of first while...\n");

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

    pid_t rc = fork();
    if (rc < 0) { // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) { // child (new process)

        Expression * com_prev;// = com->previous;
        if(com->previous != NULL){
            com_prev = com->previous->value;

            if(com_prev->operators == PIPE){
                if(com->previous->previous != NULL){
                    com_prev = com->previous->previous->value;

                    if(com_prev->operators == EXIT && com_to_exec->std_in==NULL){
                        com_to_exec->std_in=strdup(com_prev->name);
                        FILE * fp;
                        fp = freopen(com_to_exec->std_in,"w", stdin);
                        fclose(fp);


                        fp = freopen(com_to_exec->std_in,"r", stdin);

                        int e = execvp(myargs[0], myargs);
                        fclose(fp);
                    }
                }
            }
        }

        FILE * fp;
        if(com_to_exec->std_in != NULL){
            // printf("com_to_exec->std_in != NULL..\n");

            fp = freopen(com_to_exec->std_in,"r", stdin);
        }
        // char buf[10];
        // int num = read(STDIN_FILENO, buf, sizeof(buf));
        // printf("%d\n",num);
        int e = execvp(myargs[0], myargs);

        if(com_to_exec->std_in != NULL){
            fclose(fp);
        }

        

       

        //int e = execvp(myargs[0], myargs);
        // fclose(stdin);
        // fclose(stdout);
        // kill(rc, SIGKILL);

        // printf("I'm in child process...\n");
    }
    else { // parent goes down this path (main)
        int *status=0;
        pid_t p=waitpid(rc,status,0);
        //exit(1);
        //kill(rc, SIGKILL);
        // signal(SIGCHLD, handler(SIGKILL));
        // int child_pid = waitpid(-1, status, WNOHANG);
        // printf("%d\n", child_pid);
        //int wc = wait(NULL);
        // fclose(stdin);
        // fclose(stdout);
        kill(rc, SIGKILL);
    }
    // close();
    // kill(rc, SIGKILL);
    // exit(1);
   
    // printf("%d", e);
    // fclose(stdin);
    // fclose(stdout); 
}

void handler(int sig, pid_t pid){
    kill(pid, sig);
}

int SIMPLE_Expression_CODE(node * exp){
    // int rc = fork();
    // if (rc < 0) { // fork failed; exit
    //     fprintf(stderr, "fork failed\n");
    //     exit(1);
    // } else if (rc == 0) { // child (new process)
    //     printf("I'm in child process...\n");
        Run(exp);
        return 0;
    // } else { // parent goes down this path (main)
    //     int wc = wait(NULL);
    // }
}

// int AND_CODE(node * com){

// }
int TRUE_CODE(node* exp){
    return 0;
}

int FALSE_CODE(node* exp){
    return 1;
}

bool inside_pipe = false;

int EXIT_CODE(node* exp){
    // printf("Inside EXIT_CODE METHOD\n");
    if(inside_pipe){
        // Expression * next=exp->next->value;
        // if(next->operators==PIPE)
        // {
        //     Expression * command_=exp->next->next->value;
        //     command_->std_in=command_->name;
        //     freopen(command_->std_in,"w", stdin);
        //     fclose(stdin);

        // }
        
        //FILE * fp;
        //     // printf("Inside pipe was True...\n");
        // printf("%d",0);
        // pid_t rc = fork();
        // if (rc < 0) { // fork failed; exit
        //     fprintf(stderr, "fork failed\n");
        //     exit(1);
        // } else if (rc == 0) { // child (new process)
        //     exit(0);
        // }
        // else { // parent goes down this path (main)
        //     int *status=0;
        //     pid_t p=waitpid(rc,status,0);
            //exit(1);
            //kill(rc, SIGKILL);
            // signal(SIGCHLD, handler(SIGKILL));
            // int child_pid = waitpid(-1, status, WNOHANG);
            // printf("%d\n", child_pid);
            //int wc = wait(NULL);
            // fclose(stdin);
            // fclose(stdout);
        //     kill(rc, SIGKILL);
        // }
        //     // write(STDOUT_FILENO, 0, 1); 
        //Expression * current=exp->value;
        //fp = freopen(current->name, "w", stdout);
        //fclose(fp);
        // exit(0);
        return 0;
    }
    exit(0);
    return 0;
}

int SET_CODE(node* exp){
    // printf("Inside Run Method...\n");
    Expression * com_to_exec = exp->value;
    if(com_to_exec->operators != SET)
        return -1;

    Expression * var_name = exp->next->value;

    node * last = exp->next->next;
    Expression * last_exp;

    if(last != NULL)
        last_exp = last->value;
    else
        last_exp = NULL;

    int count = 0;
    // while(last_com != NULL && (last_com->operators == ARGS || last_com->operators== ARCHIVE)){// Con ARGS me refiero a los argumentos lo q aun no se bien como ponerle
    //     count++;
    //     last = last->next;
    //     if(last != NULL)
    //         last_com = last->value;
    //     else
    //         last_com = NULL;
    // }
    // printf("Out of first while...\n");
    return 0;
}

int (*testing[])(node*) = {
        TRUE_CODE,
        FALSE_CODE,
        SIMPLE_Expression_CODE,
        EXIT_CODE,
        SET_CODE,
        // GET_CODE,
        // UNSET_CODE,
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
            //continue;
            }
        else if(current_temp->operators == END){
            is_if = false;
            //continue;
            }
        else if((current_temp->operators == AND || current_temp->operators == OR) && !is_if){
            // free(current_temp);
            return current;
        }

        current = current->next;
    }

    // if(current == NULL)
    //     printf("current is NULL");
    // if(current_temp == NULL)
    //     printf("current_temp is NULL");
    // free(current_temp);
    // free(current);
    return NULL;
}

node * Search_PIPE(node * first_cmd, node * last_cmd){
    // printf("In Search PIPE method...\n");
    bool is_if = false;
    node * current = first_cmd;
    Expression * current_temp;// = current->value;
    while(current != last_cmd){
        // printf("In while searching pipes...\n");
        current_temp = current->value;
        // current_temp->operators
        if(current_temp->operators == IF){
            is_if = true;
            //continue;
            }
        else if(current_temp->operators == END){
            is_if = false;
            //continue;
            }
        else if(current_temp->operators == PIPE && !is_if){
            // free(current_temp);
            return current;
        }

        // printf("Changing current variable...\n");
        current = current->next;
        // if(current == NULL)
        //     printf("current is NULL");
        // printf("current variable changed...\n");
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
    // printf("I'm in IF THEN ELSE method...\n");
    node * current = first_cmd;
    Expression * current_temp = current->value;
    while(current != NULL){
        current_temp = current->value;
        if(current_temp->operators == operator){
            return current;
        }
        current = current->next;
    }
    // free(current);
    // free(current_temp);
    // printf("End of Search IF_THEN_ELSE method...\n");
    return NULL;
}

// Leaves
int GetIndex(Expression * op){
    return (int)op->operators;
}

int ExecuteExpression(node * cmd_to_exec){
    Expression * node_com = cmd_to_exec->value;
    // enum OPERATORS op = node_com->operators;
    // printf("Getting index_method...\n");
    // printf("%d", node_com->operators);
    int index = node_com->operators;
    // printf("Index: %d \n", index);
    return testing[index](cmd_to_exec);
    // free(node_com);
}


int SolveExpressions(node * first_cmd, node * last_cmd){
    node * current = first_cmd;

    // printf("Start ExecuteExpression method\n");

    return ExecuteExpression(current);
}


int SolveBiggerRedir(node * first_cmd, node * last_cmd, int exp_out){
    Expression * output = NULL;
    node * current=first_cmd;
    FILE *fp;
    bool redir_found = false;

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

            // continue;

        }
        else if(current_exp->operators==DOUBLEREDIRBIG)
        {
            Expression *current_next=current->next->value;
            fp = freopen(current_next->name, "a", stdout);
            fclose(fp);
            output=current_next;
            // continue;

        }
        current=current->next;
    }
    if(output!=NULL){
        Expression * exp=first_cmd->value;
        fp=freopen(output->name,"a",stdout);
        printf("%s \n", exp->std_out);
        fclose(fp);

        // exp->std_out="1";
        redir_found = true;
    }

    if(redir_found == false)
        return exp_out;
    return 0;
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

    // printf("Llegamos a < wiiiiiii!!!!\n");
    while (current != NULL)
    {
        current_exp = current->value;
        if(current_exp->operators == REDIRLESS){
            // printf("Inside current REDIRLESS if...\n");
            // printf("current_exp.name = %d", current_exp->operators);
            input=current->next;
            break;
        }
        current = current->previous;
    }
    // if(current_exp != NULL)
    //     printf("%s\n", current_exp->name);
    // else
    //     printf("current_exp = NULL");
    
    if(input!=NULL)
    {
        // printf("Input was not equal NULL...\n");
        Expression * input_exp=input->value;
        // printf("%s \n", input_exp->name);
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
        // printf("%s \n", fisrt_exp->std_in);
    }
    // printf("End of Less Redir...\n");
    return first_cmd;
}


int Solve_Leaves(node * first_cmd, node * last_cmd){
    SolveLessRedir(first_cmd, last_cmd);
    // printf("Starting to solve Expressions...\n");
    int exp_out = SolveExpressions(first_cmd, last_cmd);
    return SolveBiggerRedir(first_cmd, last_cmd, exp_out);

}

// Leaves
#define READ_END 0
#define WRITE_END 1

int Execute(node * first_cmd, node * last_cmd){
    if(first_cmd == last_cmd){
        return Solve_Leaves(first_cmd, last_cmd);
    }

    // printf("Into Execute Method...\n");

    // AND OR
    node * AND_OR = Search_AND_OR(first_cmd, last_cmd); // Buscando And u Or (&& u ||) sin que se encuentren dentro de un if
    Expression * AND_OR_com;
    if(AND_OR != NULL){
        AND_OR_com = AND_OR->value;

        int output = Execute(first_cmd, AND_OR->previous);
        // printf("Out of left &&...\n");
        // wait(NULL);
        //Expression * output_com = output->value;
        if(AND_OR_com->operators == AND){
            if(output ==0)
            { // Si && retorna true devuelve el codigo a la derecha
                // fclose(stdin);
                // fclose(stdout); 
                //close(STDOUT_FILENO);
                //close(STD_FILENO);
                return Execute (AND_OR->next, last_cmd);
            }
            return output;
        }else if(AND_OR_com->operators == OR){
            if(output!=0) // Si || retorna false devuelve el codigo a la derecha
                return Execute(AND_OR->next, last_cmd);
            return output;
        }
        // free(output);
        // free(output_com);
        // return NULL;

        // free(AND_OR);
        // free(AND_OR_com);
    }
    
    // printf("Start searching Pipes...\n");
    // Pipes
    node * PIPE_node = Search_PIPE(first_cmd, last_cmd);
    // printf("Out of found PIPE method...\n");
    Expression * PIPE_node_com;
    //if(PIPE_node != NULL)
        

    if(PIPE_node!= NULL){ // 3 casos
        PIPE_node_com = PIPE_node->value;
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
        int *status=0;
        // char buf[10];
        // int num;
        int output=0;

        inside_pipe = true;
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

           // pid_t p=waitpid(pidC,status,0);
            //kill(p, SIGKILL);
            // pidC = fork();
            // if(pidC==0)
            // {
            // pid_t p1=waitpid(pidC,status,0);
            // kill(p1, SIGKILL);

            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
                // num = read(fd[0], buf, sizeof(buf));
                // printf("%d\n",num);
            close(fd[0]);

                // printf("I'm in execute 2...\n");
            output = Execute(PIPE_node->next, last_cmd);
            // }
            // else
            //{
           // pid_t p=waitpid(pidC,status,0);
                //wait(NULL);
            //}
           //wait(status);

           
        //    pid_t p1=waitpid(pidC,status,0);
        //     kill(pidC, SIGKILL);
            int child_pid = waitpid(-1, status, WNOHANG);
            // printf("%d\n", child_pid);
            // execvp(myargsaaa[0], myargsaaa);
            // break;
        }
        
        
        // wait(status);

            // free(fd);

       // wait(NULL);
        // }
        inside_pipe = false;

        Expression * com_prev;// = com->previous;
        if(PIPE_node->previous != NULL){
            com_prev = PIPE_node->previous->value;

            if(com_prev->operators == EXIT ){
                if(remove("exit")){}
            }
        }
        // printf("found a Pipe...\n");
        return output;


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

    
    // printf("Starting If Else part...\n");
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
        int IF_output = Execute(IF_ELSE_node->next, THEN_node->previous); // Aca se supone que se modifique la variable first_cmd, para que luego apunte hacia la instruccion siguiente al 
        if(IF_output==0){ // Si devuelve True es xq tiene q ir hacia el THEN
            // Ejecuta el Then
            return Execute(THEN_node->next, ELSE_node->previous);
        }
        else if(ELSE_node != NULL){
            // Ejecuta el Else
            return Execute(ELSE_node->next, END_node->previous);
        }
        return 0;

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

    // printf("Starting to solve leaves...\n");

    // Leaves
    return Solve_Leaves(first_cmd, last_cmd);
}
#pragma endregion


#pragma region Parser


void EjecuteLine(list* line)
{
    
    bool command=true;//indica si en este momento debo recibir un comando
    bool archive=false;//indica si en este momento debo recibir un archivo
    bool special_caracter_last=false;//indica si la palabra anterior fue un caracter especial
    bool if_caracter_last=false;//indica si la expresion anterior es de tipo if, else then o end
    
    Expression * exp1 = (Expression*)malloc(sizeof(Expression));//para guardar la primera expresion
       
    char* temp = pop_front(line)->value; //guarda temporalmente el primer valor de la lista de string
    
    if(SpecialCaracters(temp)&& !RedirCaracter(temp))////cuando como primera palabra tenemos un caracter especial que no puede ocupar ese lugar
    {
        printf("syntax error near unexpected token `%s'",temp);
        return;
    }

    exp1->name=strdup(temp);
    exp1->operators = GetOperator(temp);

    list* exp_line=init_list(exp1); //linea de expresiones clasificadas
    //printf("%s:%d\n",exp1->name,exp1->operators);

    //free(exp1);///verificar si esta bien liberar aqui
    //printf("%s:%d",exp_line->head->value->name,exp1->operators);

    command=false;
    if(SpecialCaracters(temp)){command=true;special_caracter_last=true;}
    if_caracter_last=IfCommand(temp);
    if(if_caracter_last)command=true;
    if(RedirCaracter(temp)) archive=true;
    //free(temp);//verificar si esta bien liberar aqui

    //node* current=exp_line->head;

    printf("%s:%d \n",exp1->name,exp1->operators);


    while(line->head!=NULL)
    {
        temp=line->head->value;
        //push_back(exp_line,(Expression*)malloc(sizeof(Expression)));
        //current=current->next;
        //current->value->name=temp;

        //Expression * exp = (Expression*)malloc(sizeof(Expression));//para crear cada expresion de la lista de expresiones
        char* name=strdup(temp);
        bool special_caracter_now=SpecialCaracters(temp);//la expresion actual es un caracter especial
        if(special_caracter_last && special_caracter_now){printf("syntax error near unexpected token `%s'",temp); return;}
        if(if_caracter_last && special_caracter_now && !RedirCaracter(temp)){printf("syntax error near unexpected token `%s'",temp); return;}
        if(special_caracter_last || if_caracter_last)command=true;

        //exp->operators=GetOperator(temp);
        int op=GetOperator(temp);
        //if(exp->operators==SIMPLE_EXPRESSION)
        if(op==SIMPLE_EXPRESSION)
        {
            //if(!command)exp->operators=ARGS;//Si no estamos esperando un comando entonces estamos en presencia de un argumento
            //if(archive)exp->operators=ARCHIVE;//Si la expresion anterior es un caracter especial de redireccion shora estamos en presencia de un archivo
            if(!command)op=ARGS;//Si no estamos esperando un comando entonces estamos en presencia de un argumento
            if(archive)op=ARCHIVE;//Si la expresion anterior es un caracter especial de redireccion shora estamos en presencia de un archivo


        }    
        if_caracter_last=IfCommand(temp);
        command=false;
        archive=false;
        if(RedirCaracter(temp)) archive=true;

        special_caracter_last=special_caracter_now;
        push_back(exp_line,GetStructExpression(name, op));
        //printf("%s:%d\n",exp->name,exp->operators);
        printf("%s:%d\n",name,op);

        free(pop_front(line));
    
    }

    ////Descomentar esto desde aqui
    int _pid =fork();
    if(_pid==0)
    {
    Execute(exp_line->head,exp_line->tail);

    }
    else if(_pid>0)
    {
        return;
    }
    else
    {
        int *status=0;
        waitpid(_pid,status,0);

    }
////Descomentar esto hasta aqui

    // fclose(stdin);
    // fclose(stdout);
    free_list(exp_line);
}

void ReadAndEjecuteLine(list* line,char* word, char c)//crea una lista de string por cada instruccion y luego manda a ejecutarlas
{
    bool history=true;
    if(c==' ') history=false;
    char *strline=(char*)calloc(sizeof(char),500);
    if(history)ConcatChar(c,strline);

    while(c!= '\n')
        {
         
            if(c=='#')
            {
                EndReadLine(strline,history);
                break;
            }

            // // // // if(c=='&')
            // // // // {
            // // // //     if(strcmp(word,"")!=0 && strcmp(word,"&")!=0)
            // // // //     {
            // // // //         push_back(line, strdup(word));
            // // // //         strcpy(word,"");
            // // // //     }
            // // // //     ConcatChar(c,word);                    
            // // // //     c=GetOneChar(strline,history);


            // // // //     if(c!='&' && strcmp(word,"&")==0)
            // // // //     {
            // // // //         if(c=='\n')
            // // // //         {
            // // // //         //mandar el prceso al background
            // // // //         }
            // // // //         printf("syntax error near unexpected token `%c'",c);

            // // // //         return;
            // // // //     }

            // // // //     // if(c!='&')
            // // // //     // {
            // // // //     //     EndReadLine(strline,history);//Hacer el background
            // // // //     // }
            // // // //     /////mandar el proceso actual a background
            // // // //     /////verificar que primero se busca un and para despues comprobar que sea un &
            // // // // }

            if(c==';')
            {
                ////////Primero terminar de armar la linea actual, y mandarla a ejecutar
                if(strcmp(word,"")==0||strcmp(line->head->value,"init")==0)
                {
                    printf("syntax error near unexpected token `;'");
                    return;
                }
                push_back(line, strdup(word));
                strcpy(word,"");
                pop_front(line);
                EjecuteLine(line);
                //print_list(line);                
                //pop_back(line);
                free_list(line);
                line=init_list("init");

                c=GetOneChar(strline,history);
                //c=GetOneChar();
                continue;
            }
            
            if(SpecialCaracter(c))
            {            
                if(word[0]=='\0' ||SpecialCaracter(word[0]) )//Si la palabra que tengo hasta el 
                //momento es vacia es porque se dejo un espacio y tengo que crear una nueva 
                //para el caracter especial o en el caso que se este conforrmando un caracter especial,
                //lo termino de conformar.
                {
                    ConcatChar(c,word);
                    if(!SpecialCaracters(word))
                    {
                        printf("syntax error near unexpected token '%c' \n",c);
                        strcpy(word,"");
                        free_list(line);
                        line=init_list("init");
                        EndReadLine(strline,history);
                        return;
                    }
                    c=GetOneChar(strline,history);
                    continue;
                }
                
                   push_back(line, strdup(word));
                   strcpy(word,"");
                   ConcatChar(c,word);
                   
                   c=GetOneChar(strline,history);
                   //c=GetOneChar();
                   continue;      
                
            }

            if(SpecialCaracters(word))///Si es un caracter normal pero esta pegado a un caracter especial anterior
            {
                if(c==' ')c=GetOneChar(strline,history);
                push_back(line, strdup(word));//saco el caracter especial para la lista y renuevo a word
                strcpy(word,"");
                ConcatChar(c,word);
                c=GetOneChar(strline,history);
                //c=GetOneChar();
                continue;
                
            }
            
            if(c==' ')
            {
                if(word[0]=='\0')
                {
                    c=GetOneChar(strline,history);
                    //c=GetOneChar();
                    continue;
                    
                }
                push_back(line, strdup(word));

                strcpy(word,"");///vacio a word para comenzar una nueva palabra
                c=GetOneChar(strline,history);
                //c=GetOneChar();
                continue;
            }
            
            //if(concat)
            ConcatChar(c,word);
            c=GetOneChar(strline,history);
            //c=GetOneChar();
        }

        if(strcmp(word,"")!=0) push_back(line, strdup(word));
        strcpy(word,"");
        free(word);
        pop_front(line);
        //print_list(line);
        EjecuteLine(line);

        //free_list(line);

///////////Llamar aqui al metodo para guardar history

    //printf("\n HISTORY: %s \n",strline);
}

void Shell()
{

 while (1)
    {
        printf("\n my-shell $ ");

        char * word= (char*)calloc(sizeof(char),100);//word es cada una de las palabras que se mandan en un espacio de line
        strcpy(word,""); 

        list* line=init_list("init"); //line es la linked list que guarda los comandos argumentos y caracteres especiales
        //pop_front(line);

        char c=getchar();  //cada uno de los char a leer de consola   
        
        //bool concat=true;

        ReadAndEjecuteLine(line,word,c);
        //EndReadLine("",false);

    }

}

#pragma endregion

int main(int argc, char const *argv[])
{
    signal(SIGINT,CtrlC);
    Shell();
    return 0;
}
