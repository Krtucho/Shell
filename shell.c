#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <search.h>


#pragma region Lista

#define __ReservedMemoryforNode (node*)malloc(sizeof(node));
#define __SafeMemoryReserved(x) if(x == NULL) exit(1);
#define __SafeMemoryFree(x) if(x!=NULL) free(x);

typedef struct node
{
    void * value;
    struct node * previous;
    struct node * next;
} node;

typedef struct list
{
    node* head;
    node* tail;
    unsigned int size;
} list;




list* init_list(void* v)
{
    list* l = (list*)malloc(sizeof(list));
    __SafeMemoryReserved(l)
    node *first = __ReservedMemoryforNode
    __SafeMemoryReserved(first)
    first->previous = NULL;
    first->next = NULL;
    first->value = v;
    l->head = first;
    l->tail = first;
    l->size = 1;
    return l;
}
void push_back(list* l, void* v)
{
    node* insert = __ReservedMemoryforNode
    __SafeMemoryReserved(insert)
    if(l->size==0)
    {
        insert->previous = NULL;
        insert->next = NULL;
        insert->value = v;
        l->head = insert;
        l->tail = insert;
        l->size = 1;
        return;
    }
    insert->value = v;
    insert->previous = l->tail;
    insert->next = NULL;
    l->tail->next = insert;
    l->tail = insert;
    l->size++;
}
void push_front(list* l, void* v)
{
    node* insert = __ReservedMemoryforNode
    __SafeMemoryReserved(insert)
    if(l->size==0)
    {
        insert->previous = NULL;
        insert->next = NULL;
        insert->value = v;
        l->head = insert;
        l->tail = insert;
        l->size = 1;
        return;
    }
    insert->value = v;
    insert->next = l->head;
    insert->previous = NULL;
    l->head->previous = insert;
    l->head = insert;
    l->size++; 
}
node* pop_back(list* l)
{
    if(l->size == 0) return NULL;
    node* last = l->tail;
    l->tail = l->tail->previous;
    if(l->tail != NULL)
    {
        l->tail->next = NULL;
    }
    l->size--;
    return last;
}
node* pop_front(list* l)
{
    if(l->size == 0) return NULL;
    node* first = l->head;
    l->head = l->head->next;
    if(l->head != NULL)
    {
        l->head->previous = NULL;
    }
    l->size--;
    return first;
}
node* getAt(list* l, int ind)
{
    if(ind<0 || ind>=l->size) 
    {
        printf("Index out of range."); 
        exit(1);
    }
    node* current = l->head;
    for (size_t i = 0; i < ind; i++, current = current->next);
    return current;   
}
void insert(list* l, void* v, int ind)
{
    if(ind<0 || ind>=l->size) 
    {
        printf("Index out of range."); 
        return;
    }
    if(ind==0) 
    {
        push_front(l, v); 
        return;
    }
    if(ind==l->size-1)
    {
        push_back(l,v); 
        return;
    }
    node* insert = __ReservedMemoryforNode
    __SafeMemoryReserved(insert)
    insert->value = v;
    node* lnode = l->head;
    node* rnode = l->head->next;
    for(size_t i = 1; i < ind; i++)
    {
        lnode = rnode;
        rnode = rnode->next;
    }
    lnode->next = insert;
    insert->previous = lnode;
    insert->next = rnode;
    rnode->previous = insert;
    l->size++;
}
void removeNode(list* l, node* item)
{
    if(l->size == 0) return;
    if(item->previous == NULL) pop_front(l);
    else if (item->next == NULL) pop_back(l);
    else
    {
        node* prev = item->previous;
        node* next = item->next; 
        prev->next = next;
        next->previous = prev;
        l->size--;   
    }
}

void free_list(list* l)
{
    node* temp = l->head->next;
    node* current = l->head;
    while(current!=NULL)
    {
        free(current);
        current = temp;
        if(temp != NULL)
        {
            temp = temp->next;
        }
    }
    free(l);
}

void print_list(list * t) 
{
    node * current = t->head;
    printf("Imprimiendo lista...\n");
    while (current != NULL) {
        printf("%s ", (char*)current->value);
        current = current->next;
    }
    printf("\n");
}


#pragma endregion

