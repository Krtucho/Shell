#include <stdio.h>
#include <string.h>
#include "list.h"




int SpecialCaracter(char c)
{
    //if(c=='#')return 1;
    //if(c==';')return 1;
    if(c=='&')return 1;
    if(c=='|')return 1;
    if(c=='<')return 1;
    if(c=='>')return 1;
    return 0;
}
int SpecialCaracters(char* c)
{
    //if(c=='#')return 1;
    //if(c==';')return 1;
    if(c=="&")return 1;
    if(c=="|")return 1;
    if(c=="<")return 1;
    if(c==">")return 1; 
    if(c=="&&")return 1;
    if(c=="||")return 1;
    if(c==">>")return 1; 
    return 0;
}

void EjecuteLine(list* line)
{

}



void ConcatChar(char c, char *chain)
{
    char temp[2];
    temp[0] = c;
    temp[1] = '\0';
    strcat(chain, temp);
}

int main()
{
     while (1)
    {
        printf("\n my-shell $ ");


        char word[0];  ////word es cada una de las palabras que se mandan en un espacio de line
        word[0] = '\0';
        
        list* line=init_list("");  ///line es la linked list que guarda los comandos argumentos y caracteres especiales
        


        char c=getchar();  ///cada uno de los char a leer de consola    
        while(c!= '\n')
        {
            if(c=='#')//ya
            {
                break;
            }
            if(c==';')//ya
            {
                ////////Primero terminar de armar la linea actual, y mandarla a ejecutar
                push_back(line, word);
                EjecuteLine(line);
            }
            if(SpecialCaracter(c))///pensar bien
            {            
                if(word==NULL)ConcatChar(word,c);//Si la palabra que tengo hasta el momento es vacia es porque se dejo un espacio y solo tengo que crear una nueva para el caracter especial
                if(word[1]==NULL && SpecialCaracter(word[0]))ConcatChar(word,c);//Si lo que tengo es un pedazo de caracter, lo completo
                
            }

            if(c==' ')
            {
                if(word==NULL)
                {
                    c=getchar();
                    continue;
                    
                }//Si la palabra que tengo hasta el momento es vacia continuo(multiples espacios)
                push_back(line, word);
                word[1];//
                word[0]=c;



            }


            if(SpecialCaracters(word))///Si es un caracter normal pero esta pegado a un caracter especial anterior
            {
                push_back(line, word);//saco el caracter especial para la lista y renuevo a word
                word[1];
                word[0]='\0';
                
            }
            ConcatChar(c,word);


            c=getchar();
        }
        push_back(line, word);////Primero poner la ultima palabra conformada y luego ejecutar la linea
        EjecuteLine(line);

        //printf("%s",word);




    }
}




