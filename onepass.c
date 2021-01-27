//Name   : onepass.c
//Desc   : Implementation of a single pass assembler.
//Input  : OPTAB, and the file which contains the code to be processed whose name is
//         provided as input.
//Output : It displays the Pass Object program and the contents of OPTAB and SYMTAB
//Author : Sheen Xavier A
//Date   : 28 / 10 / 2020

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Data structure used to store labels and operands and dependants
typedef struct {
    char name[100];
    int address;
    int dependants[100];
    int dependantLength;
}symcontent;

//Date structure used to store opcodes and corresponding machine codes
typedef struct{
    char opcode[100];
    char bin[2];
    int flag;
}opcontent;

//Function which seperates a line into sections based on spaces
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
    FILE *ptr, *ptrW;
    int LOCCTR, i = 0, j = 0, hashValue, startingAddress = 0, value, lengthProgram, count, textlength = 0, valueO, valueS, error = 0, objectlength = 0, textStartingAddress, seperatorCount = 0, currAddress, instrAddress = -1, isRESWB;
    char line[100], section[3][100], opcode[100], fname[100], textrecord[100], objectCode[7], pname[100], c;
    //Initializing the OPTAB and SYMTAB
    symcontent SYMTAB[2000];
    opcontent OPTAB[2000];
    for(i = 0 ; i < 2000 ; i++)
    {
        SYMTAB[i].dependantLength = 0;
        SYMTAB[i].address = -1;
    }
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
    
    //Performing one pass assembly
    ptr = fopen(fname, "r");
    ptrW = fopen("interm.txt", "w");
    fscanf(ptr, " %[^\n]", line);
    lineSeperator(line, section);

    if(strcmp("START", section[1]) == 0)
    {
        sscanf(section[2], "%X", &LOCCTR);
        startingAddress = LOCCTR;
        strcpy(pname, section[0]);
        fscanf(ptr, " %[^\n]", line);
        lineSeperator(line, section);
    }
    else
    {
        LOCCTR = 0000;
        pname[0] = '*';
    }
    textStartingAddress = LOCCTR;
    while(strcmp(section[1], "END"))
    {
        isRESWB = 0;
        if(line[0] != '.')
        {
            //Inserting SYMBOLS into SYMTAB
            if(section[0][0] != '-')
            {
                hashValue = hash(section[0]);
                if(SYMTAB[hashValue].address == -1)
                {
                    strcpy(SYMTAB[hashValue].name, section[0]);
                    SYMTAB[hashValue].address = LOCCTR;
                }
            }
            valueO = hash(section[1]);
            currAddress = LOCCTR;
            if(OPTAB[valueO].flag)
            {
                strcpy(objectCode, OPTAB[valueO].bin);
                valueS = hash(section[2]);
                if(section[2][0] == '-')
                    sprintf(objectCode + 2, "%04X", 0);
                else if(SYMTAB[valueS].address != -1)
                    sprintf(objectCode + 2, "%04X", SYMTAB[valueS].address);
                else
                {
                    //Adding dependants of a particular SYMBOL which is defined in the future
                    SYMTAB[valueS].dependants[SYMTAB[valueS].dependantLength] = LOCCTR + 1;
                    SYMTAB[valueS].dependantLength++;
                    sprintf(objectCode + 2, "%04X", 0);
                }
                LOCCTR += 3;
                objectlength = 6;
                if(instrAddress == -1)
                    instrAddress = currAddress;
            }
            else if(strcmp(section[1], "BYTE") == 0)
            {
                count = strlen(section[2]);
                j = 0;
                if(section[2][0] == 'C')
                {
                    for(i = 2 ; i < (count - 1) ; i++)
                    {
                        value = section[2][i];
                        sprintf(objectCode + j, "%02X", value);
                        j += 2;
                    }
                    objectlength = (count - 3) * 2;
                    LOCCTR += count - 3;
                }
                else
                {
                    objectlength = 2;
                    sprintf(objectCode, "%.2s", section[2] + 2);
                    LOCCTR++;
                }

            }
            else if(strcmp(section[1], "WORD") == 0)
            {
                LOCCTR += 3;
                objectlength = 6;
                value = atoi(section[2]);
                sprintf(objectCode, "%06X", value);
            }
            else if(strcmp(section[1], "RESW") == 0)
            {
                sscanf(section[2], "%d", &value);
                LOCCTR += 3 * value;
                isRESWB = 1;
            }
            else if(strcmp(section[1], "RESB") == 0)
            {
                sscanf(section[2], "%d", &value);
                LOCCTR += value;
                isRESWB = 1;
            }
            if(!isRESWB)
            {
                if(textlength + objectlength > 60 || currAddress - textStartingAddress > 60)
                {
                    textrecord[textlength + seperatorCount] = '\0';
                    fprintf(ptrW, "T^%06X^%02X%s\n", textStartingAddress, textlength / 2, textrecord);
                    textStartingAddress = currAddress;
                    textlength = 0;
                    seperatorCount = 0;
                }
            }
            //Handling Forward Referencing
            if(SYMTAB[hashValue].dependantLength && SYMTAB[hashValue].address != -1) 
            {
                if(textlength)
                {
                    textrecord[textlength + seperatorCount] = '\0';
                    fprintf(ptrW, "T^%06X^%02X%s\n", textStartingAddress, textlength / 2, textrecord);
                }
                i = 0;
                textlength = 0;
                seperatorCount = 0;
                textStartingAddress = LOCCTR;
                while(i < SYMTAB[hashValue].dependantLength)
                {
                    fprintf(ptrW, "T^%06X^%02X^%04X\n", SYMTAB[hashValue].dependants[i], 2, SYMTAB[hashValue].address); 
                    i++;
                }
                SYMTAB[hashValue].dependantLength = 0;
                textStartingAddress = LOCCTR;
                textlength = 0;
                seperatorCount = 0;
            }
            if(!isRESWB)
            {
                objectCode[objectlength] = '\0';
                sprintf(textrecord + textlength + seperatorCount, "^%s", objectCode);
                textlength += objectlength;
                seperatorCount++;
            }
        }
        fscanf(ptr, " %[^\n]", line);
        lineSeperator(line, section);
    }
    if(textlength)
    {
        textrecord[textlength + seperatorCount] = '\0';
        fprintf(ptrW, "T^%06X^%02X%s\n", textStartingAddress,  textlength / 2, textrecord);
    }
    fprintf(ptrW, "E^%06X\n", instrAddress);
    fclose(ptr);
    fclose(ptrW);
    lengthProgram = LOCCTR - startingAddress;
    
    //Writing the header record to the top of the object program
    ptr = fopen("interm.txt", "r");
    ptrW = fopen("output.txt", "w");

    if(pname[0] != '*')
        fprintf(ptrW, "H^%-6.6s^%06X^%06X\n", pname, startingAddress, lengthProgram);
    else
        fprintf(ptrW, "H^%-6.6s^%06X^%06X\n", " ",  0, lengthProgram);
    
    while((c = fgetc(ptr)) != EOF)
        fputc(c, ptrW);
    
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