#pragma region Expression
enum OPERATORS{
    TRUE = 0,
    FALSE = 1,
    SIMPLE_EXPRESSION=2,
    EXIT=3,
    CD=4,
    HISTORY=5,
    HELP=6,
    GET=7,
    UNSET=8,
    SET=9,
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


typedef struct Expression// TODO: Cambiar nombre a: Expression
{
    char * name;
    list * args;
    char * std_in;
    char * std_out;
    bool calculated;
    enum OPERATORS operators;
}Expression;

#pragma endregion

#pragma region Utiles

static char *special_strings[] = {"true","false","exit","cd","history","help","set","`","get","unset","&&","||",
                        "|","if",">","then","else","end","<",">>" };

static int specials_operators[] = { TRUE, FALSE, EXIT,CD, HISTORY, HELP,SET,SET_CHARACTER, GET, UNSET,AND , OR,
                PIPE,IF,REDIRBIG,THEN, ELSE,END,REDIRLESS,DOUBLEREDIRBIG};

void ConcatChar(char c, char *chain)
{
    char temp[2];
    temp[0] = c;
    temp[1] = '\0';
    strcat(chain, temp);
}

int GetIndexOF(char* word)
{
    for (int i = 0; i < 19/*sizeof(special_strings)*/; i++)
    {
        if(strcmp(special_strings[i],word)==0)return i;
    }
    return -1;
}

int GetOperator(char* word)
{
    int index=GetIndexOF(word);
    if(index==-1)
    {
        return SIMPLE_EXPRESSION;
    }
    return specials_operators[index];
}

char GetOneChar(char* strline,bool history)
{
    char c=getchar();
    if(history)ConcatChar(c,strline);
    return c;
}

void EndReadLine(char* strline,bool history)
{
    char c;//=GetOneChar();
    while(c!= '\n')    
    {
        c=GetOneChar(strline,history);
    }
}

Expression* GetStructExpression(char* name, int op)
{
    Expression * exp = (Expression*)malloc(sizeof(Expression));//para guardar la primera expresion
    exp->name=name;
    exp->operators=op;
    return exp;
}


#pragma endregion

#pragma region Clasificacion de Caracteres

int SpecialCaracter(char c)//devuelve si es un caracter especial simple
{
    //if(c=='#')return 1;
    //if(c==';')return 1;
    if(c=='&')return 1;
    if(c=='|')return 1;
    if(c=='<')return 1;
    if(c=='>')return 1;
    if(c=='`')return 1;
    return 0;
}

int SpecialCaracters(char* c)//devuelve si es un caracter especial en general
{
    //if(c=='#')return 1;
    //if(c==';')return 1;
    //if(strcmp(c,"&")==0)return 1;///////////not implemented
    if(strcmp(c,"|")==0)return 1;
    if(strcmp(c,"<")==0)return 1;
    if(strcmp(c,">")==0)return 1;
    if(strcmp(c,"`")==0)return 1;
    if(strcmp(c,"&&")==0)return 1;
    if(strcmp(c,"||")==0)return 1;
    if(strcmp(c,">>")==0)return 1;
    // if(c=="|")return 1;
    // if(c=="<")return 1;
    // if(c==">")return 1; 
    // if(c=="&&")return 1;
    // if(c=="||")return 1;
    // if(c==">>")return 1; 
    return 0;
}

bool RedirCaracter(char* c)//devuelve si es un caracter especial de los que puede ir sin un comando delante
{
    if(strcmp(c,"<")==0||strcmp(c,">")==0||strcmp(c,">>")==0)return true;
    return false;

}

bool IfCommand(char* c)//devuelve si es una expresion del tipo if, else then o end
{
    if(strcmp(c,"if")==0||strcmp(c,"then")==0||strcmp(c,"else")==0||strcmp(c,"end")==0)return true;
    return false;
}


#pragma endregion

#pragma region Ejecucion


void EjecuteLine(list* line)
{
    
    bool command=true;//indica si en este momento debo recibir un comando
    bool archive=false;//indica si en este momento debo recibir un archivo
    bool special_caracter_last=false;//indica si la palabra anterior fue un caracter especial
    bool if_caracter_last=false;//indica si la expresion anterior es de tipo if, else then o end
    
    Expression * exp1 = (Expression*)malloc(sizeof(Expression));//para guardar la primera expresion
       
    char* temp = pop_front(line)->value; //guarda temporalmente el primer valor de la lista de string
    
    if(SpecialCaracters(temp)&& !RedirCaracter(temp))////cuando como primera palabra tenemos un caracter especial que no puede ocupar ese lugar
    {
        printf("syntax error near unexpected token `%s'",temp);
        return;
    }

    exp1->name=strdup(temp);
    exp1->operators = GetOperator(temp);

    list* exp_line=init_list(exp1); //linea de expresiones clasificadas
    printf("%s:%d\n",exp1->name,exp1->operators);

    //free(exp1);///verificar si esta bien liberar aqui
    //printf("%s:%d",exp_line->head->value->name,exp1->operators);

    command=false;
    if(SpecialCaracters(temp)){command=true;special_caracter_last=true;}
    if(IfCommand(temp))command=true;
    if(RedirCaracter(temp)) archive=true;
    //free(temp);//verificar si esta bien liberar aqui

    //node* current=exp_line->head;

    //printf("%s:%d",exp1->name,exp1->operators);


    while(line->head!=NULL)
    {
        temp=line->head->value;
        //push_back(exp_line,(Expression*)malloc(sizeof(Expression)));
        //current=current->next;
        //current->value->name=temp;

        //Expression * exp = (Expression*)malloc(sizeof(Expression));//para crear cada expresion de la lista de expresiones
        char* name=strdup(temp);
        bool special_caracter_now=SpecialCaracters(temp);//la expresion actual es un caracter especial
        if(special_caracter_last && special_caracter_now){printf("syntax error near unexpected token `%s'",temp); return;}
        if(if_caracter_last && !RedirCaracter(temp)){printf("syntax error near unexpected token `%s'",temp); return;}
        if(special_caracter_last)command=true;

        //exp->operators=GetOperator(temp);
        int op=GetOperator(temp);
        //if(exp->operators==SIMPLE_EXPRESSION)
        if(op==SIMPLE_EXPRESSION)
        {
            //if(!command)exp->operators=ARGS;//Si no estamos esperando un comando entonces estamos en presencia de un argumento
            //if(archive)exp->operators=ARCHIVE;//Si la expresion anterior es un caracter especial de redireccion shora estamos en presencia de un archivo
            if(!command)op=ARGS;//Si no estamos esperando un comando entonces estamos en presencia de un argumento
            if(archive)op=ARCHIVE;//Si la expresion anterior es un caracter especial de redireccion shora estamos en presencia de un archivo


        }    
        command=false;
        archive=false;
        if(RedirCaracter(temp)) archive=true;

        special_caracter_last=special_caracter_now;
        push_back(exp_line,GetStructExpression(name, op));
        //printf("%s:%d\n",exp->name,exp->operators);
        printf("%s:%d\n",name,op);

        free(pop_front(line));
    
    }

}

void ReadAndEjecuteLine(list* line,char* word, char c)//crea una lista de string por cada instruccion y luego manda a ejecutarlas
{
    bool history=true;
    if(c==' ')history=false;
    char *strline=(char*)calloc(sizeof(char),500);
    if(history)ConcatChar(c,strline);

    while(c!= '\n')
        {
         
            if(c=='#')
            {
                EndReadLine(strline,history);
                break;
            }

            if(c=='&')
            {
                if(strcmp(word,"")!=0)
                {
                    push_back(line, strdup(word));
                    strcpy(word,"");
                }
                ConcatChar(c,word);                    
                c=GetOneChar(strline,history);

                if(c!='&')
                {
                    EndReadLine(strline,history);//Hacer el background
                }
                /////mandar el proceso actual a background
                /////verificar que primero se busca un and para despues comprobar que sea un &
            }

            if(c==';')
            {
                ////////Primero terminar de armar la linea actual, y mandarla a ejecutar
                if(strcmp(word,"")==0||strcmp(line->head->value,"init")==0)
                {
                    printf("syntax error near unexpected token `;'");
                    return;
                }
                push_back(line, strdup(word));
                strcpy(word,"");
                pop_front(line);
                EjecuteLine(line);
                //print_list(line);                
                //pop_back(line);
                free_list(line);
                line=init_list("init");

                c=GetOneChar(strline,history);
                //c=GetOneChar();
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
                        printf("syntax error near unexpected token '%c' \n",c);
                        strcpy(word,"");
                        free_list(line);
                        line=init_list("init");
                        EndReadLine(strline,history);
                        return;
                    }
                    c=GetOneChar(strline,history);
                    continue;
                }
                
                   push_back(line, strdup(word));
                   strcpy(word,"");
                   ConcatChar(c,word);
                   
                   c=GetOneChar(strline,history);
                   //c=GetOneChar();
                   continue;      
                
            }

