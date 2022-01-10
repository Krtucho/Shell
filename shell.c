//#include "help.h"
#include "input_output.h"
#include<signal.h>
#include "tools.h"





#pragma region Comandos Especiales

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
        return 1;
    }
    return 0;
}

bool did_ctrl_c;
int pid;
int pid_inicial;
void CtrlC()
{
    if(did_ctrl_c)
    {
        did_ctrl_c=false;

        if(pid!=pid_inicial) kill(pid,9);
    }
    else
    {
        did_ctrl_c=true;
        if(pid==pid_inicial) 
        {
            did_ctrl_c=false;

        }

    }
}

#pragma region History

char history_direction[200];
char again_direction[200];
bool did_again;

int StrToInt(char* n)
{
    if(strcmp(n,"1")==0)return 1;
    if(strcmp(n,"2")==0)return 2;
    if(strcmp(n,"3")==0)return 3;
    if(strcmp(n,"4")==0)return 4;
    if(strcmp(n,"5")==0)return 5;
    if(strcmp(n,"6")==0)return 6;
    if(strcmp(n,"7")==0)return 7;
    if(strcmp(n,"8")==0)return 8;
    if(strcmp(n,"9")==0)return 9;
    if(strcmp(n,"10")==0)return 10;
    return -1;
}

char* LineListToCharArray(list*strlist)
{
    char* line=calloc(sizeof(char),1000);
    node* current=strlist->head;
    while(current!=NULL)
    {
        strcat(line,strdup(current->value));
        strcat(line," ");
        current=current->next;
    }
    strcat(line,"\n");
    return line;
}

void CharArrayToLineList(list*strlist,char* line)
{
    char c=line[0];
    int i=0;
    char * word= (char*)calloc(sizeof(char),100);
    strcpy(word,"");
    while(c!='\n')
    {
        if(c==' ')
        {
            push_back(strlist,strdup(word));
            strcpy(word,"");            
        }
        else ConcatChar(c,word);
        i++;
        c=line[i];
    }
    free(word);

}

int only_write(char * file);
int only_append(char * file, char buf[1000], int num);
int input_read(char * file);

int HISTORY_CODE(node* argument)
{

    int std_in=dup(STDIN_FILENO);
    int std_out=dup(STDOUT_FILENO);
    if(!input_read(history_direction))
    {
        char file_contents[1000];
        int num=read(STDIN_FILENO,file_contents,sizeof(file_contents));
        printf("%s",file_contents);

        dup2(std_in,STDIN_FILENO);
        dup2(std_out,STDOUT_FILENO);
        close(std_in);
        close(std_out);

        return 0;
    }
    dup2(std_in,STDIN_FILENO);
    dup2(std_out,STDOUT_FILENO);
    close(std_in);
    close(std_out);
    return 1;

}

int ReadHistory(list* history_lines,char* file_contents,int fc_size)
{
    int count=0;
    char* line=calloc(sizeof(char),1000);

    for (int i = 0; i <= fc_size; i++)
    {
        if(file_contents[i]=='\n')
        {
            ConcatChar(file_contents[i],line);
            push_back(history_lines,strdup(line));
            strcpy(line,"");
            count++;
            continue;
        }
        ConcatChar(file_contents[i],line);

    }
    free(line);
    return count;
    
}


void EjecuteLine(list* line);


