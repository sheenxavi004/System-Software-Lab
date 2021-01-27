#include <stdio.h>
#include <stdlib.h>

int intValidator(char * str)
{
    int val = atoi(str);
    if(val == 0 && str[0] == '0')
        return 0;
    if(val == 0)
        return -1;
    return val;
}
int main()
{
    FILE *ptr;
    FILE *ptrW;
    ptr = fopen("input.txt", "r");
    int i, j;
    char str[100], part[100];
    while(fscanf(ptr, " %[^\n]", str) != EOF)
    {
        i = 0;
        while(str[i] != '\0')
        {
            if(str[i] == '\t')
                j++;
            i++;
        }
    }
    printf("%d", j);
    fclose(ptr);
    return 0;
}