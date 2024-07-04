#include <stdio.h>

int main(int argc, char const *argv[])
{
    // char buffer[256];
    // char a;
    // if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    // {
    //     printf("%s\n", buffer);
    // }
    // scanf("%s", buffer);
    // printf("%s\n", buffer);

    int character;
    printf("Enter some characters (Ctrl+D to end on Unix/Linux, Ctrl+Z on Windows):\n");

    // Read characters from stdin until EOF is encountered
    while ((character = fgetc(stdin)) != EOF)
    {
        // Print the character
        printf("You entered: %c\n", character);
    }

    printf("End of input.\n");

    return 0;
}