void SaveLine(char* strline, list*strlist)
{
    int std_in=dup(STDIN_FILENO);
    int std_out=dup(STDOUT_FILENO);

    input_read(history_direction);
    char file_contents_history[1000];
    int num_history=read(STDIN_FILENO,file_contents_history,sizeof(file_contents_history));


    input_read(again_direction);
    char file_contents_again[1000];
    int num_again=read(STDIN_FILENO,file_contents_again,sizeof(file_contents_again));


    list*history_lines=init_list("init");
    list*again_lines=init_list("init");

    int count_history=ReadHistory(history_lines,file_contents_history,num_history);    
    int count_again=ReadHistory(again_lines,file_contents_again,num_again);    

    pop_front(history_lines);
    pop_front(again_lines);


    if(strcmp(history_lines->tail->value,strline)!=0)
    {
        push_back(history_lines,strdup(strline));  

        char*tempstrline=LineListToCharArray(strlist);
        push_back(again_lines,strdup(tempstrline));           

        free(tempstrline);
        count_history++;
        count_again++;

    }

    if(count_history>=11) 
    {
        free(pop_front(history_lines));
        free(pop_front(again_lines));
        count_again--;
        count_history--;
    }
    

    char* history_output=calloc(sizeof(char),1000);
    char* again_output=calloc(sizeof(char),1000);

    while(count_history>0)
    {
        strcat(history_output,history_lines->head->value);
        strcat(again_output,again_lines->head->value);

        free(pop_front(again_lines));
        free(pop_front(history_lines));

        count_history--;
    }
    

    FILE * fp_history = fopen(history_direction, "w");
    fclose(fp_history);        
    only_write(history_direction);
    printf("%s",strdup(history_output));

    FILE * fp_again = fopen(again_direction, "w");
    fclose(fp_again);        
    only_write(again_direction);
    printf("%s",strdup(again_output));

    free(history_lines);
    free(history_output);


    free(again_lines);
    free(again_output);

    dup2(std_in,STDIN_FILENO);
    dup2(std_out,STDOUT_FILENO);
    close(std_in);
    close(std_out);
}


int AGAIN_CODE(node*argument)
{
     if(argument->next==NULL)
    {
        printf("empty again argument\n");
        return 1;
    }
    Expression* index=argument->next->value;
    int index_int=StrToInt(index->name);
    did_again=true;

    if(index_int==-1)
    {   
        printf("invalid again argument :%s\n",index->name);
        return 1;
    }

     int std_in=dup(STDIN_FILENO);
     int std_out=dup(STDOUT_FILENO);

    input_read(again_direction);
    char file_contents_again[1000];
    int num_again=read(STDIN_FILENO,file_contents_again,sizeof(file_contents_again));

    input_read(history_direction);
    char file_contents_history[1000];
    int num_history=read(STDIN_FILENO,file_contents_history,sizeof(file_contents_history));



    list* history_lines = init_list("init");
    list* again_lines = init_list("init");
    int count=ReadHistory(history_lines,file_contents_history,num_history);    
    count=ReadHistory(again_lines,file_contents_again,num_again);    

    pop_front(history_lines);
    pop_front(again_lines);


    if(index_int>count)
    {
        printf("again's argument is bigger than history's commands saved: %d \n",count);
        return 1;
    }

    node* current_again = again_lines->head;
    node* current_history = history_lines->head;

    for (int i = 1; i <index_int; i++)
    {
        current_again=current_again->next;
        current_history=current_history->next;

    }
    

    list* strlist=init_list("init");
    CharArrayToLineList(strlist,strdup(current_again->value));
    free(pop_front(strlist));

    SaveLine(strdup(current_history->value),strlist);
    if(strlist->head!=NULL) EjecuteLine(strlist);


    dup2(std_in,STDIN_FILENO);
    dup2(std_out,STDOUT_FILENO);
    close(std_in);
    close(std_out);

    free_list(again_lines);
    return 0;
}


#pragma endregion


#pragma endregion

#pragma region Ejecucion


