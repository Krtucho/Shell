#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    FILE * fp;

    fp = fopen("a.txt", "w+");
    fprintf(fp,"algo mas bonito");
    fclose(fp);
    return 0;
}
