#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    char buffer[256];
    strcpy(buffer, "The summer soldier,the sunshine patriot");
    char *p = strtok(buffer, " ");
    while (p != NULL)
    {

        printf("%s---", p);
        p = strtok(NULL, " ");
    }
    return 0;
}