int Run(node * com){ // Correr comandos simples, hace un fork y el hijo llama a execvp, en caso de ser exit y que existan pipes, hacemos exit(0) desde el hijo y cerramos el fork
    Expression * com_to_exec = com->value; // quedandonos con el value(Expression del primer nodo), debe de ser un comando a ejecutar, o algun comando que no exista
    if(com_to_exec->operators != SIMPLE_EXPRESSION)
        return -1;

    node * last = com->next; // Pedimos el nodo siguiente, para luego utilizarlo para buscar el ultimo argumento del comando a ejecutar
    Expression * last_com; // Nos quedamos con el valor de tipo Expression del nodo que pedimos anteriormente

    if(last != NULL)
        last_com = last->value;
    else
        last_com = NULL;

    int count = 0;  // Contador para saber cuantos argumentos tenemos
    while(last_com != NULL && (last_com->operators == ARGS || last_com->operators== ARCHIVE)){// Comparamos en todo momento que sea de tipo ARGS o ARCHIVO para no equivocarnos y pasarle argumentos de algun tipo incorrecto
        count++;
        last = last->next;
        if(last != NULL)
            last_com = last->value;
        else
            last_com = NULL;
    }

    char *myargs[count+2]; // Arreglo con los argumentos para llamar al execvp
    myargs[count+1] = NULL; // Seteamos el ultimo valor del arreglo en Null para q sepa hasta donde tiene q leer
    myargs[0] = strdup(com_to_exec->name); // En la primera posicion colocamos el nombre del comando a ejecutar

    int temp = 1; // Para ir ubicando argumenta a argumento en el arreglo
    node * current = com->next;
    while (temp < count+1)
    {
        Expression * current_com = current->value;
        myargs[temp] = strdup(current_com->name);
        temp++;
        current=current->next;
    }

    pid_t rc = fork(); // Hago fork

    pid=rc;

    did_ctrl_c=false;

    if (rc < 0) { // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) { // child (new process)

        // Aca verificamos el caso especial en que tengamos EXIT | WC u otro comando
        Expression * com_prev; // Expression del nodo anterior
        if(com->previous != NULL){ // Si el nodo actual tiene alguno antes de el, lo cogemos
            com_prev = com->previous->value;

            if(com_prev->operators == PIPE){ // Si este fue un PIPE nos quedamos con el anterior a el y luego comparamos con que sea un EXIT
                if(com->previous->previous != NULL){
                    com_prev = com->previous->previous->value;

                    if(com_prev->operators == EXIT && com_to_exec->std_in==NULL){ // De ser un EXIT sabemos que tenemos que cerrar el fork del pipe pero no el proceso principal
                        com_to_exec->std_in=strdup(com_prev->name); // Guardamos en el std_in del comando a ejecutar el nombre: exit para luego abrir un archivo con este nombre y guardar la salida del exit

                        input_read(com_to_exec->std_in); // Leemos del archivo que se va a crear vacio y enviamos su contenido al stdin

                        int e = execvp(myargs[0], myargs); // Ejecutamos el comando pasandole como entrada la salida del archivo vacio (null)
                        if(e == -1){ // Si el comando no existe, se devolvera una salida no satisfactoria
                            printf("%s: command not found\n", myargs[0]);
                            exit(-1);
                        }
                    }
                }
            }
        }

        if(com_to_exec->std_in != NULL){ // Si el comando a ejecutar tiene que leer de algun archivo, el nombre del cual esta guardado en el std_in(operador <), x
            input_read(com_to_exec->std_in); // entonces lo abrimos y enviamos su contenido al stdin
        }

        // Caso en que sea necesario escribir la salida del comando en un archivo
        if(com != NULL && com->next != NULL){
            node * current = com;
            Expression * exp;

            while(current != NULL){
                exp = current->value;
                if(exp->operators == REDIRBIG || exp->operators == DOUBLEREDIRBIG){
                    FILE * fp = fopen("temp", "w"); // Abrimos temp vacio para luego escribir contenido en el
                    fclose(fp);
                    only_write("temp"); // Todo el contenido que salga por el stdout sera redirigido al archivo temp
                    break;
                }
                current = current->next;
            }
        }

        int e = execvp(myargs[0], myargs); // Ejecutamos el comando y todo su contenido que saldra por el stdout estara siendo redirigido al archivo temp

        if(e == -1){ // En caso de no encontrarse el comando se devuelve una salida no satisfactoria
            printf("%s: command not found\n", myargs[0]);
            exit(-1);
        }
    }
    else { // parent goes down this path (main)
        int status; // Estado de la salida de la ejecucion del comando

        pid_t p=waitpid(rc,&status,0); // esperamos por el hijo a que este termine de ejecutar el comando

        kill(rc, SIGKILL); // Matamos al hijo para que no quede en estado zombie
        return status; // Retornamos el valor de la salida de la ejecucion del comando, si se encontro y se ejecuto se devolvera 0, en caso contrario se devolvera algun numero distinto de 0 que describa el error
    }
}

