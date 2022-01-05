// #include "using_execvp.h"
#define _GNU_SOURCE
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdbool.h> // Booleanos
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <search.h>


#ifndef MAX_BUF
#define MAX_BUF 200
#endif

char *buf; //Contiene la direccion del proyecto
int cantcomad;
int PidEject;
int Pid;
int Control;

#pragma region List
#define __ReservedMemoryforNode (node *)malloc(sizeof(node));
#define __SafeMemoryReserved(x) \
    if (x == NULL)              \
        exit(1);
#define __SafeMemoryFree(x) \
    if (x != NULL)          \
        free(x);

typedef struct node
{
    void *value;
    struct node *previous;
    struct node *next;
} node;

typedef struct linked_list
{
    node *head;
    node *tail;
    unsigned int size;
} linked_list;

linked_list *init_list(void *v)
{
    linked_list *l = (linked_list *)malloc(sizeof(linked_list));
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
void push_back(linked_list *l, void *v)
{
    node *insert = __ReservedMemoryforNode
        __SafeMemoryReserved(insert) if (l->size == 0)
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
void push_front(linked_list *l, void *v)
{
    node *insert = __ReservedMemoryforNode
        __SafeMemoryReserved(insert) if (l->size == 0)
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
node *pop_back(linked_list *l)
{
    if (l->size == 0)
        return NULL;
    node *last = l->tail;
    l->tail = l->tail->previous;
    if (l->tail != NULL)
    {
        l->tail->next = NULL;
    }
    l->size--;
    return last;
}
node *pop_front(linked_list *l)
{
    if (l->size == 0)
        return NULL;
    node *first = l->head;
    l->head = l->head->next;
    if (l->head != NULL)
    {
        l->head->previous = NULL;
    }
    l->size--;
    return first;
}

void removeNode(linked_list *l, node *item)
{
    if (l->size == 0)
        return;
    if (item->previous == NULL)
        pop_front(l);
    else if (item->next == NULL)
        pop_back(l);
    else
    {
        node *prev = item->previous;
        node *next = item->next;
        prev->next = next;
        next->previous = prev;
        l->size--;
    }
}

void free_list(linked_list *l)
{
    node *temp = l->head->next;
    node *current = l->head;
    while (current != NULL)
    {
        free(current);
        current = temp;
        if (temp != NULL)
        {
            temp = temp->next;
        }
    }
    free(l);
}

#pragma endregion

#pragma region Expression
enum OPERATORS
{
    op_true = 0,
    op_false = 1,
    op_simple_expression = 2,
    op_exit = 3,
    op_cd = 4,
    op_history = 5,
    op_again = 6,
    op_help = 7,
    op_controlc = 8,
    op_jobs = 9,
    op_fg = 10,
    op_get = 11,
    op_unset = 12,
    op_and = 13,
    op_or = 14,
    op_redir_less = 15,
    op_archive = 16,
    op_double_redir_big = 17,
    op_background = 18,
    op_set = 19,
    op_pipe = 20,
    op_if = 21,
    op_redir_big = 22,
    op_then = 23,
    op_else = 24,
    op_end = 25,
    op_semicolon = 26,
    op_args = 27,
    op_if_else = 28,
    op_invertcomin = 29
} OPERATORS;

typedef struct Command // TODO: Cambiar nombre a: Expression
{
    char *name;
    linked_list *args;
    char *std_in;
    char *std_out;
    bool calculated;
    enum OPERATORS operators;
} Command;

#pragma endregion
void ConcatChar(char c, char *chain);
linked_list *Transform(linked_list *);
char Waiting();
void Execute_Instruction(linked_list *);

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

void Run(node *com)
{
    // printf("Inside Run Method...\n");
    Command *com_to_exec = com->value;
    if (com_to_exec->operators != op_simple_expression)
        return;

    // list * temp_args = init_list(com->next);

    node *last = com->next;
    Command *last_com;

    if (last != NULL)
        last_com = last->value;
    else
        last_com = NULL;

    int count = 0;
    while (last_com != NULL && (last_com->operators == op_args || last_com->operators == op_archive))
    { // Con ARGS me refiero a los argumentos lo q aun no se bien como ponerle
        count++;
        last = last->next;
        if (last != NULL)
            last_com = last->value;
        else
            last_com = NULL;
    }
    // printf("Out of first while...\n");

    char *myargs[count + 2];
    myargs[count + 1] = NULL;
    myargs[0] = strdup(com_to_exec->name);

    int temp = 1; // Para ir ubicando argumenta a argumento en el arreglo
    node *current = com->next;
    while (temp < count + 1)
    {
        Command *current_com = current->value;
        myargs[temp] = strdup(current_com->name);
        temp++;
        current = current->next;
    }

    pid_t rc = fork();
    PidEject = rc;
    Control = 0;
    if (rc < 0)
    { // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    { // child (new process)

        Command *com_prev; // = com->previous;
        if (com->previous != NULL)
        {
            com_prev = com->previous->value;

            if (com_prev->operators == op_pipe)
            {
                if (com->previous->previous != NULL)
                {
                    com_prev = com->previous->previous->value;

                    if (com_prev->operators == op_exit && com_to_exec->std_in == NULL)
                    {
                        com_to_exec->std_in=strdup(com_prev->name);
                        //FILE * fp;
                        input_read(com_to_exec->std_in);
                        int e = execvp(myargs[0], myargs);

                        //fp = freopen(com_to_exec->std_in,"w", stdin);
                        //fclose(fp);


                        //fp = freopen(com_to_exec->std_in,"r", stdin);

                        // com_to_exec->std_in = strdup(com_prev->name);
                        // FILE *fp;
                        // fp = freopen(com_to_exec->std_in, "w", stdin);
                        // fclose(fp);

                        // fp = freopen(com_to_exec->std_in, "r", stdin);

                        // int e = execvp(myargs[0], myargs);
                        // fclose(fp);
                    }
                }
            }
        }

       // FILE *fp;
        if (com_to_exec->std_in != NULL)
        {
            input_read(com_to_exec->std_in);
            // printf("com_to_exec->std_in != NULL..\n");

            //fp = freopen(com_to_exec->std_in, "r", stdin);
        }
        if(com != NULL && com->next != NULL){

            node * current = com;
            Command * exp;

            while(current != NULL){
                exp = current->value;
                if(exp->operators == op_redir_big || exp->operators == op_double_redir_big){
                    FILE * fd=fopen("temp","w");
                    fclose(fd);
                    only_write("temp");
                    //fp_out = freopen("temp", "w", stdout);
                    break;
                }
                current = current->next;
            }
        }
        // char buf[10];
        // int num = read(STDIN_FILENO, buf, sizeof(buf));
        // printf("%d\n",num);
        int e = execvp(myargs[0], myargs);

        // if (com_to_exec->std_in != NULL)
        // {
        //     fclose(fp);
        // }
    }
    else
    { // parent goes down this path (main)
        int *status = 0;
        pid_t p = waitpid(rc, status, 0);
        kill(rc, SIGKILL);
    }
}

void handler(int sig, pid_t pid)
{
    kill(pid, sig);
}

int SIMPLE_Command_CODE(node *exp)
{
    Run(exp);
    return 0;
    // } else { // parent goes down this path (main)
}

// int AND_CODE(node * com){

// }
int TRUE_CODE(node *exp)
{
    return 0;
}

int FALSE_CODE(node *exp)
{
    return 1;
}

bool inside_pipe = false;

int EXIT_CODE(node *exp)
{
    // printf("Inside EXIT_CODE METHOD\n");
    if (inside_pipe)
    {

        return 0;
    }
    exit(0);
    return 0;
}

int SET_CODE(node *exp)
{
    // printf("Inside Run Method...\n");
    Command *com_to_exec = exp->value;
    if (com_to_exec->operators != op_set)
        return -1;

    Command *var_name = exp->next->value;

    node *last = exp->next->next;
    Command *last_exp;

    if (last != NULL)
        last_exp = last->value;
    else
        last_exp = NULL;

    int count = 0;

    return 0;
}

int CD(node *n) //cd
{
    if (n->next == NULL)
    {
        if (chdir(getenv("HOME")) == -1)
        {
            perror("cd");
            return -1;
        }
    }
    Command *next = (Command *)calloc(100, sizeof(Command));
    next = n->next->value;
    if (next->operators == op_args)
    {
        if (chdir(next->name) == -1)
        {
            perror("cd");
            return -1;
        }
    }
    else
    {
        perror("Not Found");
        return -1;
    }

    return 0;
}

int Help(node *n) //Help
{
    FILE *fd;
    //char *dir = (char *)calloc(100, sizeof(char));
    char dir[1000] = "";
    strcat(dir, buf);

    if (n->next == NULL)
    {
        strcat(dir, "/Help");
        fd = fopen(dir, "r");
    }
    else
    {
        Command *arg = (Command *)malloc(100 * sizeof(Command));
        arg = n->next->value;

        if (arg->operators != op_args)
            return -1;

        if (strcmp(arg->name, "basic") == 0)
        {
            strcat(dir, "/Help_Basic");
            fd = fopen(dir, "r");
        }
        else if (strcmp(arg->name, "multi-pipe") == 0)
        {
            strcat(dir, "/Help_MultiPipe");
            fd = fopen(dir, "r");
        }
        else if (strcmp(arg->name, "background") == 0)
        {
            strcat(dir, "/Help_Background");
            fd = fopen(dir, "r");
        }
        else if (strcmp(arg->name, "history") == 0)
        {
            strcat(dir, "/Help_History");
            fd = fopen(dir, "r");
        }
        else if (strcmp(arg->name, "control+c") == 0)
        {
            strcat(dir, "/Help_Control");
            fd = fopen(dir, "r");
        }
        else if (strcmp(arg->name, "chain") == 0)
        {
            strcat(dir, "/Help_Chain");
            fd = fopen(dir, "r");
        }
        else if (strcmp(arg->name, "if") == 0 || strcmp(arg->name, "multi-if") == 0)
        {
            strcat(dir, "/Help_IF");
            fd = fopen(dir, "r");
        }
        else if (strcmp(arg->name, "variables") == 0)
        {
            strcat(dir, "/Help_Var");
            fd = fopen(dir, "r");
        }
    }
    int c;
    if (fd == NULL)
    {
        perror("Error");
        return -1;
    }
    do
    {
        c = fgetc(fd);
        if (feof(fd))
            break;
        printf("%c", c);
    } while (1);

    fclose(fd);
    // free(dir);
    return 0;
}

void SaveHistory(char *comand, int *cantcomand) //modificar la cantcomand ,no es necesario pasarlo
{
    FILE *fichero;
    FILE *temp;

    char *dir = (char *)calloc(100, sizeof(char));
    strcat(dir, buf);
    strcat(dir, "/History");
    char *dirt = (char *)calloc(100, sizeof(char));
    strcat(dirt, buf);
    strcat(dirt, "/Temp");
    int n = 0;

    if (*cantcomand == 10) //Si ya hay 10 elemento hay que quitar el primero
    {
        fichero = fopen(dir, "r");
        if (fichero == NULL) //verificar si el fichero existe
        {
            perror("\nError opening file");
            //exit(EXIT_FAILURE);
        }

        temp = fopen(dirt, "a");
        char comand[100];
        while (fscanf(fichero, "%99s", comand) != EOF) //lee
        {
            if (n > 0)
                fprintf(temp, "%s \n", comand); //escribe
            n++;
        }

        fclose(temp);
        fclose(fichero);

        int j = remove(dir);
        int s = rename(dirt, dirt);
    }
    //En cualquier otro caso se agrega al txt
    fichero = fopen(dir, "a");
    fprintf(fichero, "%s \n", comand);
    fclose(fichero);
    *cantcomand = *cantcomand + 1;
}

int ShowHistory(node *nodo)
{
    //Command *dir = (Command *)malloc(sizeof(Command));
    // Command *arg = (Command *)malloc(sizeof(Command));
    // dir = nodo->value;
    //arg = nodo->next->value;

    //if (strcmp(dir->name, "history") != 0 || arg->operators == op_args)
    //  return -1;

    char *dirf = (char *)calloc(100, sizeof(char));
    strcat(dirf, buf);
    strcat(dirf, "/History");

    int n = 1;
    FILE *fichero;
    fichero = fopen(dirf, "r");

    if (fichero == NULL)
    {
        perror("Error opening file");
        return -1;
    }
    char comand[100];
    while (fscanf(fichero, "%99s", comand) != EOF)
    {
        printf("%d : %s \n", n, comand);
        n++;
    }

    fclose(fichero);
    return 0;
}

int Again(node *nodo) //***Falta codigo
{
    Command *p = (Command *)malloc(sizeof(Command));
    p = nodo->next->value;
    char *number = p->name;
    char *dir = (char *)calloc(100, sizeof(char));
    strcat(dir, buf);
    strcat(dir, "/History");

    if (p->operators != op_args)
        return -1;
    int n = atoi(number);
    if (n > cantcomad || n == 0)
    {
        perror("Not Found");
        return -1;
    }

    int i = 1;
    char comand[100];
    FILE *fichero;
    fichero = fopen(dir, "r");
    while (i <= n && fscanf(fichero, "%99s", comand) != EOF)
    {
        i++;
    }
    //Poner el m\'etodo de ejecutar el comando
    //Command *com = (Command *)malloc(sizeof(Command));
    //com->name = comand;
    //a = init_list(com);
    //node *n = (node *)malloc(sizeof(node));

    return 0;
}

void ControlC()
{
    if (Control)
    {
        if (PidEject != Pid)
            kill(PidEject, SIGKILL);
    }
    else
    {
        Control = 1;
    }
}

int (*testing[])(node *) = {
    TRUE_CODE,
    FALSE_CODE,
    SIMPLE_Command_CODE,
    EXIT_CODE,
    CD,
    ShowHistory,
    Again,
    Help,
    //Control,
    //SET_CODE,
    // GET_CODE,
    // UNSET_CODE,
    0};

node *Search_AND_OR(node *first_cmd, node *last_cmd)
{

    bool is_if = false;
    node *current = first_cmd;
    Command *current_temp = current->value;
    while (current != last_cmd)
    {
        current_temp = current->value;
        if (current_temp->operators == op_if)
        {
            is_if = true;
            //continue;
        }
        else if (current_temp->operators == op_end)
        {
            is_if = false;
            //continue;
        }
        else if ((current_temp->operators == op_and || current_temp->operators == op_or) && !is_if)
        {
            // free(current_temp);
            return current;
        }

        current = current->next;
    }

    return NULL;
}

node *Search_PIPE(node *first_cmd, node *last_cmd)
{
    // printf("In Search PIPE method...\n");
    bool is_if = false;
    node *current = first_cmd;
    Command *current_temp; // = current->value;
    while (current != last_cmd)
    {
        // printf("In while searching pipes...\n");
        current_temp = current->value;
        // current_temp->operators
        if (current_temp->operators == op_if)
        {
            is_if = true;
            //continue;
        }
        else if (current_temp->operators == op_end)
        {
            is_if = false;
            //continue;
        }
        else if (current_temp->operators == op_pipe && !is_if)
        {
            // free(current_temp);
            return current;
        }

        // printf("Changing current variable...\n");
        current = current->next;
        // if(current == NULL)
        //     printf("current is NULL");
        // printf("current variable changed...\n");
        // Command * temp = current->value;
        // printf("current->value: %s", temp->name);
        // free(temp);
    }
    // if(current_temp != NULL)
    // free(current_temp);
    // if(current != NULL)
    // free(current);
    return NULL;
}

node *Search_IF_THEN_ELSE(node *first_cmd, node *last_cmd, enum OPERATORS operator)
{
    // printf("I'm in IF THEN ELSE method...\n");
    node *current = first_cmd;
    Command *current_temp = current->value;
    while (current != NULL)
    {
        current_temp = current->value;
        if (current_temp->operators == operator)
        {
            return current;
        }
        current = current->next;
    }
    // free(current);
    // free(current_temp);
    // printf("op_end of Search IF_THEN_ELSE method...\n");
    return NULL;
}

// Leaves
int GetIndex(Command *op)
{
    return (int)op->operators;
}

int ExecuteCommand(node *cmd_to_exec)
{
    Command *node_com = cmd_to_exec->value;
    // enum OPERATORS op = node_com->operators;
    // printf("Getting index_method...\n");
    // printf("%d", node_com->operators);
    int index = node_com->operators;
    // printf("Index: %d \n", index);
    return testing[index](cmd_to_exec);
    // free(node_com);
}

int SolveCommands(node *first_cmd, node *last_cmd)
{
    node *current = first_cmd;

    // printf("Start ExecuteCommand method\n");

    return ExecuteCommand(current);
}

int SolveBiggerRedir(node *first_cmd, node *last_cmd, int exp_out)
{
    Command *output = NULL;
    node *current = first_cmd;
    FILE *fp;
    bool redir_found = false;

    Command *current_exp;
    while (current != NULL)
    {
        current_exp = current->value;
        if (current_exp->operators == op_redir_big)
        {
            Command *current_next = current->next->value;
            fp=fopen(current_next->name,"w");
            //fp = freopen(current_next->name, "w", stdout);
            fclose(fp);
            output = current_next;

            // continue;
        }
        else if (current_exp->operators == op_double_redir_big)
        {
            Command *current_next = current->next->value;
            //fp = freopen(current_next->name, "a", stdout);
            fp=fopen(current_next->name,"a");
            fclose(fp);
            output = current_next;
            // continue;
        }
        current = current->next;
    }
    if (output != NULL)
    {
        Command *exp = first_cmd->value;

        input_read("temp");
        //fp = freopen(output->name, "a", stdout);
        //printf("%s \n", exp->std_out);
        //fclose(fp);
        char file_contents[1000];
        int num=read(STDIN_FILENO,file_contents,sizeof(file_contents));

        exp->std_out=strdup(file_contents);

        remove("temp");

        

        char * test = strdup(output->name);
        only_append(test, file_contents, num);

        // exp->std_out="1";
        redir_found = true;

        
    }

    if (redir_found == false)
        return exp_out;
    return 0;
}

node *SearchBiggerRedir(node *last_cmd)
{
    node *current = last_cmd;
    Command *current_temp = current->value;

    while (current != NULL && &current != &last_cmd)
    {
        if (current_temp->operators == op_redir_big)
        {
            return current;
        }

        current = current->next;
    }
    return NULL;
}

node *SolveLessRedir(node *first_cmd, node *last_cmd)
{
    node *current = last_cmd;
    node *input = NULL;
    //FILE * fp;
    Command *current_exp;

    // printf("Llegamos a < wiiiiiii!!!!\n");
    while (current != NULL)
    {
        current_exp = current->value;
        if (current_exp->operators == op_redir_less)
        {
            // printf("Inside current REDIRLESS if...\n");
            // printf("current_exp.name = %d", current_exp->operators);
            input = current->next;
            break;
        }
        current = current->previous;
    }
    // if(current_exp != NULL)
    //     printf("%s\n", current_exp->name);
    // else
    //     printf("current_exp = NULL");

    if (input != NULL)
    {
        // printf("Input was not equal NULL...\n");
        Command *input_exp = input->value;
        // fp=fopen(input_exp->name,"r");
        // struct stat sb;
        Command *fisrt_exp = first_cmd->value;
        // if(stat(input_exp->name,&sb)==-1)
        // {
        //     fisrt_exp->std_out=0;
        //     return first_cmd;
        // }
        // char * file_contents=malloc(sb.st_size);
        //fread(file_contents,sb.st_size,1,fp);
        fisrt_exp->std_in = input_exp->name;
        // printf("%s", file_contents);
        // fclose(fp);
        // printf("%s \n", fisrt_exp->std_in);
    }
    // printf("op_end of Less Redir...\n");
    return first_cmd;
}

int Solve_Leaves(node *first_cmd, node *last_cmd)
{
    SolveLessRedir(first_cmd, last_cmd);
    // printf("Starting to solve Commands...\n");
    int exp_out = SolveCommands(first_cmd, last_cmd);
    return SolveBiggerRedir(first_cmd, last_cmd, exp_out);
}


// Leaves
#define READ_op_end 0
#define WRITE_op_end 1

int Execute(node *first_cmd, node *last_cmd)
{
    if (first_cmd == last_cmd)
    {
        return Solve_Leaves(first_cmd, last_cmd);
    }

    // printf("Into Execute Method...\n");

    // AND OR
    node *AND_OR = Search_AND_OR(first_cmd, last_cmd); // Buscando And u Or (&& u ||) sin que se encuentren dentro de un if
    Command *AND_OR_com;
    if (AND_OR != NULL)
    {
        AND_OR_com = AND_OR->value;

        int output = Execute(first_cmd, AND_OR->previous);
        // printf("Out of left &&...\n");
        // wait(NULL);
        //Command * output_com = output->value;
        if (AND_OR_com->operators == op_and)
        {
            if (output == 0)
            { // Si && retorna true devuelve el codigo a la derecha
                return Execute(AND_OR->next, last_cmd);
            }
            return output;
        }
        else if (AND_OR_com->operators == op_or)
        {
            if (output != 0) // Si || retorna false devuelve el codigo a la derecha
                return Execute(AND_OR->next, last_cmd);
            return output;
        }
    }

    // printf("Start searching Pipes...\n");
    // Pipes
    node *PIPE_node = Search_PIPE(first_cmd, last_cmd);
    // printf("Out of found PIPE method...\n");
    Command *PIPE_node_com;
    //if(PIPE_node != NULL)

    if (PIPE_node != NULL)
    { // 3 casos
        PIPE_node_com = PIPE_node->value;

        int fd[2];
        pid_t pidC;
        int *status = 0;
        // char buf[10];
        // int num;
        int output = 0;

        inside_pipe = true;
        pipe(fd);
        pidC = fork();
        PidEject = pidC;
        Control = 0;
        if (pidC == 0)
        {
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            // printf("I'm in execute 1...\n");
            Execute(first_cmd, PIPE_node->previous);
            // execvp(myargs[0], myargs);
            // exit(0);
            // break;
        }
        else if (pidC == -1)
        {
            // break;
            printf("Error\n");
        }
        else
        {

            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            // num = read(fd[0], buf, sizeof(buf));
            // printf("%d\n",num);
            close(fd[0]);

            // printf("I'm in execute 2...\n");
            output = Execute(PIPE_node->next, last_cmd);
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

        Command *com_prev; // = com->previous;
        if (PIPE_node->previous != NULL)
        {
            com_prev = PIPE_node->previous->value;

            if (com_prev->operators == op_exit)
            {
                if (remove("exit"))
                {
                }
            }
        }
        // printf("found a Pipe...\n");
        return output;
    }

    node *IF_ELSE_node = Search_IF_THEN_ELSE(first_cmd, last_cmd, op_if);
    if (IF_ELSE_node != NULL)
    { // 2 casos
        //Buscando para IF_ELSE o simplemente THEN

        node *THEN_node = Search_IF_THEN_ELSE(IF_ELSE_node, last_cmd, op_then);
        node *op_end_node = Search_IF_THEN_ELSE(THEN_node, last_cmd, op_end);
        node *ELSE_node = Search_IF_THEN_ELSE(THEN_node, last_cmd, op_else);

        //Caso 1 para IF ELSE
        int IF_output = Execute(IF_ELSE_node->next, THEN_node->previous); // Aca se supone que se modifique la variable first_cmd, para que luego apunte hacia la instruccion siguiente al
        if (IF_output == 0)
        { // Si devuelve True es xq tiene q ir hacia el THEN
            // Ejecuta el Then
            return Execute(THEN_node->next, ELSE_node->previous);
        }
        else if (ELSE_node != NULL)
        {
            // Ejecuta el Else
            return Execute(ELSE_node->next, op_end_node->previous);
        }
        return 0;
    }
    // if(IF_ELSE_node != NULL)
    //     free(IF_ELSE_node);

    // printf("Starting to solve leaves...\n");

    // Leaves
    return Solve_Leaves(first_cmd, last_cmd);
}
void Execute_Instruction(linked_list * line)
{
    //Command * com;
    node * first=line->head;
    bool state=false;

    node * current=line->head;
    while(current!=NULL)
    {
        state=false;
        Command * current_com=current->value;
        if(current_com->operators==op_semicolon)
        {
            state=true;
            Execute(first,current->previous);
            first=current->next;
        }
        current=current->next;
    }
    if(!state)
    {
        Execute(first,line->tail);
    }
}
void ConcatChar(char c, char *chain)
{
    char temp[2];
    temp[0] = c;
    temp[1] = '\0';
    strcat(chain, temp);
}
char Waiting()
{
    char c = '\0';
    while (c != '\n')
    {
        c = getchar();
    }
    return c;
}
linked_list *Transform(linked_list *line)
{
    linked_list *commands_op = (linked_list *)malloc(sizeof(linked_list));
    char *str_anterior = NULL;
    char *str_actual = NULL;

    char *operadores[26] = {
        "&",
        "&&",
        "||",
        "|",
        ">",
        ">>",
        "<",
        "false",
        "if",
        "else",
        "end",
        "then",
        "true",
        "exit",
        ";",
        "cd",
        "history",
        "again",
        "help",
        "control+c",
        "jobs",
        "fg",
        "set",
        "get",
        "unset",
        "``"};
    enum OPERATORS enum_oper[26] = {op_background, op_and, op_or, op_pipe, op_redir_big, op_double_redir_big, op_redir_less, op_false, op_if, op_else, op_end, op_then,
                                    op_true, op_exit, op_semicolon, op_cd, op_history, op_again, op_help, op_controlc, op_jobs, op_fg, op_set, op_get, op_unset, op_invertcomin};

    node *expresion = (node *)calloc(1,sizeof(node));
    expresion->previous = NULL;
    commands_op->head = expresion;

    node *cadena = line->head;
    node *previous;
    int count=line->size;
    while (cadena!=NULL)
    {
        str_anterior = str_actual;
        str_actual = cadena->value;

        bool state = false;

        Command *command = (Command *)calloc(1,sizeof(Command));
        expresion->value = command;
        //command=expresion->value;
        command->name = (char *)malloc(sizeof(char));
        command->name = str_actual;

        if (strlen(str_actual) == 0)
        {
            cadena = cadena->next;
        }
        else
        {

            for (int i = 0; i < 26; i++)
            {
                if (strcmp(str_actual, operadores[i]) == 0)
                {
                    command->operators = enum_oper[i];
                    cadena = cadena->next;
                    state = true;
                    break;
                }
            }
            if (!state)
            {
                if (str_anterior == NULL)
                {
                    command->operators = op_simple_expression;
                    cadena = cadena->next;
                }
                else
                {
                    Command *temp = previous->value;
                    if (temp->operators == op_args || temp->operators == op_simple_expression || temp->operators == op_cd || temp->operators == op_help || temp->operators == op_again)
                    {
                        command->operators = op_args;
                        cadena = cadena->next;
                    }
                    else if (temp->operators == op_redir_big || temp->operators == op_redir_less || temp->operators == op_double_redir_big)
                    {
                        command->operators = op_archive;
                        cadena = cadena->next;
                    }
                    else if (temp->operators == op_and || temp->operators == op_or || temp->operators == op_pipe || temp->operators == op_if || temp->operators == op_then || temp->operators == op_else || temp->operators == op_end || temp->operators == op_semicolon)
                    {
                        command->operators = op_simple_expression;
                        cadena = cadena->next;
                    }
                }
            }
            previous = expresion;
            expresion->next = (node *)calloc(1,sizeof(node));
            expresion = expresion->next;
            expresion->previous = previous;
        }
        //count--;
    }
    commands_op->tail = expresion->previous;
    expresion = expresion->previous;
    free(expresion->next);
    expresion->next = NULL;
    return commands_op;
}

int main()
{
    PidEject = getpid();
    Pid = getpid();
    Control = 0;
    signal(SIGINT, ControlC);
    buf = get_current_dir_name();
    if (buf == NULL)
    {
        perror("get_current_dir_name");
        return -1;
    }
    printf("Current working directory: %s\n", buf);

    while (1)
    { //FALTA LO DEL ;
        int stdin_in=dup(STDIN_FILENO);
        int stdin_out=dup(STDOUT_FILENO);

        linked_list *cadena_ = (linked_list *)malloc(sizeof(linked_list));
        cadena_->head=(node *)calloc(1,sizeof(node));
       // cadena_->head = (node *)malloc(sizeof(node));
        cadena_->head->value = (char *)calloc(100,sizeof(char));
        //cadena_->head->value="";
        cadena_->size=0;
        node *cadena = cadena_->head;
        printf("\n my_shell:) $ ");
        char c;
        char anterior;
        do
        {
            anterior = c;
            c = getchar();
            if (c == '#')
            {
                c = Waiting();
            }
            else
            {
                if ((c == '&' || c == '|' || c == '<' || c == '>' || c == ';') && strlen(cadena->value) > 1)
                {
                    cadena->next = (node *)calloc(1,sizeof(node));
                    cadena = cadena->next;
                    cadena->value = (char *)calloc(100,sizeof(char));
                    //cadena->value="";
                    //cadena_->size+=1;
                    
                }
                if ((c == '&' && anterior == '&') || (c == '|' && anterior == '|') || (c == '>' && anterior == '>'))
                {
                    ConcatChar(c, cadena->value);
                    cadena->next = (node *)calloc(1,sizeof(node));
                    cadena = cadena->next;
                    cadena->value = (char *)calloc(100,sizeof(char));
                    //cadena->value="";
                    //cadena_->size+=1;
                }
                else if (c != ' ' && (((anterior == '&' || anterior == '|' || anterior == '>' || anterior == '<' || anterior == ';') && strlen(cadena->value) != 0) || ((c == '>' || c == '|' || c == '<' || c == '&' || c == ';') && strlen(cadena->value) > 0)))
                {
                    cadena->next = (node *)calloc(1,sizeof(node));
                    cadena = cadena->next;
                    cadena->value = (char *)calloc(100,sizeof(char));
                    //cadena->value="";
                    ConcatChar(c, cadena->value);
                    //cadena_->size+=1;
                }
                else if (c == ' ')
                {
                    cadena->next = (node *)calloc(1,sizeof(node));
                    cadena = cadena->next;
                    cadena->value = (char *)calloc(100,sizeof(char));
                    //cadena->value="";
                    cadena_->size+=1;
                }
                else if(c=='\n')
                {
                    break;
                }
                else
                {
                    ConcatChar(c, cadena->value);
                }
            }

        } while (c != '\n' && c!=EOF);

        node *node_anterior = NULL;
        node *node_actual = cadena_->head;
        //int count=cadena_->size;

        while (node_actual!=NULL)
        {

            if (node_actual->value!=NULL && strlen(node_actual->value) == 0)
            {
                if (node_anterior != NULL)
                {
                    node_anterior->next = node_actual->next;
                    node_actual = node_anterior;
                }
                else
                {
                    node_actual = node_actual->next;
                }
            }
            node_anterior = node_actual;
            node_actual = node_actual->next;
            //count--;
        }
        cadena_->tail = node_anterior;
        // char *last = (char *)malloc(sizeof(char));
        // last = strncat(last, cadena_->tail->value, strlen(cadena_->tail->value) - 1);
        //cadena_->tail->value = last;

        linked_list *commands_op = Transform(cadena_);
        Execute_Instruction(commands_op);
        //Execute(commands_op->head, commands_op->tail);

        dup2(stdin_in,STDIN_FILENO);
        dup2(stdin_out,STDOUT_FILENO);
        close(stdin_in);
        close(stdin_out);

        free_list(cadena_);
       // free(cadena);
       // free_list(commands_op);
        ///free(last);

        
    }
}
