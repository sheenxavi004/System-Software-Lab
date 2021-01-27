//Name   : twopassmacro.c
//Desc   : Program which performs performs two pass macro processing on provided input.
//Input  : The Program which contains the program file is provided as input.
//Output : It displays the contents of NAMTAB and DEFTAB after Pass 1 and the contents
//         of output file which contains the macro processed code.
//Author : Sheen Xavier A
//Date   : 28 / 11 / 2020

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Data structure for NAMTAB
typedef struct{
    char name[100];
    char ARGSTAB[10][100];
    int argsCount;
    int startPosition, endPosition;
    int flag;
}namtype;

//Data structure for DEFTAB
typedef struct{
    char line[100];
}deftype;

//Function to seperate lines into different sections based on seperating character
int lineSeperator(char *line, char seperator, char section[10][100])
{
    int i = 0, j = 0, sectionNumber = 0;
    while(line[i] != '\0')
    {
            if(line[i] == seperator)
            {
                section[sectionNumber][j] = '\0';
                j = 0;
                sectionNumber++;              
            }
            else
            {
                section[sectionNumber][j++] = line[i];
            }
            i++;    
    }
    section[sectionNumber][j] = '\0';
    return sectionNumber + 1;
}

//Function used for extracting the argument in the definition
int extractArg(char line[], char arg[])
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
    return i;
}

//Function which generates the positional notation form for macro
void convertLineToPN(char line[], char args[10][100], int argsCount, char outputLine[100])
{
    int i, j = 0, length = 0, k;
    char arg[100];
    for(i = 0; line[i] != '\0'; )
    {
        if(line[i] == '&')
        {
            outputLine[j++] = '?';
            length = extractArg(line + i, arg);
            for(k = 0 ; k < argsCount ; k++)
            {
                if(!(strcmp(args[k], arg)))
                    break;
            }
            outputLine[j++] = 49 + k; 
            i += length;
        }
        else
            outputLine[j++] = line[i++];
    }
    outputLine[j] = '\0';
}

//Function for extracting argument number based on postional notation
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

//Function for substituting arguments instead of positional parameters
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

//Hash function for NAMTAB hash table implementation
int hash(char *label)
{
    int i = 0;
    int hashValue = 0;
    while(label[i] != '\0')
    {
        hashValue += (i + 1) * (label[i] - 65 + 1);
        i++;
    }
    return hashValue;
}