int SIMPLE_Expression_CODE(node * exp){ // Resolver expresiones simples, digase ejecutar un comando
        return Run(exp);
}

int TRUE_CODE(node* exp){ // Comando true
    return 0;
}

int FALSE_CODE(node* exp){ // Comando false
    return 1;
}

bool inside_pipe = false; // Variable booleana para saber si nos encontramos dentro de un pipe (Muy util para ejecutar el comando EXIT)

int EXIT_CODE(node* exp){
    if(inside_pipe){ // Si nos encontramos dentro de un pipe, este no se cierra, por tanto retornamos 0, sino cerramos el proceso desde el cual se llamo al comando exit
        return 0;
    }
    exit(0);
    return 0;
}


list * var_list; // Lista para guardar las variables del get, set y unset

int PrintVariables(){ // Metodo para imprimir las variables que contenga la lista de variables al llamar al set

    if(var_list->size <= 1)
        return 0;

    node * current = var_list->head->next;
    char * output = strdup("");
    int count = 2;

    while (current != NULL && count <= var_list->size) // Mientras quedes nodos en la lista sigue ejecutando el bucle
    {

        keyvaluepair * kv = current->value;
        char * current_exp = kv->key;
        char * value = kv->value;

        strcat(output, current_exp); // Concatenando key a la salida
        strcat(output, "=");

        if(value!= NULL)
            strcat(output, value); // Concatenando value a la salida

        if(count != var_list->size)
            strcat(output, "\n");
        current = current->next;
        count++;
    }

    printf("%s\n", output);
    return 0;
}

int Execute(node * first_cmd, node * last_cmd);

// Metodo utilizado para buscar el nodo que contenga una llave igual a target
node * search_str_node(list * l, char * target){
    node * current = l->head->next;
    int count = 2;


    while(current != NULL && count <= l->size && current->value != NULL)
    {
        keyvaluepair * kv = current->value;
        char * current_exp = strdup(kv->key);
        if(strcmp(current_exp, target) == 0){
            return current;
        }
        current = current->next;
        count++;
    }
    return NULL;
}

// Annadir una variable de tipo keyvaluepair a la lista de variables del get, set y unset
int AddVar(char * key, char * value){
    node * temp;
    if((temp = search_str_node(var_list, key)) == NULL){ // Comprobando que no se haya agregado un nodo con esa llave a la lista, en caso de encontrar alguno, se cambia el value de esa variable
        keyvaluepair * kv = (keyvaluepair*)malloc(sizeof(keyvaluepair));
        kv->key = strdup(key);
        kv->value = strdup(value);

        push_back(var_list, kv);
    }
    else{
        keyvaluepair * kv = temp->value;
        kv->value = strdup(value);
    }
    return 0;
}

