#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int extractArgIndex(char line[], char arg[], int * length)
{
    int i;
    for(i = 0 ; line[i] != '\0' ; i++)
    {
        if(line[i] == '\'' || line[i] == ',' || line[i] == ' ')
        {
            arg[i] = '\0';
            break;
        }
        else
            arg[i] =  line[i];
    }
    *length = i;
    return atoi(arg) - 1;
}

void convertPNToAN(char line[], char args[10][100], int argsCount, char outputLine[100])
{
    int i, j = 0, length = 0, argPos, k = 0;
    char arg[100];
    for(i = 0; line[i] != '\0'; )
    {
        if(line[i] == '?')
        {
            i++;
            argPos = extractArgIndex(line + i, arg, &length);
            k = 0;
            while(args[argPos][k] != '\0')
                outputLine[j++] = args[argPos][k++];
            i += length;
        }
        else
            outputLine[j++] = line[i++];
    }
    outputLine[j] = '\0';
}

int main()
{
    char line[] = {"LDA X'?1' ssss"};
    char args[][100] = {"ssss", "ss2s"};
    char outputLine[100];
    convertPNToAN(line, args, 2, outputLine);
    printf("%s\n", outputLine);
    return 0;
}