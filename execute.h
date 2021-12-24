#include "expression.h"

int Execute(node * first_cmd, node * last_cmd);
node * SearchBiggerRedir(node * last_cmd);
int SolveBiggerRedir(node * first_cmd, node * last_cmd);
node * SolveLessRedir(node * first_cmd, node * last_cmd);
int Solve_Leaves(node * first_cmd, node * last_cmd);
node * SolveCommands(node * first_cmd, node * last_cmd);
void ExecuteCommand(node * cmd_to_exec);
void Run(node * com);