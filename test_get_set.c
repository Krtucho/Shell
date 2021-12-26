#include <stdlib.h>
#include <stdio.h>
#include <string.h>




int main(int argc, char const *argv[])
{

    char * a = strdup("a");
    char * b = strdup("`ls`");

    int result = setenv(a, b, 1);
    
    printf("%s\n", getenv(a));


    int removed = unsetenv(a);
    int removed2 = unsetenv(a);

    printf("%d\n", result);
    char * c = getenv(a);
    if(c != NULL)
        printf("%s\n", getenv(a));
    else
        printf("NULL");

    return 0;
}
