#include "execute.h"
// #include "using_execvp.h"
#include <unistd.h>
#include <sys/wait.h>
// #include "bin_tree.h"
// #include "list.h"
// #include<stdbool.h>// Booleanos

void Run(node * com){
    Expression * com_to_exec = com->value;
    if(com_to_exec->operators != SIMPLE_EXPRESSION)
        return;
        
    // list * temp_args = init_list(com->next);

    node * last = com->next;
    Expression * last_com = last->value;
    int count = 0;
    while(last_com->operators == LEAF){// Con leaf me refiero a los argumentos lo q aun no se bien como ponerle
        count++;
        last = last->next;
    }

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
    
    execvp(myargs[0], myargs);
}

void SIMPLE_Expression_CODE(node * com){
    int rc = fork();
    if (rc < 0) { // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) { // child (new process)
        Run(com);
    } else { // parent goes down this path (main)
        int wc = wait(NULL);
    }
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
            free(current_temp);
            return current;
        }

        current = current->next;
    }

    if(current == NULL)
        printf("current is NULL");
    if(current_temp == NULL)
        printf("current_temp is NULL");
    free(current_temp);
    free(current);
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
            free(current_temp);
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
    //     free(current);
    return NULL;
}

node * Search_IF_THEN_ELSE(node * first_cmd, node * last_cmd, enum OPERATORS operator){
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
    int index = GetIndex(node_com);
    testing[index](cmd_to_exec);
}


node * SolveExpressions(node * first_cmd, node * last_cmd){
    node * current = first_cmd;
    while (current != last_cmd)
    {
        ExecuteExpression(current);
        current = current->next;
    }
    return current;
}


node * Solve_Leaves(node * first_cmd, node * last_cmd){
    SolveBiggerRedir(first_cmd, last_cmd);
    SolveLessRedir(first_cmd, last_cmd);
    printf("Starting to solve Expressions...\n");
    return SolveExpressions(first_cmd, last_cmd);
}

void SolveBiggerRedir(node * first_cmd, node * last_cmd){
    node * current;
    node * BIGGER_REDIR = SearchBiggerRedir(last_cmd);

    if(BIGGER_REDIR == NULL)
        return;

    current = BIGGER_REDIR->previous;

    Expression * prev_temp = current->previous->value;
    Expression * current_temp = current->value;


    while(current_temp->operators==REDIRBIG ||  // >
          current_temp->operators==REDIRLESS || // <
          (current_temp->operators==ARCHIVE && //  < archivo o....... > archivo
           (prev_temp->operators==REDIRLESS || prev_temp->operators==REDIRBIG)
          )|| // < comando o ............. > comando 
          (current_temp->operators==SIMPLE_EXPRESSION && //  < comando o....... > comando
           (prev_temp->operators==REDIRLESS || prev_temp->operators==REDIRBIG)
          )
          ){
        // Expression * prev_temp = current->previous->value;
        // Expression * current_temp = current->value;
        // current->previous->value 
        prev_temp->std_in = current_temp->std_out;
        current = current->previous;

        if(current == NULL){
            return;
        }

        prev_temp = current->previous->value;
        current_temp = current->value;
    }
}

node * SearchBiggerRedir(node * last_cmd){
    node * current = last_cmd;
    Expression * current_temp = current->value;

    while(current != last_cmd){
        if(current_temp->operators == REDIRBIG){
            return current;
        }

        current = current->next;
    }
    return NULL;
}

node * SolveLessRedir(node * first_cmd, node * last_cmd){
    return NULL;
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
        free(output);
        free(output_com);
        // return NULL;

        free(AND_OR);
        free(AND_OR_com);
    }
    
    printf("Start searching Pipes...\n");
    // Pipes
    node * PIPE_node = Search_PIPE(first_cmd, last_cmd);
    printf("Out of found PIPE method...\n");
    Expression * PIPE_node_com;
    if(PIPE_node != NULL)
        PIPE_node_com = PIPE_node->value;

    if(PIPE_node!= NULL){ // 3 casos
        printf("found a Pipe...\n");

        node * output;
        // Creo las variables de tipo Expression auxiliares, necesarias en este if para pedirles su -> operators
        Expression * first_cmd_next_com = first_cmd->next->value;
        Expression * first_cmd_next_next_com = first_cmd->next->next->value;

        if(first_cmd_next_com->operators == LEAF){ // Caso en q sea una hoja a la derecha
            node * pipe_left = Solve_Leaves(first_cmd, PIPE_node->previous);
            Expression* pipe_left_com = pipe_left->value;
            PIPE_node_com->std_in = pipe_left_com->std_out;
            //node * pipe_right = 
            SolveBiggerRedir(PIPE_node->next, last_cmd);
            output = Execute(PIPE_node->next, last_cmd);
        }
        else if(first_cmd->next != NULL && first_cmd->next->next != NULL && first_cmd->next->next == PIPE_node){ // Caso 2: Luego del primer PIPE que encontramos viene otro PIPE | Expression | ...
            node * pipe_left = Solve_Leaves(first_cmd, PIPE_node->previous);
            Expression* pipe_left_com = pipe_left->value;
            PIPE_node_com->std_in = pipe_left_com->std_out;
            // node * pipe_right = Solve_Leaves(PIPE_node, )
            if(PIPE_node_com->std_in == NULL)
                PIPE_node_com->std_in = pipe_left_com->std_out;
            Execute(PIPE_node->next, last_cmd);
        }
        else if(first_cmd->next != NULL && first_cmd_next_com->operators == IF){ // 3er caso: Le sigue un if a la derecha | if()then()else()...
            node * pipe_left = Solve_Leaves(first_cmd, PIPE_node->previous);
            Expression* pipe_left_com = pipe_left->value;
            PIPE_node_com->std_in = pipe_left_com->std_out;
            Execute(PIPE_node->next, last_cmd);
        }

        free(PIPE_node);
        free(PIPE_node_com);

        free(first_cmd_next_com);
        free(first_cmd_next_next_com);
        return output;
        free(output);
    }

    
    printf("Starting If Else part...");
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
        free(THEN_node);
        free(END_node);
        free(ELSE_node);
        free(IF_output);
    }
    free(IF_ELSE_node);

    printf("Starting to solve leaves...\n");
    // Leaves
    return Solve_Leaves(first_cmd, last_cmd);
}