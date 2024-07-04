#include <stdio.h>
#include <string.h>

int main()
{
    char s[20] = "abcde";

    printf("size %d strlen %d\n", sizeof(s), strlen(s));

    char *s2 = "12345";
    printf("size %d strlen %d s2[0]%c s2[1]%c\n", sizeof(s2), strlen(s2), s2[0], s2[1]);
}