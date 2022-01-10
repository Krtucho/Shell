#include "list.h"
#include<stdbool.h>// Booleanos

enum OPERATORS{ // Enum con todos los tipos de operadores utilizados en el codigo
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


typedef struct Expression // Struct creado para guardar todos los tipos de tokens que podemos encontrarnos, tendran un nombre y un enum del tipo OPERATOR para diferenciarlos
{
    char * name;
    list * args;
    char * std_in;
    char * std_out;
    bool calculated;
    enum OPERATORS operators;
}Expression;

#pragma endregion

#pragma region KeyValuePair
typedef struct keyvaluepair{ // Struct creado para guardar todos los pares de llave-valor que se guardaran en la lista de variables con sus valores para implementar el get set
    void * key; // Nombre de la variable
    void * value; // Valor guardado en la variable
}keyvaluepair;
#pragma endregion