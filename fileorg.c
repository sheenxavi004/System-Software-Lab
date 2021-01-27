//Name   : fsfs.c
//Desc   : Simulation of Single Level, Two-Level and Hierarchical Level File Organisation.
//Author : Sheen Xavier A
//Date   : 28 / 09 / 20

#include <stdio.h>
#include <string.h>
#define MAX_SUB 5

//Structure for directory in a Single-Level-File-Organisation System
struct directory{
    char directoryName[100];
    char fileNames[100][100];
    int fileCount;
};

//Structure for Two-Level-File-Organisation System
struct twoLevel{
    struct directory folders[100];
    int dirCount;
};

//Structure for directory in a Hierarchical-File-Organisation System
struct directoryH{
    char directoryName[100];
    struct directory folders[MAX_SUB];
    char fileNames[100][100];
    int fileCount;
    int dirCount;
};

//Function for searching files in single level
int searchFileSL(struct directory *root, char name[100])
{
    int i;
    for(i = 0 ; i < root -> fileCount ; i++)
    {
        if(!strcmp(name, root -> fileNames[i]))
            return i;
    }
    return -1;
}
//Function for searching files in hierarchical level
int searchFileH(struct directoryH *root, char name[100])
{
    int i;
    for(i = 0 ; i < root -> fileCount ; i++)
    {
        if(!strcmp(name, root -> fileNames[i]))
            return i;
    }
    return -1;
}
//Function for searching directories in  Twolevel level
int searchDir(struct twoLevel *root, char name[100])
{
    int i;
    for(i = 0 ; i < root -> dirCount ; i++)
    {
        if(!strcmp(name, root -> folders[i].directoryName))
            return i;
    }
    return -1;
}

//Function for searching directories in hierarchical level
int searchDirH(struct directoryH *root, char name[100])
{
    int i;
    for(i = 0 ; i < root -> dirCount ; i++)
    {
        if(!strcmp(name, root -> folders[i].directoryName))
            return i;
    }
    return -1;
}

//Function to create directories in Two Level
void createDir(struct twoLevel *root)
{
    char dname[100];
    printf("NAME OF DIRECTORY : ");
    scanf(" %[^\n]", dname);
    if(searchDir(root, dname) != -1)
        printf("There is already a directory with the same name!\n");
    else
    {
        strcpy(root -> folders[root -> dirCount].directoryName, dname);
        root -> dirCount++;
        printf("The Directory %s has been created successfully!\n", dname);
    }
}

//Function to create directories in Hierarchial Level
void createDirH(struct directoryH *root, int currIndex)
{
    char dname[100];
    printf("NAME OF DIRECTORY : ");
    scanf(" %[^\n]", dname);
    if(searchDirH(&root[currIndex], dname) != -1)
        printf("There is already a directory with the same name!\n");
    else
    {
        strcpy(root[currIndex * 5 + 1 + root[currIndex].dirCount].directoryName, dname);
        strcpy(root[currIndex].folders[root -> dirCount].directoryName, dname);
        root[currIndex].dirCount++;
        printf("The Directory %s has been created successfully!\n", dname);
    }
}

//Function to create Files in Single level
void createFileSL(struct directory *root)
{
    char fname[100];
    printf("NAME OF FILE : ");
    scanf(" %[^\n]", fname);
    if(searchFileSL(root, fname) != -1)
        printf("There is already a file with the same name!\n");
    else
    {
        strcpy(root -> fileNames[root -> fileCount], fname);
        root -> fileCount++;
        printf("The File %s has been created successfully!\n", fname);
    }
}


//Function to create Files in Hierarchical level
void createFileH(struct directoryH *root)
{
    char fname[100];
    printf("NAME OF FILE : ");
    scanf(" %[^\n]", fname);
    if(searchFileH(root, fname) != -1)
        printf("There is already a file with the same name!\n");
    else
    {
        strcpy(root -> fileNames[root -> fileCount], fname);
        root -> fileCount++;
        printf("The File %s has been created successfully!\n", fname);
    }
}

//Function to delete files in Single Level
void deleteFilesSL(struct directory *root)
{
    char fname[100];
    int index, i;
    printf("NAME OF FILE : ");
    scanf(" %[^\n]", fname);
    index = searchFileSL(root, fname);
    if(index == -1)
        printf("File doesn't Exist!\n");
    else
    {
        for(i = index ; i < root -> fileCount - 1 ; i++)
            strcpy(root -> fileNames[i],root -> fileNames[i + 1]);
        root -> fileCount--;
        printf("The File %s has been deleted successfully!\n", fname);
    }
}

//Function to delete files in Hierarchical Level
void deleteFilesH(struct directoryH *root)
{
    char fname[100];
    int index, i;
    printf("NAME OF FILE : ");
    scanf(" %[^\n]", fname);
    index = searchFileH(root, fname);
    if(index == -1)
        printf("File doesn't Exist!\n");
    else
    {
        for(i = index ; i < root -> fileCount - 1 ; i++)
            strcpy(root -> fileNames[i],root -> fileNames[i + 1]);
        root -> fileCount--;
        printf("The File %s has been deleted successfully!\n", fname);
    }
}

