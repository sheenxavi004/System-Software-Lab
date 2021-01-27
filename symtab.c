//Name   : symtab.c
//Desc   : Program which implements the symbol table using hashing.
//Input  : Input required to be INSERTED, DELETED, MODIFIED, SEARCHED.
//Output : It displays the result on the respective operation
//Author : Sheen Xavier A
//Date   : 19 / 10 / 2020

#include <stdio.h>
#include <string.h>

//Defining the Structure for Symbol Table
typedef struct{
    char label[100];
    char type[10];
    int address;
    int flag;
}symcontent;

//Hash function to implement hashtable based on the label of the SYMBOL
int hash(char * str)
{
    int i = 0, sum = 0;
    for(i = 0 ; str[i] != '\0'; i++)
        sum += (i + 1) * (str[i] - 64);
    return sum;
}

int main()
{
    int i, choice, hashValue;
    char label[100];
    //Declaring the Symbol Table
    symcontent SYMTAB[2000];
    //Initializing the Symbol Table
    for(i = 0 ; i < 2000 ; i++)
        SYMTAB[i].flag = 0;
    printf("MENU\n1.INSERT\n2.DELETE\n3.MODIFY\n4.SEARCH\n5.DISPLAY\n6.EXIT\n");
    while(1)
    {
        printf("\nCHOICE : ");
        scanf("%d", &choice);
    
        if(choice == 6) //Handling EXIT Operation
            break;
        
        switch(choice)
        {
            case 1: printf("LABEL : "); //Handling INSERT Operation
                    scanf(" %[^\n]", label);
                    hashValue = hash(label);
                    if(SYMTAB[hashValue].flag)
                        printf("DUPLICATE SYMBOL ENCOUNTERED\n");
                    else
                    {
                        strcpy(SYMTAB[hashValue].label, label);
                        printf("TYPE : ");
                        scanf(" %[^\n]", SYMTAB[hashValue].type);
                        printf("ADDRESS/VALUE(HEX) : ");
                        scanf(" %X", &SYMTAB[hashValue].address);
                        printf("SYMBOL SUCCESSFULLY ADDED TO THE SYMTAB\n");
                        SYMTAB[hashValue].flag = 1;
                    }
                    break;
            case 2: printf("LABEL : "); //Handling DELETE Operation
                    scanf(" %[^\n]", label);
                    hashValue = hash(label);
                    if(SYMTAB[hashValue].flag == 0)
                        printf("SYMBOL NOT FOUND !\n");
                    else
                    {
                        SYMTAB[hashValue].flag = 0;
                        printf("SYMBOL SUCCESSFULLY REMOVED FROM THE SYMTAB\n");
                    }
                    break;
            case 3: printf("LABEL : "); //Handling MODIFICATION Operation
                    scanf(" %[^\n]", label);
                    hashValue = hash(label);
                    if(SYMTAB[hashValue].flag == 0)
                        printf("SYMBOL NOT FOUND !\n");
                    else
                    {
                        printf("NEW VALUES : \n");
                        printf("TYPE : ");
                        scanf(" %[^\n]", SYMTAB[hashValue].type);
                        printf("ADDRESS/VALUE(HEX): ");
                        scanf(" %X", &SYMTAB[hashValue].address);
                        printf("SYMBOL SUCCESSFULLY UPDATED\n");
                    }
                    break;
            case 4: printf("LABEL : "); //Handling SEARCH Operation
                    scanf(" %[^\n]", label);
                    hashValue = hash(label);
                    if(SYMTAB[hashValue].flag == 0)
                        printf("SYMBOL NOT FOUND !\n");
                    else
                    {
                        printf("SYMBOL FOUND!\nSYMBOL INFO : \n\n");
                        printf("LABEL : %s\nTYPE : %s\nADDRESS/VALUE : %X\n", SYMTAB[hashValue].label, SYMTAB[hashValue].type, SYMTAB[hashValue].address);
                    }
                    break;
            case 5: printf("CONTENTS OF SYMTAB : \n"); //Handling SEARCH Operation
                    printf("LABEL\tTYPE\tADDRESS/VALUE\n");
                    for(i = 0 ; i < 2000 ; i++)
                        if(SYMTAB[i].flag)
                            printf("%s\t%s\t%X\n", SYMTAB[i].label, SYMTAB[i].type, SYMTAB[i].address);
                    break;
            default: printf("Wrong Input !\n");
        }
    }
    return 0;
}