// Busca en la lista el proximo nodo Expression que sea un Operador de Caracter Set
node * FindSetCharacter(node * beg){
    node * current = beg;

    while (current!=NULL)
    {
        Expression * current_exp = current->value;
        if(current_exp->operators == SET_CHARACTER){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Ejecuta el contenido dentro de las comillas del comando SET
int ExecuteSetCharacter(char * key, node* beg , node * last){
    node * end =  FindSetCharacter(last->next); // Busca el nodo final, para tener bien definidos los limites del codigo a ejecutar entre las comillas

    if(end == NULL)
        return -1;

    FILE * fp = fopen("temp2", "w"); // Abriendo archivo temp2 para guardar salida de ejecucion de lo que se encuentre entre las comillas
    fclose(fp);

    only_write("temp2"); // escribiendo lo siguiente que salga por el stdout al archivo temp2


    Execute(last->next, end->previous); // ejecutando el comando que se ha pasado entre las comillas
    int *status=0;
    waitpid(-1, status, WNOHANG);

    input_read("temp2"); // leyendo el contenido del archivo temp2


    char file_contents[1000];
    int num=read(STDIN_FILENO,file_contents,sizeof(file_contents));
    file_contents[num] = '\0'; // guardando el contenido en file_contents

    AddVar(key, file_contents); // Annadiendo una nueva variable con llave=key y value=file_contents

    return 0;
}

// Ejecuta el comando Set
int SET_CODE(node* exp){
    // printf("Inside Run Method...\n");
    Expression * com_to_exec = exp->value;
    if(com_to_exec->operators != SET)
        return -1;

    if(exp->next == NULL)
        return PrintVariables();

    Expression * var_name = exp->next->value; // me quedo con su .next(el nombre de la variable que viene a continuacion)

    if(var_name->operators != ARGS){
        return PrintVariables();
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
        int output = ExecuteSetCharacter(var_name->name, exp ,last); // pasamos el comando set principal y el nodo a partir del cual se encontro el primer caracter de Set
        // if(remove("temp2")){}
        return output;
    }

    // Buscando los argumentos para ir concatenandolos y asi formar el string final que corresponde a la variable

    int count = 0;
    while(last_exp != NULL && (last_exp->operators == ARGS )){// Con ARGS me refiero a los argumentos lo q aun no se bien como ponerle
        count++;
        last = last->next;
        if(last != NULL)
            last_exp = last->value;
        else
            last_exp = NULL;
    }

    char * str_value = strdup("");

    int temp = 0; // Para ir ubicando argumenta a argumento en el arreglo
    node * current = exp->next->next;
    while (temp < count)
    {
        Expression * current_com = current->value;
        if(temp >0)
            strcat(str_value, " ");
        strcat(str_value, current_com->name);
        temp++;
        current=current->next;
    }

    AddVar(var_name->name, str_value);
    return 0;
}

// Ejecuta el comando Get
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

    node * temp = search_str_node(var_list, next_exp->name);
    if(temp != NULL)
    {
        keyvaluepair * kv = temp->value;
        char * output = kv->value;
        printf("%s\n", output);
        return 0;
    }
    return -1;
}


// Ejecuta el comando unset
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

    node * temp = search_str_node(var_list, next_exp->name);
    if(temp != NULL){
        removeNode(var_list, temp);
        return 0;
    }

    return -1;
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

    bool is_if, is_set_char = false;
    node * current = first_cmd;
    Expression * current_temp = current->value;
    while(current != last_cmd){
        current_temp = current->value;
        if(current_temp->operators == IF){
            is_if = is_set_char = true;
            }
        else if(current_temp->operators == SET_CHARACTER){
            is_set_char = !is_set_char;
        }
        else if(current_temp->operators == END){
            is_if = is_set_char = false;
            }
        else if((current_temp->operators == AND || current_temp->operators == OR) && !is_if && !is_set_char){
            return current;
        }

        current = current->next;
    }
    return NULL;
}

node * Search_PIPE(node * first_cmd, node * last_cmd){
    bool is_if, is_set_char = false;
    node * current = first_cmd;
    Expression * current_temp;
    while(current != last_cmd){
        current_temp = current->value;
        if(current_temp->operators == IF){
            is_if = true;
            }
        else if(current_temp->operators == SET_CHARACTER){
            is_set_char = !is_set_char;
        }
        else if(current_temp->operators == END){
            is_if = is_set_char = false;
            }
        else if(current_temp->operators == PIPE && !is_if && !is_set_char){
            return current;
        }

        current = current->next;
    }
    return NULL;
}

node * Search_IF_THEN_ELSE(node * first_cmd, node * last_cmd, enum OPERATORS operator){
    node * current = first_cmd;
    bool is_set_char = false;
    Expression * current_temp = current->value;
    while(current != NULL){
        current_temp = current->value;
        if(current_temp->operators == SET_CHARACTER){
            is_set_char = !is_set_char;
        }
        else if(current_temp->operators == operator && !is_set_char){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Leaves
int GetIndex(Expression * op){
    return (int)op->operators;
}

int ExecuteExpression(node * cmd_to_exec){
    Expression * node_com = cmd_to_exec->value;
    int index = node_com->operators;
    return testing[index](cmd_to_exec);
}


int SolveExpressions(node * first_cmd, node * last_cmd){
    node * current = first_cmd;
    Expression * current_exp = current->value;

    if(current_exp->operators != REDIRBIG && current_exp->operators != REDIRLESS && current_exp->operators != DOUBLEREDIRBIG)
        return ExecuteExpression(current);
    return -1;
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

            fp=fopen(current_next->name,"w");
            fclose(fp);
            output=current_next;
        }
        else if(current_exp->operators==DOUBLEREDIRBIG)
        {
            Expression *current_next=current->next->value;

            fp=fopen(current_next->name,"a");
            fclose(fp);

            output=current_next;

        }
        current=current->next;
    }
    if(output!=NULL){
        Expression * exp=first_cmd->value;

        if(exp->operators != REDIRBIG && exp->operators != REDIRLESS && exp->operators != DOUBLEREDIRBIG){
        input_read("temp");


        char file_contents[1000];
        int num=read(STDIN_FILENO,file_contents,sizeof(file_contents));


        exp->std_out=strdup(file_contents);

       if(remove("temp")){}

        char * test = strdup(output->name);
        only_append(test, file_contents, num);

        redir_found = true;
        }
    }

    if(redir_found == false)
        return exp_out;
    return 0;
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
    Expression * current_exp;

    while (current != NULL)
    {
        current_exp = current->value;
        if(current_exp->operators == REDIRLESS){
            input=current->next;
            break;
        }
        current = current->previous;
    }

    if(input!=NULL)
    {
        Expression * input_exp=input->value;
        Expression * fisrt_exp=first_cmd->value;
        fisrt_exp->std_in=input_exp->name;
    }
    return first_cmd;
}


int Solve_Leaves(node * first_cmd, node * last_cmd){
    SolveLessRedir(first_cmd, last_cmd);
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

    // AND OR
    node * AND_OR = Search_AND_OR(first_cmd, last_cmd); // Buscando And u Or (&& u ||) sin que se encuentren dentro de un if
    Expression * AND_OR_com;
    if(AND_OR != NULL){
        AND_OR_com = AND_OR->value;

        int output = Execute(first_cmd, AND_OR->previous);
        if(AND_OR_com->operators == AND){
            if(output ==0)
            { // Si && retorna true devuelve el codigo a la derecha
                return Execute (AND_OR->next, last_cmd);
            }
            return output;
        }else if(AND_OR_com->operators == OR){
            if(output!=0) // Si || retorna false devuelve el codigo a la derecha
                return Execute(AND_OR->next, last_cmd);
            return output;
        }
    }

    // Pipes
    node * PIPE_node = Search_PIPE(first_cmd, last_cmd);
    Expression * PIPE_node_com;


    if(PIPE_node!= NULL){ // 3 casos
        PIPE_node_com = PIPE_node->value;

        int fd[2];
        pid_t pidC;
        int *status=0;
        int output=0;

        inside_pipe = true;
        pipe(fd);
        pidC = fork();
        pid=pidC;
        did_ctrl_c=false;

        if(pidC==0){
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            Execute(first_cmd, PIPE_node->previous);
        }
        else if(pidC==-1){
            printf("Error\n");
        }
        else{

            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);

            output = Execute(PIPE_node->next, last_cmd);
            int child_pid = waitpid(-1, status, WNOHANG);
            kill(pidC, SIGKILL);
        }

        inside_pipe = false;

        Expression * com_prev;
        if(PIPE_node->previous != NULL){
            com_prev = PIPE_node->previous->value;

            if(com_prev->operators == EXIT ){
                if(remove("exit")){}
            }
        }
        return output;
    }


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
            if(ELSE_node == NULL)
                ELSE_node = END_node;
            return Execute(THEN_node->next, ELSE_node->previous);
        }
        else if(ELSE_node != NULL){
            // Ejecuta el Else
            return Execute(ELSE_node->next, END_node->previous);
        }
        return IF_output;
    }

    // Leaves
    return Solve_Leaves(first_cmd, last_cmd);
}
#pragma endregion