            if(SpecialCaracters(word))///Si es un caracter normal pero esta pegado a un caracter especial anterior
            {
                if(c==' ')c=GetOneChar(strline,history);
                push_back(line, strdup(word));//saco el caracter especial para la lista y renuevo a word
                strcpy(word,"");
                ConcatChar(c,word);
                c=GetOneChar(strline,history);
                //c=GetOneChar();
                continue;
                
            }
            
            if(c==' ')
            {
                if(word[0]=='\0')
                {
                    c=GetOneChar(strline,history);
                    //c=GetOneChar();
                    continue;
                    
                }
                push_back(line, strdup(word));

                strcpy(word,"");///vacio a word para comenzar una nueva palabra
                c=GetOneChar(strline,history);
                //c=GetOneChar();
                continue;
            }
            
            //if(concat)
            ConcatChar(c,word);
            c=GetOneChar(strline,history);
            //c=GetOneChar();
        }

        push_back(line, strdup(word));
        strcpy(word,"");
        free(word);
        pop_front(line);
        //print_list(line);
        EjecuteLine(line);
        //free_list(line);

    //printf("\n HISTORY: %s \n",strline);
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
        
        //bool concat=true;

        ReadAndEjecuteLine(line,word,c);
    }

}

#pragma endregion

int main(int argc, char const *argv[])
{
    Shell();
    return 0;
}
