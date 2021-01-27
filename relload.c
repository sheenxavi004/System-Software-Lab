//Name   : relload.c
//Desc   : Implementation of a Relocatable Loader.
//Input  : The name of the file which contains the object program is provided as the input
//         and the starting location of the memory block
//Output : It displays contents of the memory location after loading
//Author : Sheen Xavier A
//Date   : 16 / 11 / 2020

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define MAX 32768

int main()
{
    FILE *ptr, *ptrW;
    int LOCCTR, startingAddress, i = 0, j = 0, programLength, MEMLOC[MAX], modifyBitLength, byteCode;
    char line[100], fname[100], pname[100], objectCode[10];

    //Initiliazing Memory Locations
    for(i = 0; i < MAX; i++)
        MEMLOC[i] = INT_MAX;

    //Reading the name of the file which contains the input code
    printf("FILENAME : ");
    scanf(" %[^\n]", fname);

    //Opening the file which contains the object program
    ptr = fopen(fname, "r");
    
    //Enter the starting address of the memory block
    //to which the program is to be stored

    printf("RELOCATE OBJECT PROGRAM TO : ");
    scanf("%X", &startingAddress);

    //Verifying the program name and length
    fscanf(ptr, " %[^\n]", line);
    sscanf(line + 16, "%X", &programLength);
    sscanf(line + 2, "%6s", pname);

    //Reading the next line
    fscanf(ptr, " %[^\n]", line);

    printf("PROGRAM NAME\t: %s\n", pname);
    printf("PROGRAM LENGTH\t: %06X\n", programLength);

    //Read until the we reach the end of the file
    while(line[0] != 'E')
    {
        //Initializing the memory address
        sscanf(line + 2, "%6X", &LOCCTR);
        LOCCTR += startingAddress;
        //Since the characters are in packed form we don't need to convert each half byte to specific character
        if(line[0] == 'T')
        {
            i = 12;
            while(line[i] != '\0')
            {
                sscanf(line + i, "%2X", MEMLOC + LOCCTR);
                //Moving LOCCTR to next byte
                LOCCTR++;
                i += 2;
                if(line[i] == '^')
                    i++;
            }
        }

        //Handling the modification record
        else if(line[0] == 'M')
        {
            i = 9;
            sscanf(line + i, "%2X", &modifyBitLength);

            //SIC/XE Mode
            if(modifyBitLength > 4)
                sprintf(objectCode, "%06X", startingAddress);
            //SIC Mode
            else
                sprintf(objectCode, "%04X", startingAddress);
            j = 0;
            while(modifyBitLength > 0)
            {
                sscanf(objectCode + j, "%2X", &byteCode);
                MEMLOC[LOCCTR] += byteCode;
                LOCCTR++;
                j += 2;
                modifyBitLength -= 2;
            }
        }
        
        //Reading next line
        fscanf(ptr, " %[^\n]", line);
    }
    //Bringing control back to first executable instruction
    sscanf(line + 2, "%6X", &LOCCTR);
    LOCCTR += startingAddress;
    fclose(ptr);
    printf("------------------------------\n");
    printf("MEMORY LOCATION  OBJECT CODE\n");
    printf("------------------------------\n");
    for(i = 0; i < MAX ; i++)
        if(MEMLOC[i] != INT_MAX)
            printf("%06X\t\t\t%02X\n", i, MEMLOC[i]);
    printf("------------------------------\n");
    return 0;
}