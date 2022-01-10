#include "help.h"

void help_init()
{
    printf("Integrantes: Carlos Carret Miranda(C-212)\n"
                         "Lauren Guerra Hernandez(C-212)\n"
        "Funcionalidades:\n"
        "basic: funcionalidades básicas (1-8)  (3 puntos)\n"
        "multi-pipe: múltiples tuberías  (9)   (1 punto)\n"
        "spaces: múltiples espacios   (11)    (0.5 puntos)\n"
        "history: trabajar con el historial de comandos f(12)    (0.5 puntos)\n"
        "ctrl+c: capturar y enviar señales a procesos (13) (0.5 puntos)\n"
        "if:funcionalidad 15, 1punto"
        "chain: concatenación de comandos con ; y operaciones booleanas (14) (0.5 puntos)\n"
        "help: ayuda del shell (17) (1 punto)\n"

        "Comandos built-in:\n"
        "cd: cambia de directorios\n"
        "exit: termina el shell\n"
        "history: muestra los 10 últimos comandos ejecutados\n"
        "again: se sutituye por un comando seleccionado dentro del historial\n"
        "true: retorna 1\n"
        "false: retorna 0\n"
        "if expresi'on booleana\n"
        "then\n"
        "else\n"
        "end\n"
        "set:asigna un valor a una variable\n"
        "get:obtiene el valor de una variable\n"
        "unset:elimina una variable\n"
        "help: muestra esta ayuda\n\n"
        );
}

void help_basic()
{
    printf("\n Funcionalidad 1: Al comenzar cada iteraci'on de nuestra shell lo primero que se realiza en el m'etodo Shell"
    "es imprimir `my-shell $` en la consola para luego con el getchar() leer del stdin de la misma.\n"
	"Funcionalidad 2: Los comandos se ejecutan correctamente ya sean propios del sistema o pertenezcan"
    "a nuestro build-in.\n"
    "Funcionalidad 3: El comando cd cambia el directorio ,empleando la función chdir, seg'un el path que recibe como argumento.\n"

    "Funcionalidad 4: Los operadores de redireccion fueron implementados comenzando con el operador <, luego ejecutando el comando al cual se le entra el contenido del algun archivo, luego, vamos a ejecutar la funcionalidad de los operadores > y >>, guardando o sobreescribiendo el contenido de algunos archivos\n"
    "Funcionalidad 5: Los pipes fueron implementados utilizando un arreglo de de dos posiciones y el metodo pipe() para redirigir la entrada y la salida stdin, stdout, se ejecutan haciendo un fork y ejecutando con execvp en caso de ser necesario la parte izquierda y luego hacemos otro fork en un llamado recursivo a la parte derecha\n"
    "Funcionalidad 6: Luego de terminar la ejecuci'on de una l'inea de comandos se vuelve al m'etodo shell"
    "donde se vuelve a imprimir `my-shell $` y se puede entrar otra l'inea de comando para ser ejecutada \n"
    "Funcionalidad 7: Se separan los comandos con un espacio, mas al tambi'en estar implementada la funcionalidad"
    "spaces, se puede dejar m'as de un espacio entre comandos y entre 0 y muchos con operadores especiales .\n"
    "Funcionalidad 8: En nuestro parser, m'etodo ReadAndEjecuteLine() luego de encontrarnos mediante el getchar()"
    "con el caracter #, terminamos de leer esta linea pero no guardamos nada de lo que se encuentre despu'es de #. Adem'as se implement'o el comando exit que termina la ejecici'on de la consola,"
    "utilizando el m'etodo exit() con exit(0) se indica que la salida fue satisfactoria y con exit(-1) lo opuesto.\n\n");

}


void help_chain()
{
	printf("\n Los operadores &&, ||, ; concatenan comandos, en el caso del ; vamos leyendo cada linea, cuando"
    " nos encontramos con uno de estos enviamos esta l'inea a ejecutarse y seguimos leyendo esta l'inea de la consola y "
    "cuando finalice la mandamos a ejecutar aparte, a la hora de ejecutar una línea tomamos como caracteres de mayor"
    "prioridad a && y ||  , luego buscamos los pipes (command1|command2) y luego buscamos los if then else y end, por 'ultimo resolvemos"
    "las expresiones simples, del tipo command1 argumento < archivo. \n\n");
}

void help_spaces()
{
	printf("\n Entre comandos y argumentos puede haber cualquier cantidad de espacios >=1 \n. Esto se implemt'o a la hora de ir tomando las instrucciones "
    "de la consola con el getchar() si tenemos un espacio tal que la palabra que estamos formando esta vac'ia a'un, este espacio se obvia. Ejemplo:\n"
    "`com1 arg1` y `com1     arg1` \n y entre comandos y operadores puede haber igualmente cualquier cantidad "
    "de espacios. \n Ejemplo: com1 arg 1||com2 y com1               &&com2 arg1.\n \n");

}

