#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    FILE * fp;

    printf("AAA stdout \n");

    char * a = strdup("DDDDDDD");
    fp = freopen("file.txt", "w", stdout);
    fclose(fp);

    // printf("AAA file.txt\n");

    // printf("CCC\n");
    // printf("\n%s \n", stdin);
    

    return 0;
}