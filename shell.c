#include <stdio.h>
#include <string.h>
//#include "list.h"
#include <stdbool.h>
#include "execute.h"

#pragma region Utiles

int SpecialCaracter(char c)
{
    if(c=='&')return 1;
    if(c=='|')return 1;
    if(c=='<')return 1;
    if(c=='>')return 1;
    return 0;
}
int SpecialCaracters(char* c)
{
    if(strcmp(c,"&")==0)return 1;
    if(strcmp(c,"|")==0)return 1;
    if(strcmp(c,"<")==0)return 1;
    if(strcmp(c,">")==0)return 1;
    if(strcmp(c,"&&")==0)return 1;
    if(strcmp(c,"||")==0)return 1;
    if(strcmp(c,">>")==0)return 1;
    return 0;
}
void ConcatChar(char c, char *chain)
{
    char temp[2];
    temp[0] = c;
    temp[1] = '\0';
    strcat(chain, temp);
}

#pragma endregion



Expression* GetExpression(char* word)
{
    Expression * exp = (Expression*)malloc(sizeof(Expression));
    exp->name=strdup(word);
    exp->operators = SIMPLE_EXPRESSION;
    if(strcmp(word,"&&")==0)exp->operators = AND;
    else if(strcmp(word,"||")==0)exp->operators = OR;
    else if(strcmp(word,"|")==0)exp->operators = PIPE;
    else if(strcmp(word,"if")==0)exp->operators = IF;
    else if(strcmp(word,">")==0)exp->operators = REDIRBIG;
    else if(strcmp(word,"then")==0)exp->operators = THEN;
    else if(strcmp(word,"else")==0)exp->operators = ELSE;
    else if(strcmp(word,"end")==0)exp->operators = END;
    //else if(strcmp(word,"&")==0)exp->operators = Com;///////////que significa com
    //else if(strcmp(word,"&")==0)exp->operators = LEAF;////////ver que poner en hoja
    else if(strcmp(word,"&")==0)exp->operators = IF_ELSE;/////////verificar lo del if else
    else if(strcmp(word,"<")==0)exp->operators = REDIRLESS;
    else if(strcmp(word,"&")==0)exp->operators = ARCHIVE;///////////ver como comprobar que sea .txt
    else exp->operators =SIMPLE_EXPRESSION;
    return exp;
}
list* CreateExpressionList(list* string_list)
{

}

void EjecuteLine(list* line)
{

}

void Shell()
{

while (1)
    {
        printf("\n my-shell $ ");

        char * word= (char*)calloc(sizeof(char),100);//word es cada una de las palabras que se mandan en un espacio de line
        strcpy(word,""); 

        list* line=init_list("init"); //line es la linked list que guarda los comandos argumentos y caracteres especiales
        //pop_front(line);

        char c=getchar();  //cada uno de los char a leer de consola   
        
//        bool concat=true;

        while(c!= '\n')
        {
         
            if(c=='#')
            {
                 while(c!= '\n')
                 {
                     c=getchar();
                 }
                break;
            }

            if(c=='&')
            {
                /////mandar el proceso actual a background

            }

            if(c==';')
            {
                ////////Primero terminar de armar la linea actual, y mandarla a ejecutar
                push_back(line, strdup(word));
                strcpy(word,"");
                pop_front(line);
                EjecuteLine(line);
                print_list(line);                
                //pop_back(line);
                free_list(line);
                line=init_list("init");

                c=getchar();
                continue;
            }
            
            if(SpecialCaracter(c))
            {            
                if(word[0]=='\0' ||SpecialCaracter(word[0]) )//Si la palabra que tengo hasta el 
                //momento es vacia es porque se dejo un espacio y tengo que crear una nueva 
                //para el caracter especial o en el caso que se este conforrmando un caracter especial,
                //lo termino de conformar.
                {
                    ConcatChar(c,word);
                    if(!SpecialCaracters(word))
                    {
                        printf("syntax error near unexpected token '%s'",c);
                        return;
                    }
                    c=getchar();
                    continue;
                }
                   push_back(line, strdup(word));
                   strcpy(word,"");
                   ConcatChar(c,word);
                   c=getchar();
                   continue;      
                
            }

            if(SpecialCaracters(word))///Si es un caracter normal pero esta pegado a un caracter especial anterior
            {
                push_back(line, strdup(word));//saco el caracter especial para la lista y renuevo a word
                strcpy(word,"");
                ConcatChar(c,word);
                c=getchar();
                continue;
                
            }
            
            if(c==' ')
            {
                if(word[0]=='\0')
                {
                    c=getchar();
                    continue;
                    
                }
                push_back(line, strdup(word));

                strcpy(word,"");///vacio a word para comenzar una nueva palabra
                c=getchar();
                continue;
            }
            
            //if(concat)
            ConcatChar(c,word);
            c=getchar();
        }

        push_back(line, strdup(word));
        pop_front(line);
        EjecuteLine(line);
        strcpy(word,"");
        free(word);        
        //print_list(line);
        free_list(line);

    }



}

int main(int argc, char const *argv[])
{
    Shell();
    return 0;
}