//Function to delete directories in Two Level
void deleteDir(struct twoLevel *root)
{
    int i, index;
    char dname[100];
    printf("NAME OF DIRECTORY : ");
    scanf(" %[^\n]", dname);
    index = searchDir(root, dname);
    if(index == -1)
        printf("Directory doesn't Exist!\n");
    else
    {
        for(i = index ; i < root -> dirCount ; i++)
            root -> folders[i] = root -> folders[i + 1];
        root -> dirCount--;
        printf("The Directory %s has been deleted successfully!\n", dname);
    }
}


//Function to delete directories in Hierarchical Level
void deleteDirH(struct directoryH *root, int currIndex)
{
    int i, index;
    char dname[100];
    printf("NAME OF DIRECTORY : ");
    scanf(" %[^\n]", dname);
    index = searchDirH(&root[currIndex], dname);
    if(index == -1)
        printf("Directory doesn't Exist!\n");
    else
    {
        for(i = index ; i < root -> dirCount ; i++)
            root -> folders[i] = root -> folders[i + 1];
        root[currIndex].dirCount--;
        printf("The Directory %s has been deleted successfully!\n", dname);
    }
}

//Function to display files in Single Level
void displayFilesSL(struct directory *root)
{
    int i;
    if(root -> fileCount == 0)
        printf("There are no files inside the %s!\n", root -> directoryName);
    else
    {
        printf("/%s : ", root -> directoryName);
        for(i = 0 ; i < root -> fileCount ; i++)
        {
            printf("%s ", root -> fileNames[i]);
        }
        printf("\n");
    }
}

//Function to display contents in Two Level
void displayDir(struct twoLevel *root)
{
    int i;
    if(root -> dirCount == 0)
        printf("There are no directories currently!\n");
    else
    {
        printf("CONTENTS OF TWO LEVEL DIRECTORY SYSTEM : \n");
        for(i = 0 ; i < root -> dirCount ; i++)
        {
            displayFilesSL(&(root -> folders[i]));
        }
    }
}

//Function to display contents in Hierarchical Level
void displayDirH(struct directoryH *root)
{
    int i;
    if(root -> dirCount == 0 && root -> fileCount == 0)
        printf("There directory is currently empty!\n");
    else
    {
        printf("CONTENTS OF %s : \n",root -> directoryName);
        for(i = 0 ; i < root -> dirCount ; i++)
        {
            printf("/%s ", root -> folders[i].directoryName);
        }
        for(i = 0 ; i < root -> fileCount ; i++)
        {
            printf("%s ", root -> fileNames[i]);
        }
        printf("\n");
    }
}

//Function which prints the current path in a Hierarchical
void displayPath(struct directoryH *root, int currIndex)
{
    if(currIndex == 0)
    {
        printf("/%s", root[currIndex].directoryName);   
        return;
    }
    displayPath(root, currIndex / 5);
    printf("/%s", root[currIndex].directoryName);
}

//Simultaion of Single Directory System
void singleLDirectorySystem()
{
    struct directory root;
    int choice;
    char fname[100];
    printf("NAME OF ROOT DIRECTORY: ");
    scanf(" %[^\n]", root.directoryName);
    root.fileCount = 0;
    printf("1. CREATE FILE\n2. DELETE FILE\n3. SEARCH FILE \n4. DISPLAY FILES\n5. EXIT");
    while(1)
    {
        printf("\nCHOICE : ");
        scanf("%d", &choice);
        if(choice == 5)
            break;
        switch(choice)
        {
            case 1:  if(root.fileCount == 100)
                        printf("The Directory is Full !\n");
                     else
                        createFileSL(&root);
                     break;
            case 2:  deleteFilesSL(&root);
                     break;
            case 3:  printf("FILE NAME: ");
                     scanf(" %[^\n]",fname);
                     if(searchFileSL(&root, fname) == -1)
                        printf("%s not Found !\n", fname);
                     else
                        printf("%s Found!\n", fname);
                     break;
            case 4:  displayFilesSL(&root);
                     break;
            default: printf("Wrong Input\n");
        }
    }
}