int main()
{
    FILE *ptr;
    FILE *ptrW;
    int i, j, expanding = 0, hashValue, defTabIndex = 0, level, argCount, length;
    char fname[100], line[100], section[10][100], positionalNotation[100], ARGTAB[10][100], argumentNotation[100];
    //Initializing the OPTAB and SYMTAB
    namtype NAMTAB[2000];
    deftype DEFTAB[2000];

    //Initializing the NAMTAB
    for(i = 0 ; i < 2000 ; i++)
        NAMTAB[i].flag = 0;
    
    //Reading the name of the file which contains the input code
    printf("FILENAME : ");
    scanf(" %[^\n]", fname);
    ptr = fopen(fname, "r");

    fscanf(ptr, " %[^\n]", line);
    lineSeperator(line, ' ', section);

    //Pass 1 : Defining the Macros and Populating the NAMTAB and DEFTAB
    while(strcmp("END", section[1]))
    {   
        if(!(strcmp("MACRO", section[1])))
        {
            hashValue = hash(section[0]);
            strcpy(NAMTAB[hashValue].name, section[0]);
            NAMTAB[hashValue].startPosition = defTabIndex;
            NAMTAB[hashValue].flag = 1;
            sprintf(line, "%s", section[0]);
            length = strlen(section[0]);
            sprintf(line + length, " %s", section[2]);
            strcpy(DEFTAB[defTabIndex++].line, line);
            NAMTAB[hashValue].argsCount = lineSeperator(section[2], ',', NAMTAB[hashValue].ARGSTAB);
            level = 1;
            while(level > 0)
            {
                fscanf(ptr, " %[^\n]", line);
                lineSeperator(line, ' ', section); 
                if(line[0] != '.')
                {
                    convertLineToPN(line, NAMTAB[hashValue].ARGSTAB, NAMTAB[hashValue].argsCount, positionalNotation);
                    strcpy(DEFTAB[defTabIndex++].line, positionalNotation + 2);
                    if(!(strcmp(section[1], "MACRO")))
                        level++;
                    else if(!(strcmp(section[1], "MEND")))
                        level--;
                }
            }
            NAMTAB[hashValue].endPosition = defTabIndex - 1;
        }
        fscanf(ptr, " %[^\n]", line);
        lineSeperator(line, ' ', section);       
    }
    fclose(ptr);

    //Pass 2 : Expanding the macro invocations in the program
    ptr = fopen(fname, "r");
    ptrW = fopen("output.txt", "w");

    fscanf(ptr, " %[^\n]", line);
    lineSeperator(line, ' ', section);

    while(strcmp("END", section[1]))
    {
        if(!(strcmp("MACRO", section[1])))
        {
            level = 1;
            while(level > 0)
            {
                fscanf(ptr, " %[^\n]", line);
                lineSeperator(line, ' ', section); 
                if(!(strcmp(section[1], "MACRO")))
                    level++;
                else if(!(strcmp(section[1], "MEND")))
                    level--;
            }
        }
        else
        {
            hashValue = hash(section[1]);
            if(NAMTAB[hashValue].flag)
            {
                fprintf(ptrW, ".%s\n", line);
                lineSeperator(section[2], ',', ARGTAB);
                for(j = NAMTAB[hashValue].startPosition + 1 ; j < NAMTAB[hashValue].endPosition ; j++ )
                {
                    convertPNToAN(DEFTAB[j].line, ARGTAB, NAMTAB[hashValue].argsCount, argumentNotation);
                    lineSeperator(argumentNotation, ' ', section);                     
                    fprintf(ptrW, "-\t%s\t%s\n", section[0], section[1]);
                } 
            }
            else
                fprintf(ptrW, "%s\t%s\t%s\n", section[0], section[1], section[2]);
        }
        fscanf(ptr, " %[^\n]", line);
        lineSeperator(line, ' ', section); 
    }
    fprintf(ptrW, "%s\t%s\t%s\n", section[0], section[1], section[2]);
    fclose(ptrW);
    fclose(ptr);

    printf("******************\n");
    printf("CONTENTS OF NAMTAB\n");
    printf("******************\n");
    for(i = 0 ; i < 2000 ; i++)
    {
        if(NAMTAB[i].flag)
        {
            printf("NAME: %s\n", NAMTAB[i].name);
            printf("ARGUMENTS: ");
            for(j = 0 ; j < NAMTAB[i].argsCount - 1 ; j++)
                printf("%s, ", NAMTAB[i].ARGSTAB[j] + 1);
            printf("%s\n", NAMTAB[i].ARGSTAB[NAMTAB[i].argsCount - 1] + 1);
            printf("MACRO LENGTH: %d\n", NAMTAB[i].endPosition - NAMTAB[i].startPosition + 1);
            printf("******************\n");
        }
    }
    printf("******************\n");
    printf("CONTENTS OF DEFTAB\n");
    printf("******************\n");
    for (i = 0; i < defTabIndex; i++)
        printf("%s\n", DEFTAB[i].line);
    
    ptr = fopen("output.txt", "r");
    printf("***************************************\n");
    printf("THE CONTENTS OF MACRO PROCESSED PROGRAM\n");
    printf("***************************************\n");
    while(fscanf(ptr, " %[^\n]", line) != EOF)
        printf("%s\n", line);
    printf("***************************************\n");
    fclose(ptr);
    return 0;
}