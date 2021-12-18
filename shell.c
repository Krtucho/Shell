#include <stdio.h>
#include <string.h>
#include "list.h"
#include <stdbool.h>


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
            ConcatChar(c,word);
            c=getchar();
        }

        push_back(line, strdup(word));
        strcpy(word,"");
        free(word);
        pop_front(line);
        print_list(line);
        free_list(line);

    }


}

int main(int argc, char const *argv[])
{
    Shell();
    return 0;
}