//Simultaion of Two-Level Directory System
void twoLDirectorySystem()
{
    struct twoLevel root;
    int choice, index;
    char dname[100], fname[100];
    root.dirCount = 0;
    printf("1. CREATE DIRECTORY\n2. CREATE FILE\n3. DELETE FILE\n4. DELETE DIRECTORY\n5. SEARCH FILE \n6. DISPLAY FILES\n7. EXIT");
    while(1)
    {
        printf("\nCHOICE : ");
        scanf("%d", &choice);
        if(choice == 7)
            break;
        switch(choice)
        {
            case 1:  if(root.dirCount == 100)
                        printf("The Parent Directory is Full !\n");
                     else
                        createDir(&root);
                     break;

            case 2:  printf("DIRECTORY NAME : ");
                     scanf(" %[^\n]", dname);
                     index = searchDir(&root, dname);
                     if(index == -1)
                        printf("There is no directory with name as %s!\n", dname);
                     else
                     {
                        if(root.folders[index].fileCount == 100)
                            printf("The Directory is Full !\n");
                        else
                            createFileSL(&(root.folders[index]));
                     }
                     break;

            case 3:  printf("DIRECTORY NAME : ");
                     scanf(" %[^\n]", dname);
                     index = searchDir(&root, dname);
                     if(index == -1)
                        printf("There is no directory with name as %s!\n", dname);
                     else
                     {
                        if(root.folders[index].fileCount == 100)
                            printf("The Directory is Full !\n");
                        else
                            deleteFilesSL(&(root.folders[index]));
                     }
                     break;
            case 4:  deleteDir(&root);
                     break;

            case 5:  printf("DIRECTORY NAME : ");
                     scanf(" %[^\n]", dname);
                     index = searchDir(&root, dname);
                     if(index == -1)
                        printf("There is no directory with name as %s!\n", dname);
                     else
                     {
                        printf("FILE NAME: ");
                        scanf(" %[^\n]",fname);
                        if(searchFileSL(&(root.folders[index]), fname) == -1)
                            printf("%s not Found !\n", fname);
                        else
                            printf("%s Found!\n", fname);
                     }
                     break;
            case 6:  displayDir(&root);
                     break;
            default: printf("Wrong Input\n");
        }
    }
}

//Simultaion of Hierarchical Directory System
void hierarchicalSystem()
{
    struct directoryH treeH[100];
    int choice, index, currIndex;
    char dname[100], fname[100];
    strcpy(treeH[0].directoryName,"ROOT");
    printf("1. CREATE DIRECTORY\n2. CREATE FILE\n3. DELETE FILE\n4. DELETE DIRECTORY\n5. SEARCH FILE \n6. DISPLAY CONTENTS\n7. JUMP TO IMMEDIATE PARENT \n8. JUMP TO CHILD DIRECTORY\n9. DISPLAY PATH\n10. EXIT");
    while(1)
    {
        printf("\nCHOICE : ");
        scanf("%d", &choice);
        if(choice == 10)
            break;
        switch(choice)
        {
            case 1:  if(treeH[currIndex].dirCount == 5)
                        printf("The Parent Directory is Full !\n");
                     else
                     {
                        createDirH(treeH, currIndex);
                     }
                     break;

            case 2:  if(treeH[currIndex].fileCount == 100)
                        printf("The Directory is Full !\n");
                     else
                        createFileH(&(treeH[currIndex]));
                     break;

            case 3:  if(treeH[currIndex].folders[index].fileCount == 100)
                        printf("The Directory is Full !\n");
                     else
                        deleteFilesH(&(treeH[currIndex]));
                     break;

            case 4:  deleteDirH(treeH, currIndex);
                     break;

            case 5:  printf("FILE NAME: ");
                     scanf(" %[^\n]",fname);
                     if(searchFileSL(&(treeH[currIndex].folders[index]), fname) == -1)
                        printf("%s not Found !\n", fname);
                     else
                        printf("%s Found!\n", fname);
                     break;

            case 6:  displayDirH(&treeH[currIndex]);
                     break;

            case 7:  currIndex = currIndex / 5;
                     printf("Jumped to immediate parent!\n");
                     break;

            case 8:  printf("DIRECTORY NAME : ");
                     scanf(" %[^\n]", dname);
                     index = searchDirH(&treeH[currIndex], dname);
                     if(index == -1)
                        printf("There is no directory with name as %s!\n", dname);
                     else
                     {
                         currIndex = currIndex * 5 + index + 1;
                         printf("Jumped to %s\n", dname);
                     }
                     break;
            
            case 9:  printf("CURRENT PATH: ");
                     displayPath(treeH, currIndex);
                     printf("\n");
                     break;

            default: printf("Wrong Input\n");
        }
    }
}

int main()
{
    int directoryChoice;
    printf("MENU\n1. SINGLE LEVEL DIRECTORY\n2. TWO-LEVEL DIRECTORY\n3. HIERARCHICAL DIRECTORY\n4. EXIT");
    while(1)
    {
        printf("\nFILE SYSTEM CHOICE: ");
        scanf("%d", &directoryChoice);
        if(directoryChoice == 4)
            break;
        switch(directoryChoice)
        {
            case 1 : singleLDirectorySystem();
                     break;
            case 2 : twoLDirectorySystem();
                     break;
            case 3 : hierarchicalSystem();
                     break;
            default: printf("Wrong Input");
        }
    }
    return 0;
}