//Name   : pass1.c
//Desc   : Program which simulates the Pass 1 assembling in a 2 Pass Assembler.
//Input  : OPTAB, and the file which contains the code to be processed whose name is
//         provided as input.
//Output : It displays the Pass 1 Assembled Code(Intermediate Code) and the contents
//         of OPTAB, SYMTAB
//Author : Sheen Xavier A
//Date   : 15 / 10 / 2020

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Data structure element for the SYMTAB
typedef struct {
    char name[100];
    int address;
}symcontent;

//Data structure element for the OPTAB
typedef struct{
    char opcode[100];
    char bin[2];
    int flag;
}opcontent;

//Function which is used to seperate lines into words
void lineSeperator(char *line, char section[3][100])
{
    int i = 0, j = 0, sectionNumber = 0;
    while(line[i] != '\0')
    {
            if(line[i] == ' ')
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
}

//Hash function for mapping the opcodes as well as labels to OPTABLE and SYMTAB respectively 
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
    int LOCCTR, i = 0, j = 0, sectionNumber = 0, hashValue, startingAddress = 0, value, lengthProgram, error = 0, count, errorOPCODE = 0;
    char line[100], section[3][100], opcode[100], fname[100];
    symcontent SYMTAB[2000];
    opcontent OPTAB[2000];
    
    //Initializing the OPTAB and SYMTAB
    for(i = 0 ; i < 2000 ; i++)
        SYMTAB[i].address = -1;

    for(i = 0 ; i < 2000 ; i++)
        OPTAB[i].flag = 0;

    //Adding Opcodes into the OPTAB
    printf("CONTENTS OF OPTAB\nNUMBER OF OPCODES : ");
    scanf("%d", &count);
    printf("********************\n");
    for(i = 0 ; i < count ; i++)
    {
        printf("OPCODE : ");
        scanf(" %[^\n]", opcode);
        value = hash(opcode);
        strcpy(OPTAB[value].opcode, opcode);
        printf("BINARY CODE : ");
        scanf(" %[^\n]", OPTAB[value].bin);
        OPTAB[value].flag = 1;
        printf("********************\n");
    }

    //Reading the file which contains the code 
    printf("FILENAME : ");
    scanf(" %[^\n]", fname);
    ptr = fopen(fname, "r");
    
    //Opening the file to which the Pass 1 assembled code is written to
    ptrW = fopen("interm.txt", "w");
    
    //Reading first line
    fscanf(ptr, " %[^\n]", line);
    lineSeperator(line, section);

    //Checking whether the line is a START statement
    if(strcmp(section[1], "START") == 0)
    {
        sscanf(section[2], "%X", &LOCCTR);
        startingAddress = LOCCTR;
        fprintf(ptrW, "%04X\t%s\t%s\t%s\n", LOCCTR, section[0], section[1], section[2]);
        fscanf(ptr, " %[^\n]", line);
        lineSeperator(line, section);
    }
    //Else initializing LOCCTR to 0000
    else
        LOCCTR = 0000;
    
    //Reading and processing each line until we reach the end of the Code Segment
    while(strcmp("END", section[1]))
    {
        error = 0;
        if(section[0][0] != '-')
        {
            hashValue = hash(section[0]);
            if(SYMTAB[hashValue].address != -1)
                error = 1;  
            else
            {
                SYMTAB[hashValue].address = LOCCTR;
                strcpy(SYMTAB[hashValue].name, section[0]);
            }
        }
        if(OPTAB[hash(section[1])].flag == 1)
        {
            value = 3;
            LOCCTR = LOCCTR + value;
        }
        else if(strcmp(section[1], "WORD") == 0)
        {
            value = 3;
            LOCCTR = LOCCTR + value;
        }
        else if(strcmp(section[1], "RESW") == 0)
        {
            value = atoi(section[2]) * 3;
            LOCCTR = LOCCTR + value;
        }
        else if(strcmp(section[1], "RESB") == 0)
        {
            value = atoi(section[2]);
            LOCCTR = LOCCTR + value;
        }
        else if(strcmp(section[1], "BYTE") == 0)
        {
            value = strlen(section[2]) - 3;
            LOCCTR = LOCCTR + value;
        }
        else
        {
            printf("INVALID OPERATION! ABORTING....");
            errorOPCODE = 1;
            break;
        }
        if(error)
            fprintf(ptrW, "***DUPLICATE SYMTAB ENTRY***\n%s\n****************************\n", section[0]);
        else
            fprintf(ptrW, "%04X\t%s\t%s\t%s\n", LOCCTR - value, section[0], section[1], section[2]);
        
        fscanf(ptr, " %[^\n]", line);
        lineSeperator(line, section);
    }
    //Calculating the length of the Pass 1 assembled code
    lengthProgram = LOCCTR - startingAddress;
    fprintf(ptrW, "-\t%s\t%s\t%s\n", section[0], section[1], section[2]);
    fclose(ptr);
    fclose(ptrW);
    
    //Displayoing the contents of OPTAB, SYMTAB and intermediate file
    if(!errorOPCODE)
    {
        printf("CONTENTS OF OPTAB\n");
        printf("*****************\n");
        printf("OPCODE\tCODE\n");
        for(i = 0 ; i < 2000 ; i++)
        {
            if(OPTAB[i].flag)
                printf("%s\t%s\n", OPTAB[i].opcode, OPTAB[i].bin);
        }
        printf("******************\n");

        printf("CONTENTS OF SYMTAB\n");
        printf("******************\n");
        printf("LABEL\tADDRESS\n");
        for(i = 0 ; i < 2000 ; i++)
        {
            if(SYMTAB[i].address != -1)
                printf("%s\t%X\n", SYMTAB[i].name, SYMTAB[i].address);
        }
        printf("*********************************\n");
        printf("THE CONTENTS OF INTERMEDIATE FILE\n");
        printf("*********************************\n");
        ptr = fopen("interm.txt", "r");
        while(fscanf(ptr, " %[^\n]", line) != EOF)
            printf("%s\n", line);
        fclose(ptr);
        printf("*********************************\n");
        printf("PROGRAM LENGTH : %04X\n", lengthProgram);
        printf("*********************************\n");
    }
    return 0;
}