#include "execute.h"
// #include "using_execvp.h"
#include <unistd.h>
#include <sys/wait.h>
#include<sys/stat.h>
// #include "bin_tree.h"
// #include "list.h"
// #include<stdbool.h>// Booleanos

void Run(node * com){
    printf("Inside Run Method...\n");
    Expression * com_to_exec = com->value;
    if(com_to_exec->operators != SIMPLE_EXPRESSION)
        return;
        
    // list * temp_args = init_list(com->next);

    node * last = com->next;
    Expression * last_com = last->value;
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
    freopen(com_to_exec->std_in,"r", stdin);

    printf("Executing command...\n");
    int e = execvp(myargs[0], myargs);
    fclose(stdin);
    printf("%d", e);
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


node * Solve_Leaves(node * first_cmd, node * last_cmd){
    SolveLessRedir(first_cmd, last_cmd);
    printf("Starting to solve Expressions...\n");
    SolveExpressions(first_cmd, last_cmd);
    return SolveBiggerRedir(first_cmd, last_cmd);

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
    node *input;
    FILE * fp;
    Expression * current_exp;

    printf("Llegamos a < wiiiiiii!!!!\n");
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
    return first_cmd;
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

node * Execute(node * first_cmd, node * last_cmd){
    if(first_cmd == last_cmd){
        return first_cmd;
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
        int fd[2];
        int status,pid;

        pipe(fd);
        pid=fork();
        //if(pid==0)
        printf("found a Pipe...\n");

        node * output;
        // Creo las variables de tipo Expression auxiliares, necesarias en este if para pedirles su -> operators
        Expression * first_cmd_next_com = first_cmd->next->value;
        Expression * first_cmd_next_next_com = first_cmd->next->next->value;

        

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
        return output;
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