#pragma region Parser

bool IfThenElseEndComprobation(int* operator,bool* did_if, bool* did_then, bool* did_else)
{
    if((*operator)==IF)
    {
        if((*did_if)==true)
        {
            (*operator)=ARGS;
            return false;
        }
        (*did_if)=true;
        return true;
    }
    if((*operator)==THEN)
    {
        if(!(*did_if))
        {
            (*operator)=ARGS;
            return false;
        }
        if((*did_then))
        {
            (*operator)=ARGS;
            return false;
        }
        (*did_then)=true;
        return true;
    }
    if((*operator)==ELSE)
    {
        if(!(*did_if))
        {
            (*operator)=ARGS;
            return false;
        }
        if(!(*did_then))
        {
            (*operator)=ARGS;
            return false;            
        }
        if((*did_else))
        {
            (*operator)=ARGS;
            return false;    
        }
        (*did_else)=true;
        return true;
    }
    if((*operator)==END)
    {
        if(!(*did_if))
        {
            (*operator)=ARGS;
            return false;
        }
        if(!(*did_then))
        {
            (*operator)=ARGS;
            return false;            
        }
        (*did_if)=false;
        (*did_then)=false;
        (*did_else)=false;
        return true;
    }
}

void EjecuteLine(list* line)
{

    bool command=true;//indica si en este momento debo recibir un comando
    bool archive=false;//indica si en este momento debo recibir un archivo
    bool special_caracter_last=false;//indica si la palabra anterior fue un caracter especial
    bool if_caracter_last=false;//indica si la expresion anterior es de tipo if, else then o end
    bool open_set_command=false;//indica si ocurrio un comando ` pero no el segundo
    bool did_if=false;//indica si ocurrio un if que fue tomado como if y no como argumento
    bool did_then=false;//indica si ocurrio un then que fue tomado como then y no como argumento
    bool did_else=false;//indica si ocurrio un else que fue tomado como else y no como argumento

    Expression * exp1 = (Expression*)malloc(sizeof(Expression));//para guardar la primera expresion

    char* temp = pop_front(line)->value; //guarda temporalmente el primer valor de la lista de string
    if(strcmp("^C",temp)==0)return;

    if(SpecialCaracters(temp)&& !RedirCaracter(temp))////cuando como primera palabra tenemos un caracter especial que no puede ocupar ese lugar
    {
        printf("syntax error near unexpected token `%s'\n",temp);
        
        return;
    }


    exp1->name=strdup(temp);
    exp1->operators = GetOperator(temp);

    list* exp_line=init_list(exp1); //linea de expresiones clasificadas


    command=false;
    if(SpecialCaracters(temp)){command=true;special_caracter_last=true;}
    if_caracter_last=strcmp(temp,"if")==0;
    if(if_caracter_last && strcmp(temp,"if")!=0)
    {
        printf("syntax error near unexpected command: `%s'\n",temp);
    }
    if(if_caracter_last)command=true;
    if(RedirCaracter(temp)) archive=true;
    if(exp1->operators==IF){did_if=true;}
    


    while(line->head!=NULL)
    {
        temp=line->head->value;
        if(strcmp(temp," ")==0)
        {
            free(pop_front(line));
            continue;
        }

        char* name=strdup(temp);

        if(strcmp(";",name)==0)
        {
            free(pop_front(line));
            Execute(exp_line->head,exp_line->tail);
            if(line->head!=NULL) EjecuteLine(line);
            pid=getpid();
            if(did_if && !did_then) {printf("syntax error near unexpected token: `%s'\n",name);return;}

            return;
        }

        bool special_caracter_now=SpecialCaracters(temp);//la expresion actual es un caracter especial
        if(special_caracter_last && special_caracter_now){printf("syntax error near unexpected token `%s'\n",temp); return;}
        if(if_caracter_last && special_caracter_now && !RedirCaracter(temp)){printf("syntax error near unexpected token `%s'\n",temp); return;}
        if(special_caracter_last || if_caracter_last)command=true;
        if(special_caracter_now && strcmp(temp,"`")==0) open_set_command=!open_set_command;

        int op=GetOperator(temp);

        if(op==SIMPLE_EXPRESSION || PossibleArgumentExpression(op))////////lo que agregue esta solo aqui y en el metodo
        {
           if(!command)op=ARGS;//Si no estamos esperando un comando entonces estamos en presencia de un argumento
            if(archive)op=ARCHIVE;//Si la expresion anterior es un caracter especial de redireccion shora estamos en presencia de un archivo

        }

        if_caracter_last=IfCommand(op);
        if(if_caracter_last)
        {
            if_caracter_last=IfThenElseEndComprobation(&op,&did_if,&did_then,&did_else);
        }
        command=false;
        archive=false;
        if(RedirCaracter(temp)) archive=true;

        special_caracter_last=special_caracter_now;
        push_back(exp_line,GetStructExpression(name, op));

        free(pop_front(line));

    }

    if(did_if)
    {
        if(!did_then) printf("command then not found\n");
        else printf("command end not found\n");
        
        return;
    }
    if(open_set_command)
    {
        printf("token ` ` ' not found\n");
    }
    if(exp_line->size==1 && RedirCaracter(exp1->name))
    {
        printf("bash: syntax error near unexpected token `newline'\n");
        return;
    }

    Execute(exp_line->head,exp_line->tail);

    pid=getpid();


    free_list(exp_line);
    free(line);
}