void help_multipipe()
{
    printf("\nLos pipes fueron implementados utilizando un arreglo de de dos posiciones y el metodo pipe() para redirigir la entrada y la salida stdin, stdout, se ejecutan haciendo un fork y ejecutando con execvp en caso de ser necesario la parte izquierda y luego hacemos otro fork en un llamado recursivo a la parte derecha"
    "Luego de llamar recursivo a la parte derecha, verificamos si existe otro pipe y en caso de existir volvemos a llamar recursivamente a la parte derecha y hacemos un fork a la parte izquierda del nuevo pipe encontrado para ejecutar su comando. \n "
    "Se pueden ejecutar comandos que implementen algun operador de redireccion. Se puede poner cualquier cadena de comandos y caracteres especiales entre un pipe y otro.\n\n");
}

void help_controlc()
{
    printf("\n El Ctrl+C esta implementado utilizando la señal SINGINT que se recibe al inicio del método main(), la primera señal recibida no se toma"
    ", esto se implement'o con un bool y si ya no estamos en la primera sennal se mata la ejecuci'on con SINGKILL. Hay un pequeño desperfecto a la hora de "
    "dar Ctrl+C cuand no se est'a ejecutando ning'un comando este Ctrl+C se escribe en la consola y no sale un nuevo my shell $. \n\n ");
}

void help_history()
{
    printf("\nEl comando history muestra las 10 'ultimas instrucciones que le han sido dadas a nuestra consola, esto se implement'o guardando en history.txt estas "
    "l'ineas de instrucciones y en cada ejecuci'on si la l'inea no empieza con un espacio se da la instrucci'on de guardarla. \n "
    "En el caso del again esta again.txt en donde se guardan las mismas l'ineas que en el history pero luego de ser separadas por espacios, con una parte del parser adelantada, "
    "la instrucci'on correspondiente a la l'inea que marca el again se guarda y se ejecuta llamando al m'etodo encargado de terminar de parsearla y ejecutarla.\n\n");

}

void help_if()
{
    printf("\n Se puede utilizar el comando condicional if al igual que then que significa lo que se ejecuta si el if resulta true, en este caso si se ejecut'o correctamente el proceso"
    " que estaba dentro del if, o sea devuelve 0, el else se ejecuta si lo que est'a dentro del if no se ejecuta correctamente, o sea devuelve 1, y el end que significa el final del if actual. "
    "En el caso de aparecer varios if en una misma instrucci'on solo se toma el primero como comando, los dem'as se toman como argumentos del comando anterior,"
    "en el caso de que aparezca un then sin haber aparecido anteriormente un if, es tomado igualmente como argumento, al igual ocurre con el else y el end, que si no se han encontrado los comandos necesarios para"
    " que estos se puedan ejecutar ser'an tomados como argumentos. Primeramente se ejecuta el codigo que de la condicion del if, si este devuelve 0 entonces entramos en la parte del Then, sino entramos a la parte del else.\n\n");
}

void help_variable()
{
    printf("\n Se Implemento el comando set para agregar variables con sus respectivos valores y pedir informacion sonbre las variables agregadas con sus valores en caso de pasar el set sin argumentos."
    "Puede utilizarse get nombredelavariable para pedir el valor correspondiente a la variable y unset nombredelavariable para eliminar la variable de la lista de las variables guardadas"
    "Todas estas variables son guardadas en un a lista doblemente enlazada, cuyo valor es de tipo keyvaluepair y cada nodo contiene el par llave-valor de cada variable agregada. \n\n");
}

int HELP_CODE(node* argument)
{
    Expression * next;
    if(argument->next != NULL)
        next = argument->next->value;
    Expression * node_temp = argument->value;
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
    else if(strcmp("history",next->name)==0)
    {
        help_history(); return 0;
    }

    else if(strcmp("spaces",next->name)==0)
    {
        help_spaces();    return 0;

    }
    else if(strcmp("multi-pipe",next->name)==0)
    {
        help_multipipe();    return 0;

    }

    else if(strcmp("ctrl+c",next->name)==0)
    {
        help_controlc(); return 0;
    }
     else if(strcmp("if",next->name)==0)
    {
        help_if(); return 0;
    }
     else if(strcmp("variables",next->name)==0)
    {
        help_variable(); return 0;
    }
    else
    {
         printf("invalid help command `%s'\n",next->name);
         return 1;
    }
    return 0;

}
