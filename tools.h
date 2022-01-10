#include "help.h" 




static char *special_strings[] = {"true","false","exit","cd","history","help","again","set","`","get","unset","&&","||",
                        "|","if",">","then","else","end","<",">>" };

static int specials_operators[] = { TRUE, FALSE, EXIT,CD, HISTORY, HELP,AGAIN,SET,SET_CHARACTER, GET, UNSET,AND , OR,
                PIPE,IF,REDIRBIG,THEN, ELSE,END,REDIRLESS,DOUBLEREDIRBIG};


void ConcatChar(char c, char *chain);
int GetIndexOF(char* word);
int GetOperator(char* word);
char GetOneChar(char* strline,bool history);
void EndReadLine(char* strline,bool history);
Expression* GetStructExpression(char* name, int op);
int SpecialCaracter(char c);
int SpecialCaracters(char* c);
bool RedirCaracter(char* c);
bool IfCommand(int op);
bool PossibleArgumentExpression(int op);