#include "bin_tree.h"


void AddLeftNode(TreeNode * hostNode, TreeNode * commandNode){
    hostNode->left = commandNode;
}