void ReadAndEjecuteLine(list* line,char* word, char c)//crea una lista de string por cada instruccion y luego manda a ejecutarlas
{
    bool history=true;
    if(c==' ') history=false;
    char *strline=(char*)calloc(sizeof(char),1000);
    if(history)ConcatChar(c,strline);

    while(c!= '\n' && c!= EOF)
    {

        if(c==-1) exit(0);

            if(c=='#')
            {
                EndReadLine(strline,history);
                break;
            }

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
                   continue;

            }

            if(SpecialCaracters(word))///Si es un caracter normal pero esta pegado a un caracter especial anterior
            {
                if(c==' ')c=GetOneChar(strline,history);
                push_back(line, strdup(word));//saco el caracter especial para la lista y renuevo a word
                strcpy(word,"");
                ConcatChar(c,word);
                c=GetOneChar(strline,history);
                continue;

            }

            if(c==' ')
            {
                if(word[0]=='\0')
                {
                    c=GetOneChar(strline,history);
                    continue;

                }
                push_back(line, strdup(word));

                strcpy(word,"");///vacio a word para comenzar una nueva palabra
                c=GetOneChar(strline,history);
                continue;
            }

            ConcatChar(c,word);
            c=GetOneChar(strline,history);
        }

        if(strcmp(word,"")!=0) push_back(line, strdup(word));
        strcpy(word,"");
        free(word);
        pop_front(line);
        if(line->head!=NULL)
        {
            if(strcmp(line->head->value,"again")==0)history=false;
            if(history) SaveLine(strline,line);
            EjecuteLine(line);
        }   
}

