#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <search.h>
#include <unistd.h>
#include <sys/wait.h>
#include<sys/stat.h>
//#include<stdbool.h>// Booleanos
#include <fcntl.h>
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
        printf("Index out of range.\n");
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
        printf("Index out of range.\n");
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
    CD=6,
    HISTORY=4,
    HELP=5,
    AGAIN=10,
    GET=9,
    UNSET=8,
    SET=7,
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

static char *special_strings[] = {"true","false","exit","cd","history","help","again","set","`","get","unset","&&","||",
                        "|","if",">","then","else","end","<",">>" };

static int specials_operators[] = { TRUE, FALSE, EXIT,CD, HISTORY, HELP,AGAIN,SET,SET_CHARACTER, GET, UNSET,AND , OR,
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
    for (int i = 0; i < 21/*sizeof(special_strings)*/; i++)
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
    if(c==';')return 1;
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
    if(strcmp(c,";")==0)return 1;

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

bool PossibleArgumentExpression(int op)
{
    if(op == TRUE || op == FALSE || op == EXIT || op == CD || op == HISTORY || op == HELP || op == GET || op == UNSET || op == SET)
    {
        return true;
    }
    return false;
}

#pragma endregion

#pragma region Comandos Especiales

#pragma region Help
void help_init()
{
    printf("Integrantes: Carlos Carret Miranda(C212)\n"
                         "Lauren Guerra Hernandez(C212)\n"
        "Funcionalidades:\n"
        "basic: funcionalidades básicas (1-8)  (3 puntos)\n"
        "multi-pipe: múltiples tuberías  (9)   (1 punto)\n"
        "spaces: múltiples espacios   (11)    (0.5 puntos)\n"
        //"history: trabajar con el historial de comandos f(12)    (0.5 puntos)\n"
        //"ctrl+c: capturar y enviar señales a procesos (13) (0.5 puntos)\n"
        "if:funcionalidad 15, 1punto"
        "chain: concatenación de comandos con ; y operaciones booleanas (14) (0.5 puntos)\n"
        "help: ayuda del shell (17) (1 punto)"

        "Comandos built-in:\n"
        "cd: cambia de directorios\n"
        "exit: termina el shell\n"
        //"history: muestra los 10 últimos comandos ejecutados\n"
        //"again: se sutituye por un comando seleccionado dentro del historial\n"
        "true: retorna 1\n"
        "false: retorna 0\n"
        "help: muestra esta ayuda\n\n"
        //"Total: 6.0 puntos\n\n"
        );
}

void help_basic()
{
    printf("\n Funcionalidad 1: Al comenzar cada iteraci'on de nuestra shell lo primero que se realiza en el m'etodo Shell"
    "es imprimir `my-shell $` en la consola para luego con el getchar() leer del stdin de la misma.\n"
	"Funcionalidad 2: Los comandos se ejecutan correctamente ya sean propios del sistema o pertenezcan"
    "a nuestro build-in.\n"
    "Funcionalidad 3: El comando cd cambia el directorio ,empleando la función chdir, seg'un el path que recibe como argumento."

//    "Funcionalidad 4: > >> <"
//    "Funcionalidad 5: |"
    "Funcionalidad 6: Luego de terminar la ejecuci'on de una l'inea de comandos se vuelve al m'etodo shell"
    "donde se vuelve a imprimir `my-shell $` y se puede entrar otra l'inea de comando para ser ejecutada \n"
    "Funcionalidad 7: Se separan los comandos con un espacio, mas al tambi'en estar implementada la funcionalidad"
    "spaces, se puede dejar m'as de un espacio entre comandos y entre 0 y muchos con operadores especiales .\n"
    "Funcionalidad 8: En nuestro parser, m'etodo ReadAndEjecuteLine() luego de encontrarnos mediante el getchar()"
    "con el caracter #, terminamos de leer esta linea pero no guardamos nada de lo que se encuentre despu'es de #.\n\n");

}


void help_chain()
{
	printf("\n Los operadores &&, ||, ; concatenan comandos, en el caso del ; vamos leyendo cada linea, cuando"
    " nos encontramos con uno de estos enviamos esta l'inea a ejecutarse y seguimos leyendo esta l'inea de la consola y "
    "cuando finalice la mandamos a ejecutar aparte, a la hora de ejecutar una l'inea tomamos como caracteres de mayor"
    "prioridad a && y ||  , luego buscamos los pipes (command1|command2) y luego buscamos los if then else y end, por 'ultimo resolvemos"
    "las expresiones simples, del tipo command1 argumento < archivo. \n\n");
}

void help_spaces()
{
	printf("\n Entre comandos y argumentos puede haber cualquier cantidad de espacios >=1 \n. Ejemplo:\n"
    "`com1 arg1` y `com1     arg1` \n y entre comandos y operadores puede haber igualmente cualquier cantidad "
    "de espacios. \n Ejemplo: \n ");

}

void help_multipipe()
{

}

void help_controlc()
{
    printf("\n  \n ");
}



int HELP_CODE(node* argument)
{
    Expression * next;
    if(argument->next != NULL)
        next = argument->next->value;
    Expression * node_temp = argument->value;
    //if(strcmp(node_temp->name,"")==0)
    if(argument->next==NULL)
    {
        help_init();    return 0;

    }
    else if(strcmp("basic",next->name)==0)
    {
        help_basic();    return 0;

    }
    else if(strcmp("chain",next->name)==0)
    {
        help_chain();    return 0;

    }
    // else if(strcmp("history",argument)==0)
    // {
    //     help_history();
    // }

    else if(strcmp("spaces",next->name)==0)
    {
        help_spaces();    return 0;

    }
    else if(strcmp("multi-pipe",next->name)==0)
    {
        help_multipipe();    return 0;

    }

    else if(strcmp("ctr+c",next->name)==0)
    {
        help_controlc();
    }
    else
    {
         printf("invalid help command `%s'\n",next->name);
         return 1;
    }
    return 0;

}
#pragma endregion


int CD_CODE(node* argument)
{
    Expression * node_temp;
    if(argument->next!=NULL)node_temp = argument->next->value;
    if (node_temp==NULL || node_temp->operators!=ARGS)
    {
        chdir("/home");
        return 1;
    }
    int chdir_result=chdir(node_temp->name);
    if ( chdir_result == -1)
    {
         printf("bash: cd: %s: No such file or directory\n",node_temp->name);
     //   printf("No se puede acceder a la direcci'on %s",node_temp->name);
        return 1;
    }
    return 0;
}

//void Shell();
bool did_ctrl_c;
int pid;
int pid_inicial;
void CtrlC()
{
 //   printf("\n PID inicial:%d\n",pid_inicial);
  //  printf("PID actual:%d\n",pid);
   // printf("Bool:%d\n",did_ctrl_c);
    if(did_ctrl_c)
    {
        //int pidnow=getpid();
        //printf("PID Now: %d\n",pidnow);
        //printf("PID: %d\n",pid);
        //if(pid!=pid_inicial) {kill(pid,1);}
        did_ctrl_c=false;

        if(pid!=pid_inicial) kill(pid,9);

        //else             
          //  write(stdin, strdup("\n"),1);


        //int*status=0;
        //int child_pid = waitpid(pid_inicial, status, WNOHANG);
        //printf("Lo que devuelve el wait:%d\n",child_pid);
    }
    else
    {
        did_ctrl_c=true;
        if(pid==pid_inicial) 
        {
            did_ctrl_c=false;
            // char buf[10];
            // write(STDIN_FILENO, strdup("\n"),1);
            // read(STDIN_FILENO, buf, 3);
            //close(STDIN_FILENO);
           // did_ctrl_c=false;
            //printf("LLego al ctrlC\n");
            //Shell();
            //did_ctrl_c=false;

        }

        //int pidnow=getpid();
        //printf("PID Now: %d\n",pidnow);
        //printf("PID: %d\n",pid);
        //if(pid!=pid_inicial) {kill(pid,9);}
        //int*status=0;
        //int child_pid = waitpid(pid_inicial, status, WNOHANG);
        //printf("Lo que devuelve el wait:%d",child_pid);
        //kill(pid,SIGTERM);
    }
}
char history_direction[200];

#pragma region History


int only_write(char * file);
int only_append(char * file, char buf[1000], int num);
int input_read(char * file);

int HISTORY_CODE(node* argument)
{
    

    return 0;
}
int AGAIN_CODE(node*argument)
{

}
int ReadHistory(list*history_lines,char* file_contents,int fc_size)
{
    int count=0;
    
    for (int i = 0; i < fc_size; i++)
    {
        if(file_contents[i]=='\n')count++;
    }
    return count;
    
}
void SaveLine(char* strline)
{
    return;
    int std_in=dup(STDIN_FILENO);
    int std_out=dup(STDOUT_FILENO);

    input_read(history_direction);
    char file_contents[1000];
    int num=read(STDIN_FILENO,file_contents,sizeof(file_contents));

    list*history_lines=init_list("init");
    int count=ReadHistory(history_lines,file_contents,num);
    push_back(history_lines,strline);
    pop_front(history_lines);
    if(count>=10)pop_front(history_lines);
    
    char* history_output=calloc(sizeof(char),1000);

    while(count>0)
    {
        //push_back(history_output)
        strcat(history_output,history_lines->head->value);
        strcat(history_output,"\n");
        count--;
        free(pop_front(history_lines));
    }
    

    FILE * fp = fopen(history_direction, "w");
    fclose(fp);        
    only_write(history_direction);
    printf("%s",history_output);

    free(history_lines);
    free(history_output);

    dup2(std_in,STDIN_FILENO);
    dup2(std_out,STDOUT_FILENO);
    close(std_in);
    close(std_out);
}

#pragma endregion


#pragma endregion


#pragma region Ejecucion
int only_write(char * file){
    int fd = open(file, O_WRONLY | O_CREAT);

    if(fd == -1)
        return -1;

    if(file == NULL)
        fd = STDOUT_FILENO;

    int tempdup = dup2(fd, STDOUT_FILENO);
    if(tempdup == -1)
        return -1;

    close(fd);
    return 0;
}
int only_append(char * file, char buf[1000], int num){

    int fd=open(file, O_WRONLY | O_APPEND | O_CREAT , 0);

    if(fd == -1)
        return -1;

    if(file == NULL)
        fd = STDOUT_FILENO;

    write(fd, buf, num);

    close(fd);
    return 0;
}
int input_read(char * file){
    int fd = open(file, O_RDONLY | O_CREAT);

    if(fd==-1)
        return -1;

    int tempdup = dup2(fd, STDIN_FILENO);
    if(tempdup == -1)
        return -1;

    close(fd);
    return 0;
}

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

    //FILE * fp;
    //FILE * fp_out;
    //printf("Voy a hacer fork\n");

    pid_t rc = fork();
    //printf("Acabo de hacer fork\n");

  //  printf("Pid anterior:%d\n",pid);
    pid=rc;
   // printf("Pid del fork:%d\n",pid);

    did_ctrl_c=false;

    // char * buf;

    if (rc < 0) { // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) { // child (new process)
       // printf("Estoy en el fork, pid:%d\n",pid);

        Expression * com_prev;// = com->previous;
        if(com->previous != NULL){
            com_prev = com->previous->value;

            if(com_prev->operators == PIPE){
                if(com->previous->previous != NULL){
                    com_prev = com->previous->previous->value;

                    if(com_prev->operators == EXIT && com_to_exec->std_in==NULL){
                        com_to_exec->std_in=strdup(com_prev->name);
                        //FILE * fp;
                        input_read(com_to_exec->std_in);

                        //fp = freopen(com_to_exec->std_in,"w", stdin);
                        //fclose(fp);


                        //fp = freopen(com_to_exec->std_in,"r", stdin);

                        int e = execvp(myargs[0], myargs);
                        //fclose(fp);
                    }
                }
            }
        }


        //FILE * fp;

        if(com_to_exec->std_in != NULL){
            // printf("com_to_exec->std_in != NULL..\n");
            input_read(com_to_exec->std_in);

            //fp = freopen(com_to_exec->std_in,"r", stdin);
        }

        // int num = read(STDOUT_FILENO, buf, sizeof(buf));
        // printf("%d\n",num);
        // printf("%s", buf);

        if(com != NULL && com->next != NULL){

            node * current = com;
            Expression * exp;

            while(current != NULL){
                exp = current->value;
                if(exp->operators == REDIRBIG || exp->operators == DOUBLEREDIRBIG){
                    FILE * fp = fopen("temp", "w");
                    fclose(fp);
                    only_write("temp");
                    //fp_out = freopen("temp", "w", stdout);
                    break;
                }
                current = current->next;
            }
        }

        int e = execvp(myargs[0], myargs);

        //int e = execvp(myargs[0], myargs);
        // fclose(stdin);
        // fclose(stdout);
        // kill(rc, SIGKILL);

        // printf("I'm in child process...\n");
    }
    else { // parent goes down this path (main)
        int *status=0;

        // char message[20];
        // read(STDOUT_FILENO, message, 20);
        // printf("Buff: %s\n", buf);


        pid_t p=waitpid(rc,status,0);
        //exit(1);
        //kill(rc, SIGKILL);
        // signal(SIGCHLD, handler(SIGKILL));
        // int child_pid = waitpid(-1, status, WNOHANG);
        // printf("%d\n", child_pid);
        //int wc = wait(NULL);
        // fclose(stdin);
        // fclose(stdout);

        // if(com_to_exec->std_in != NULL){
        //     if(fp != NULL)
        //         fclose(fp);
        // }
        // if(fp_out != NULL)
        //     fclose(fp_out);


        kill(rc, SIGKILL);

        // printf("%s", message);
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


static list * var_list;// = init_list("neverusethisname");

int PrintVariables(){

    if(var_list->size == 1)
        return 0;

    node * current = var_list->head->next;
    char * output = "";

    while (current != NULL)
    {
        Expression * current_exp = current->value;
        // if(strcmp(current_exp->name, "neverusethisname") == 0)
        
        strcat(output, current_exp->name);
        strcat(output, " ");
        char * c = getenv(current_exp->name);
        if(c!= NULL)
            strcat(output, c);
        strcat(output, "\n");
        current = current->next;
    }
    
    printf("%s", output);
    return 0;
}

int AddVar(char * key, char * value){
    push_back(var_list, key);
    return setenv(key, value, 1);
}

int ExecuteSetCharacter(node* exp , node * last){

}

int SET_CODE(node* exp){
    //     char * a = strdup("a");
    // char * b = strdup("`ls`");

    // int result = setenv(a, b, 1);
    
    // printf("%s\n", getenv(a));


    // int removed = unsetenv(a);
    // int removed2 = unsetenv(a);

    // printf("%d\n", result);
    // char * c = getenv(a);
    // if(c != NULL)
    //     printf("%s\n", getenv(a));
    // else
    //     printf("NULL");

    // return 0;

    // printf("Inside Run Method...\n");
    Expression * com_to_exec = exp->value;
    if(com_to_exec->operators != SET)
        return -1;

    if(exp->next == NULL)
        return PrintVariables();

    Expression * var_name = exp->next->value; // me quedo con su .next(el nombre de la variable que viene a continuacion)

    if(var_name->operators != ARGS){
        return -1;
    }

    node * last = exp->next->next;
    Expression * last_exp;

    
    if(last != NULL)
        last_exp = last->value;
    else
    {
        last_exp = NULL;
    }

    if(last_exp != NULL && last_exp->operators == SET_CHARACTER){
        return ExecuteSetCharacter(exp ,last); // pasamos el comando set principal y el nodo a partir del cual se encontro el primer caracter de Set
    }

    // Buscando los argumentos para ir concatenandolos y asi formar el string final que corresponde a la variable

    // list * temp_args = init_list(com->next);

    int count = 0;
    while(last_exp != NULL && (last_exp->operators == ARGS )){// Con ARGS me refiero a los argumentos lo q aun no se bien como ponerle
        count++;
        last = last->next;
        if(last != NULL)
            last_exp = last->value;
        else
            last_exp = NULL;
    }
    // printf("Out of first while...\n");

    // char *myargs[count+1];
    //myargs[count+1] = NULL;
    // myargs[0] = strdup(var_name->name);

    char * str_value = "";

    int temp = 0; // Para ir ubicando argumenta a argumento en el arreglo
    node * current = exp->next->next;
    while (temp < count)
    {
        Expression * current_com = current->value;
        // myargs[temp] = strdup(current_com->name);
        strcat(str_value, " ");
        strcat(str_value, current_com->name);
        temp++;
        current=current->next;
    }

    AddVar(var_name->name, str_value);
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

int GET_CODE(node* exp)
{
    Expression * com_to_exec = exp->value;
    if(com_to_exec->operators != GET)
        return -1;

    if(exp->next == NULL)
        return 0;

    Expression * next_exp = exp->next->value;
    if(next_exp->operators != ARGS)
        return -1;

    char * output = getenv(next_exp->name);
    if(output == NULL)
        return -1;
    
    printf("%s", output);
    return 0;
}

node * search_str_node(list * l, char * target){
    node * current = l->head;

    while(current != NULL)
    {
        Expression * current_exp = current->value;
        if(strcmp(current_exp->name, target) == 0){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int UNSET_CODE(node* exp)
{
    Expression * com_to_exec = exp->value;
    if(com_to_exec->operators != UNSET)
        return -1;

    if(exp->next == NULL)
        return 0;

    Expression * next_exp = exp->next->value;
    if(next_exp->operators != ARGS)
        return -1;

    int removed = unsetenv(next_exp->name);

    node * temp = search_str_node(var_list, next_exp->name);
    if(temp != NULL)
        removeNode(var_list, temp);
    // char * output = getenv(next_exp->name);
    if(removed == -1)
        return -1;
    
    return 0;
}



int (*testing[])(node*) = {
        TRUE_CODE,
        FALSE_CODE,
        SIMPLE_Expression_CODE,
        EXIT_CODE,
        HISTORY_CODE,
        HELP_CODE,
        CD_CODE,
        SET_CODE,
        UNSET_CODE,
        GET_CODE,
        AGAIN_CODE,
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
    FILE * fp;
    bool redir_found = false;

    Expression * current_exp;
    while (current!=NULL)
    {
        current_exp=current->value;
        if(current_exp->operators==REDIRBIG)
        {
            Expression *current_next=current->next->value;
            // int fd = open(current_next->name, O_WRONLY | O_CREAT);
            // close(fd);

            // fp = freopen(current_next->name, "w", stdout);
            // fclose(fp);

            fp=fopen(current_next->name,"w");
            fclose(fp);
            output=current_next;

            // continue;

        }
        else if(current_exp->operators==DOUBLEREDIRBIG)
        {
            Expression *current_next=current->next->value;
            // int fd=open(current_next->name, O_WRONLY | O_APPEND | O_CREAT , 0);
            // close(fd);

            // fp = freopen(current_next->name, "a", stdout);
            // fclose(fp);

            fp=fopen(current_next->name,"a");
            fclose(fp);

            output=current_next;
            // continue;

        }
        current=current->next;
    }
    if(output!=NULL){
        Expression * exp=first_cmd->value;

       // FILE * fp_in=fopen("temp","r");
        //struct stat sb;
        input_read("temp");


        char file_contents[1000];
        int num=read(STDIN_FILENO,file_contents,sizeof(file_contents));

        //fread(file_contents,sb.st_size,1,fp_in);

        exp->std_out=strdup(file_contents);
        // printf("%s", file_contents);
        //fclose(fp_in);

       if(remove("temp")){}

        char * test = strdup(output->name);
        only_append(test, file_contents, num);

        //FILE * fp_out = freopen(output->name,"a",stdout);
        //printf("%s \n", exp->std_out);
        //fclose(fp_out);

        //fflush(stdout);
        //fflush(stdin);
        // close(STDIN_FILENO);
        // close(STDOUT_FILENO);
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
      //  printf("Se va a hacer fork\n");
        pidC = fork();
      //  printf("Se hizo fork\n");

     //   printf("pid antes del fork: %d\n",pid);
        pid=pidC;//getpid();
     //   printf("pid despues del fork: %d\n",pid);
        did_ctrl_c=false;
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
            kill(pidC, SIGKILL);
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
    if(strcmp("^C",temp)==0)return;

    // if(temp==';')
    // {
    //     printf("syntax error near unexpected token `%s'\n",temp);
    // }
    if(SpecialCaracters(temp)&& !RedirCaracter(temp))////cuando como primera palabra tenemos un caracter especial que no puede ocupar ese lugar
    {
        printf("syntax error near unexpected token `%s'\n",temp);
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

  //  printf("%s:%d \n",exp1->name,exp1->operators);


    while(line->head!=NULL)
    {
        temp=line->head->value;
        //push_back(exp_line,(Expression*)malloc(sizeof(Expression)));
        //current=current->next;
        //current->value->name=temp;

        //Expression * exp = (Expression*)malloc(sizeof(Expression));//para crear cada expresion de la lista de expresiones
        char* name=strdup(temp);


        if(strcmp(";",name)==0)
        {
            free(pop_front(line));
            Execute(exp_line->head,exp_line->tail);
            if(line->head!=NULL) EjecuteLine(line);
            pid=getpid();
            //EjecuteLine(line);
            return;
        }

        bool special_caracter_now=SpecialCaracters(temp);//la expresion actual es un caracter especial
        if(special_caracter_last && special_caracter_now){printf("syntax error near unexpected token `%s'\n",temp); return;}
        if(if_caracter_last && special_caracter_now && !RedirCaracter(temp)){printf("syntax error near unexpected token `%s'\n",temp); return;}
        if(special_caracter_last || if_caracter_last)command=true;

        //exp->operators=GetOperator(temp);
        int op=GetOperator(temp);
        //if(exp->operators==SIMPLE_EXPRESSION)
        if(op==SIMPLE_EXPRESSION || PossibleArgumentExpression(op))////////lo que agregue esta solo aqui y en el metodo
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
       // printf("%s:%d\n",name,op);

        free(pop_front(line));

    }

    ////Descomentar esto desde aqui
    // int _pid =fork();
    // if(_pid==0)
    // {
    
    Execute(exp_line->head,exp_line->tail);

    pid=getpid();
    //char message[20];
    //read(STDOUT_FILENO, message, 20);
    //write(STDIN_FILENO, message, 20);
    //printf("%s \n",message);

    // }
    // else if(_pid>0)
    // {
    //     return;
    // }
    // else
    // {
    //     int *status=0;
    //     waitpid(_pid,status,0);

    // }
////Descomentar esto hasta aqui

    // fclose(stdin);
    // fclose(stdout);
    free_list(exp_line);
}

void ReadAndEjecuteLine(list* line,char* word, char c)//crea una lista de string por cada instruccion y luego manda a ejecutarlas
{
    bool history=true;
    if(c==' ') history=false;
    char *strline=(char*)calloc(sizeof(char),1000);
    if(history)ConcatChar(c,strline);

    while(c!= '\n' && c!= EOF)
    {

        //printf("Llego al while true\n");

        if(c==-1) exit(0);

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

            // if(c==';')
            // {
            //     // ////////Primero terminar de armar la linea actual, y mandarla a ejecutar
            //     // if(strcmp(word,"")==0 && line->head->next==NULL)
            //     // {
            //     //     printf("syntax error near unexpected token `;'\n");
            //     //     return;
            //     // }
            //     // //push_back(line, strdup(word));
            //     // //strcpy(word,"");
            //     // //pop_front(line);
            //     // //EjecuteLine(line);
            //     // //print_list(line);
            //     // //pop_back(line);
            //     // //free_list(line);
            //     // //line=init_list("init");

            //     // //c=GetOneChar(strline,history);
            //     // //c=GetOneChar();
            //     // //continue;
            //     // break;

            // }

            if(SpecialCaracter(c))
            {
                if(word[0]=='\0' ||SpecialCaracter(word[0]) )//Si la palabra que tengo hasta el
                //momento es vacia es porque se dejo un espacio y tengo que crear una nueva
                //para el caracter especial o en el caso que se este conforrmando un caracter especial,
                //lo termino de conformar.
                {
                    ConcatChar(c,word);
                    if(!SpecialCaracters(word) && strlen(word)!=1)
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
        if(history)SaveLine(strline);
        if(line->head!=NULL) EjecuteLine(line);

        //free_list(line);

///////////Llamar aqui al metodo para guardar history

    //printf("\n HISTORY: %s \n",strline);
}

void fflush_stdin() {
    char c;
    while((c = getchar()) != '\n' && c != EOF){}
}

void Shell()
{
    //signal(SIGINT,CtrlC);

printf("\n");
 while (1)
    {
        // int b = EOF;
        // printf("%d", b);
        
        int std_in=dup(STDIN_FILENO);
        int std_out=dup(STDOUT_FILENO);

        char * word= (char*)calloc(sizeof(char),100);//word es cada una de las palabras que se mandan en un espacio de line
        strcpy(word,"");

        list* line=init_list("init"); //line es la linked list que guarda los comandos argumentos y caracteres especiales
        //pop_front(line);

        // wait(NULL);
        //printf("Llego al inicio\n");
        if(pid_inicial==getpid()) printf("my-shell $ ");
        // fflush(stdin);
        

        char c;
        c = getchar();  //cada uno de los char a leer de consola

        if(c==-1) exit(0);
        // bool concat=true;
        // wait(NULL);


        ReadAndEjecuteLine(line,word,c);

        // fflush(stdout);

        dup2(std_in,STDIN_FILENO);
        dup2(std_out,STDOUT_FILENO);
        close(std_in);
        close(std_out);
        // close(STDIN_FILENO);
        // dup2(STDIN_FILENO, STDIN_FILENO);
        // char message[20];
        // read(STDIN_FILENO, message, 20);
        // printf("Buff: %s\n", message);
        // char cadena[20];
        // int a = scanf("%s", &cadena);

        // printf("%d", a);
        
        // fflush_stdin();

        //EndReadLine("",false);

    }

}

#pragma endregion
int main(int argc, char const *argv[])
{
    var_list = init_list("neverusethisname");
    getcwd(history_direction,200);
    strcat(history_direction,"/.history.txt");

    //printf("%s",history_direction);
    signal(SIGINT,CtrlC);

    //fork();
    pid=getpid();
    pid_inicial=getpid();
    did_ctrl_c=false;

    //pid=fork();
    //printf("PID inicial: %d\n",pid);
    //if(pid!=0) Shell();
    Shell();

    // Limpiando lista de variables
    free(var_list);
    return 0;
}


