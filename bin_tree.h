#include "list.h"
#include<stdbool.h>// Booleanos

enum OPERATORS{
    AND = 0,
    OR = 1,
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
    SIMPLE_COMMAND=13
}OPERATORS;

typedef struct Command// TODO: Cambiar nombre a: Expression
{
    char * name;
    list * args;
    char * std_in;
    char * std_out;
    bool calculated;
    enum OPERATORS operators;
}Command;


typedef struct TreeNode {
    Command value;
    struct TreeNode *left;
    struct TreeNode *right;
}TreeNode;

struct Tree{
    TreeNode root;
    int count;
}Tree;

void AddLeftNode(TreeNode * hostNode, TreeNode * commandNode);