void fflush_stdin() {
    char c;
    while((c = getchar()) != '\n' && c != EOF){}
}

void Shell()
{

printf("\n");
 while (1)
    {
        int std_in;
        int std_out;
           std_in=dup(STDIN_FILENO);
           std_out=dup(STDOUT_FILENO);    
        char * word= (char*)calloc(sizeof(char),100);//word es cada una de las palabras que se mandan en un espacio de line
        strcpy(word,"");

        list* line=init_list("init"); //line es la linked list que guarda los comandos argumentos y caracteres especiales
       
        if(pid_inicial==getpid()) printf("my-shell $ ");
 
        did_again=false;

        char c;
        c = getchar();  //cada uno de los char a leer de consola

        if(c==-1) exit(0);
  

        ReadAndEjecuteLine(line,word,c);

    
            dup2(std_in,STDIN_FILENO);
            dup2(std_out,STDOUT_FILENO);
            close(std_in);
            close(std_out);
       
    }

}


#pragma endregion




int main(int argc, char const *argv[])
{
    keyvaluepair * kv = (keyvaluepair*)malloc(sizeof(keyvaluepair));
    kv->key = strdup("neverusethisname");
    kv->value = strdup("neverusethisname");
    var_list = init_list(kv);

    getcwd(history_direction,200);
    strcpy(again_direction,history_direction);
    strcat(history_direction,"/history.txt");
    strcat(again_direction,"/again.txt");

    did_again=false;
    signal(SIGINT,CtrlC);

    pid=getpid();
    pid_inicial=getpid();
    did_ctrl_c=false;

    Shell();
    free(var_list);
    return 0;
}


