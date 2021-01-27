//Name   : pass2.c
//Desc   : Program which simulates the Pass 2 assembling in a 2 Pass Assembler.
//Input  : OPTAB, and the file which contains the code to be processed whose name is
//         provided as input.
//Output : It displays the Pass 2 Assembled Code(Object program) and the contents
//         of OPTAB, SYMTAB
//Author : Sheen Xavier A
//Date   : 17 / 10 / 2020

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Data structure used to store labels and operands
typedef struct {
    char name[100];
    int address;
}symcontent;

//Date structure used to store opcodes and corresponding machine codes
typedef struct{
    char opcode[100];
    char bin[2];
    int flag;
}opcontent;

//Function which seperates a line into sections based on spaces
void lineSeperator(char *line, char section[4][100])
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

//Hash function for both OPTAB as well as SYMTAB
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
    int LOCCTR, i = 0, j = 0, sectionNumber = 0, hashValue, startingAddress = 0, value, lengthProgram, count, textlength = 0, valueO, valueS, error = 0, objectlength = 0, textStartingAddress, seperatorCount = 0, currAddress, instrAddress = -1;
    char line[100], section[4][100], opcode[100], fname[100], textrecord[100], objectCode[7];
    //Initializing the OPTAB and SYMTAB
    symcontent SYMTAB[2000];
    opcontent OPTAB[2000];
    for(i = 0 ; i < 2000 ; i++)
        SYMTAB[i].address = -1;

    for(i = 0 ; i < 2000 ; i++)
        OPTAB[i].flag = 0;

    //Populating the OPTAB
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
    
    //Reading the name of the file which contains the input code
    printf("FILENAME : ");
    scanf(" %[^\n]", fname);
    ptr = fopen(fname, "r");
    fscanf(ptr, " %[^\n]", line);
    lineSeperator(line, section);

    //Processing the code by Pass 1 of assembler inorder to populate SYMTAB
    if(strcmp(section[2], "START") == 0)
    {
        sscanf(section[3], "%X", &LOCCTR);
        startingAddress = LOCCTR;
        fscanf(ptr, " %[^\n]", line);
        lineSeperator(line, section);
    }
    else
        LOCCTR = 0000;
    
    while(strcmp("END", section[2]))
    {
        if(section[1][0] != '-')
        {
            hashValue = hash(section[1]);
            if(SYMTAB[hashValue].address == -1)
            {
                SYMTAB[hashValue].address = LOCCTR;
                strcpy(SYMTAB[hashValue].name, section[1]);
            }
        }
        if(strcmp(section[2], "WORD") == 0)
        {
            value = 3;
            LOCCTR = LOCCTR + value;
        }
        else if(strcmp(section[2], "RESW") == 0)
        {
            value = atoi(section[3]) * 3;
            LOCCTR = LOCCTR + value;
        }
        else if(strcmp(section[2], "RESB") == 0)
        {
            value = atoi(section[3]);
            LOCCTR = LOCCTR + value;
        }
        else if(strcmp(section[2], "BYTE") == 0)
        {
            value = strlen(section[3]) - 3;
            LOCCTR = LOCCTR + value;
        }
        else
        {
            value = 3;
            LOCCTR = LOCCTR + value;
        }
        
        fscanf(ptr, " %[^\n]", line);
        lineSeperator(line, section);
    }
    lengthProgram = LOCCTR - startingAddress;
    fclose(ptr);
    
    //Performing Pass 2 of assembly language
    ptr = fopen(fname, "r");
    ptrW = fopen("output.txt", "w");
    fscanf(ptr, " %[^\n]", line);
    lineSeperator(line, section);

    if(strcmp("START", section[2]) == 0)
    {
        fprintf(ptrW, "H^%-6.6s^%06X^%06X\n", section[1], startingAddress, lengthProgram);
        fscanf(ptr, " %[^\n]", line);
        lineSeperator(line, section);
    }
    else
        fprintf(ptrW, "H^%-6.6s^%06X^%06X\n", " ",  0, lengthProgram);
    sscanf(section[0], "%X", &textStartingAddress);
    while(strcmp(section[2], "END"))
    {
        valueO = hash(section[2]);
        sscanf(section[0], "%X", &currAddress);
        if(OPTAB[valueO].flag)
        {
            strcpy(objectCode, OPTAB[valueO].bin);
            valueS = hash(section[3]);
            if(SYMTAB[valueS].address != -1)
                sprintf(objectCode + 2, "%04X", SYMTAB[valueS].address);
            else if(section[3][0] == '-')
                sprintf(objectCode + 2, "%04X", 0);
            else
            {
                error = 1;
                printf("OPERAND NOT FOUND! ABORTING....\n");
                break;
            }
            objectlength = 6;
            if(instrAddress == -1)
                instrAddress = currAddress;
        }
        else if(strcmp(section[2], "BYTE") == 0)
        {
            count = strlen(section[3]);
            j = 0;
            if(section[3][0] == 'C')
            {
                for(i = 2 ; i < (count - 1) ; i++)
                {
                    value = section[3][i];
                    sprintf(objectCode + j, "%02X", value);
                    j += 2;
                }
                objectlength = (count - 3) * 2;
            }
            else
            {
                objectlength = 2;
                sprintf(objectCode, "%.2s", section[3] + 2);
            }
        }
        else if(strcmp(section[2], "WORD") == 0)
        {
            objectlength = 6;
            sscanf(section[3], "%d", &value);
            sprintf(objectCode, "%06X", value);
        }
        else
        {
            fscanf(ptr, " %[^\n]", line);
            lineSeperator(line, section);
            continue;
        }
        if(textlength + objectlength > 60 || currAddress - textStartingAddress > 60)
        {
            textrecord[textlength + seperatorCount] = '\0';
            fprintf(ptrW, "T^%06X^%02X%s\n", textStartingAddress, textlength / 2, textrecord);
            textStartingAddress = currAddress;
            textlength = 0;
            seperatorCount = 0;
        }
        objectCode[objectlength] = '\0';
        sprintf(textrecord + textlength + seperatorCount, "^%s", objectCode);
        textlength += objectlength;
        seperatorCount++;
        fscanf(ptr, " %[^\n]", line);
        lineSeperator(line, section);
    }
    if(textlength > 0)
    {
        textrecord[textlength + seperatorCount] = '\0';
        fprintf(ptrW, "T^%06X^%02X%s\n", textStartingAddress,  textlength / 2, textrecord);
    }
    fprintf(ptrW, "E^%06X\n", instrAddress);
    fclose(ptr);
    fclose(ptrW);
    if(!error)
    {
        //Displaying content of both OPTAB and SYMTAB
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
        lengthProgram = LOCCTR - startingAddress;
        printf("*********************************\n");

        //Displaying the generated Object Program
        printf("THE CONTENTS OF OBJECT PROGRAM\n");
        printf("*********************************\n");
        ptr = fopen("output.txt", "r");
        while(fscanf(ptr, " %[^\n]", line) != EOF)
            printf("%s\n", line);
        fclose(ptr);
        printf("*********************************\n");
        printf("PROGRAM LENGTH : %04X\n", lengthProgram);
        printf("*********************************\n");
    }

    return 0;
}