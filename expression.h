#include "list.h"
#include<stdbool.h>// Booleanos

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
    LEAF=9,
    IF_ELSE=10,
    REDIRLESS=11,
    ARCHIVE=12,
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


// typedef struct TreeNode {
//     Expression value;
//     struct TreeNode *left;
//     struct TreeNode *right;
// }TreeNode;

// struct Tree{
//     TreeNode root;
//     int count;
// }Tree;

// void AddLeftNode(TreeNode * hostNode, TreeNode * commandNode);