#include "execute.h"
// #include "bin_tree.h"
// #include "list.h"
// #include<stdbool.h>// Booleanos

void AND_CODE(Command com){

}

void (*testing[])(Command*) = {
        AND_CODE,
        0
	};

node * Search_AND_OR(node * first_cmd, node * last_cmd){

    bool is_if = false;
    node * current = first_cmd;

    while(current != last_cmd){
        Command * current_temp = current->value;
        if(current_temp->operators == IF){
            is_if = true;
            continue;
            }
        if(current_temp->operators == END){
            is_if = false;
            continue;
            }
        if((current_temp->operators == AND || current_temp->operators == OR) && !is_if){
            return current;
        }

        current = current->next;
    }
    return NULL;
}

node * Search_PIPE(node * first_cmd, node * last_cmd){
    bool is_if = false;
    node * current = first_cmd;

    while(current != last_cmd){
        Command * current_temp = current->value;
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
            return current;
        }

        current = current->next;
    }
    return NULL;
}

node * Search_IF_THEN_ELSE(node * first_cmd, node * last_cmd, enum OPERATORS operator){
    node * current = first_cmd;
    Command * current_temp = current->value;
    while(current != last_cmd){
        if(current_temp->operators == operator){
            return current;
        }
    }
    return NULL;
}

// Leaves
node * SolveCommands(node * first_cmd, node * last_cmd){
    node * current = first_cmd;
    while (current != last_cmd)
    {
        ExecuteCommand(current);
        current = current->next;
    }
    return current;
}

void ExecuteCommand(node * cmd_to_exec){
    Command * node_com = cmd_to_exec->value;
    int index = GetIndex();
    testing[index](node_com);
}

node * Solve_Leaves(node * first_cmd, node * last_cmd){
    SolveBiggerRedir(first_cmd, last_cmd);
    SolveLessRedir(first_cmd, last_cmd);
    return SolveCommands(first_cmd, last_cmd);
}

void SolveBiggerRedir(node * first_cmd, node * last_cmd){
    node * current;
    node * BIGGER_REDIR = SearchBiggerRedir(last_cmd);
    if(BIGGER_REDIR == NULL)
        return;
    Command * prev_temp = current->previous->value;
    Command * current_temp = current->value;
    while(current==REDIRBIG ||  // >
          current==REDIRLESS || // <
          (current==ARCHIVE && //  < archivo o....... > archivo
           (current->previous==REDIRLESS || current->previous==REDIRBIG)
          )|| // < comando o ............. > comando 
          (current==SIMPLE_COMMAND && //  < comando o....... > comando
           (current->previous==REDIRLESS || current->previous==REDIRBIG)
          )
          ){
        // Command * prev_temp = current->previous->value;
        // Command * current_temp = current->value;
        // current->previous->value 
        prev_temp->std_in = current_temp->std_out;
        current = BIGGER_REDIR->previous;
        if(current == NULL){
            return;
        }


    }
}

node * SearchBiggerRedir(node * last_cmd){
    node * current = last_cmd;

    while(current != last_cmd){
        if(current == REDIRBIG){
            return current;
        }
    }
    return NULL;
}

node * SolveLessRedir(node * first_cmd, node * last_cmd){
    retun NULL;
}
// Leaves

node * Execute(node * first_cmd, node * last_cmd){
    if(first_cmd == last_cmd){
        return Call();
    }

    // AND OR
    node * AND_OR = Search_AND_OR(first_cmd, last_cmd); // Buscando And u Or (&& u ||) sin que se encuentren dentro de un if
    if(AND_OR != NULL){

        node * output = Execute(first_cmd, AND_OR->previous);
        if(AND_OR == AND){
            if(output == true) // Si && retorna true devuelve el codigo a la derecha
                return Execute (AND_OR->next, last_cmd);
            return output;
        }else if(AND_OR == OR){
            if(output == false) // Si || retorna false devuelve el codigo a la derecha
                return Execute(AND_OR->next, last_cmd);
            return output;
        }
        return;
    }

    // Pipes
    node * PIPE_node = Search_PIPE(first_cmd, last_cmd);
    Command * PIPE_node_com = PIPE_node->value;

    if(PIPE_node!= NULL){ // 3 casos
        node * output; 

        if(first_cmd->next == LEAF){ // Caso en q sea una hoja a la derecha
            node * pipe_left = Solve_Leaves(first_cmd, PIPE_node->previous);
            Command* pipe_left_com = pipe_left->value;
            PIPE_node_com->std_in = pipe_left_com->std_out;
            //node * pipe_right = 
            SolveBiggerRedir(PIPE_node->next, last_cmd);
            output = Execute(PIPE_node->next, last_cmd);
        }
        else if(first_cmd->next != NULL && first_cmd->next->next != NULL && first_cmd->next->next == PIPE_node){ // Caso 2: Luego del primer PIPE que encontramos viene otro PIPE | command | ...
            node * pipe_left = Solve_Leaves(first_cmd, PIPE_node->previous);
            Command* pipe_left_com = pipe_left->value;
            PIPE_node_com->std_in = pipe_left_com->std_out;
            // node * pipe_right = Solve_Leaves(PIPE_node, )
            if(PIPE_node_com->std_in == NULL)
                PIPE_node_com->std_in = pipe_left_com->std_out;
            Execute(PIPE_node->next, last_cmd);
        }
        else if(first_cmd->next != NULL && first_cmd->next == IF){ // 3er caso: Le sigue un if a la derecha | if()then()else()...
            node * pipe_left = Solve_Leaves(first_cmd, PIPE_node->previous);
            Command* pipe_left_com = pipe_left->value;
            PIPE_node_com->std_in = pipe_left_com->std_out;
            Execute(PIPE_node->next, last_cmd);
        }
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
        
    }

    // Leaves
    return Solve_Leaves(first_cmd, last_cmd);
}