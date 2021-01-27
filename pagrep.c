// Name   : pagrep.c
// Date   : 08/09/2020
// Aim    : To simulate page replacement algorithms such as
//		    FIFO, LRU and LFU.
// Input  : Accepts a array containing page numbers of reference string.
// Output : Display the memory status after each page fault 
//          and compute the total number of page faults.
// Author : Sheen Xavier A

#include<stdio.h>
//Function which returns the index of the element in frame which has not been accessed for a longer period
int leastAccessedValueIndex(int lastAccessed[], int frame[], int noOfFrames)
{
    int smallIndex = 0, i;
    for(i = 0 ; i < noOfFrames ; i++)
        if(lastAccessed[frame[smallIndex]] > lastAccessed[frame[i]])
            smallIndex = i;
	return smallIndex;
}
//Function which returns the index of the element in frame which has not been frequently accessed
int leastFrequentValueIndex(int lastAccessed[], int freq[], int frame[], int noOfFrames)
{
    int smallIndex = 0, i;
    for(i = 0 ; i < noOfFrames ; i++)
    {
        if(freq[frame[smallIndex]] > freq[frame[i]])
            smallIndex = i;
        else if(freq[frame[smallIndex]] == freq[frame[i]])
        {
            if(lastAccessed[frame[smallIndex]] > lastAccessed[frame[i]])
                smallIndex = i;
        }
    }
	return smallIndex;
}
//Function which detects Hit
int hit(int value, int frame[], int noOfFrame)
{
	int i;
	for(i = 0; i < noOfFrame ; i++)
		if(value == frame[i])
			return 1;
	return 0;
}
//Function which performs FIFO Page Replacement
void FIFO(int refstr[],int length,int noOfFrame)
{
	int frame[100],head = 0,tail = 0, i, pageFaults = 0, j;
	for(i = 0; i < noOfFrame ;i++)
		frame[i] = -1;
	printf("All Occurences of Page Fault : \n\n");
	for(i = 0 ; i < length ;i++)
	{
		if(hit(refstr[i], frame, noOfFrame))
			;
		else
		{
			if(frame[tail] == -1)
			{
				frame[tail] = refstr[i];
				tail = (tail + 1) % noOfFrame;
			}	
			else
			{
				frame[head] = refstr[i];
				head = (head + 1) % noOfFrame;
			}
			for(j = 0; j < noOfFrame; j++)
			{
				if(frame[j] == -1)
					printf("F");
				else
					printf("%d",frame[j]);
			}
			printf(" ");
			pageFaults++;
		}
	}
	printf("\n\nTotal Number of Page Faults : %d\n",pageFaults); 
}
//Function which performs LRU Replacement
void LRU(int refstr[],int length,int noOfFrame)
{
	int frame[100],tail = 0, i, pageFaults = 0, j, lrui;
    int biggestProcess = -1;
    for(i = 0 ; i < length ; i++)
    {
        if(refstr[i] > biggestProcess)
            biggestProcess = i;
    }

    int lastAccessed[biggestProcess + 1];
    for(i = 0 ; i <= biggestProcess ; i++)
        lastAccessed[i] = length;

	for(i = 0; i < noOfFrame ;i++)
		frame[i] = -1;
	printf("All Occurences of Page Fault : \n\n");
	for(i = 0 ; i < length ;i++)
	{
		if(hit(refstr[i], frame, noOfFrame))
			;
		else
		{
			if(frame[tail] == -1)
			{
				frame[tail] = refstr[i];
				tail++;
			}	
			else
			{
				lrui = leastAccessedValueIndex(lastAccessed,frame,noOfFrame);
				frame[lrui] = refstr[i];
			}
			for(j = 0; j < noOfFrame; j++)
			{
				if(frame[j] == -1)
					printf("F");
				else
					printf("%d",frame[j]);
			}
			printf(" ");
			pageFaults++;
		}
        lastAccessed[refstr[i]] = i;
	}
	printf("\n\nTotal Number of Page Faults : %d\n",pageFaults); 
}
//Function which performs LFU Replacement
void LFU(int refstr[],int length,int noOfFrame)
{
	int frame[100],tail = 0, i, pageFaults = 0, j, lrui;
    int biggestProcess = -1;
    for(i = 0 ; i < length ; i++)
    {
        if(refstr[i] > biggestProcess)
            biggestProcess = i;
    }

    int lastAccessed[biggestProcess + 1];
    for(i = 0 ; i <= biggestProcess ; i++)
        lastAccessed[i] = length;

    int freq[biggestProcess + 1];
    for(i = 0 ; i <= biggestProcess ; i++)
        freq[i] = 0;

	for(i = 0; i < noOfFrame ;i++)
		frame[i] = -1;
	printf("All Occurences of Page Fault : \n\n");
	for(i = 0 ; i < length ;i++)
	{
		if(hit(refstr[i], frame, noOfFrame))
			;
		else
		{
			if(frame[tail] == -1)
			{
				frame[tail] = refstr[i];
				tail++;
			}	
			else
			{
				lrui = leastFrequentValueIndex(lastAccessed,freq,frame,noOfFrame);
				frame[lrui] = refstr[i];
                freq[refstr[lrui]] = 0;
			}
			for(j = 0; j < noOfFrame; j++)
			{
				if(frame[j] == -1)
					printf("F");
				else
					printf("%d",frame[j]);
			}
			printf(" ");
			pageFaults++;
		}
        freq[refstr[i]]++;
        lastAccessed[refstr[i]] = i;
	}
	printf("\n\nTotal Number of Page Faults : %d\n",pageFaults); 
}
int main()
{
	int refstr[1000], length, noOfFrame, i, option;
	char choice;
	printf("Menu\n1.FIFO\n2.LRU\n3.LFU\n4.Exit\n");
	do
	{
		printf("\nChoice : ");
		scanf("%d",&option);

		if(option == 4)
			break;

		printf("Length of reference string : ");
		scanf("%d",&length);
		printf("Reference string : ");
		for(i = 0;i < length ; i++)
			scanf("%d",&refstr[i]);
		printf("Number of frames : ");
			scanf("%d",&noOfFrame);
		switch(option)
		{
			case 1  : FIFO(refstr,length,noOfFrame);
					  break;
			case 2  : LRU(refstr,length,noOfFrame);
					  break;
            case 3  : LFU(refstr,length, noOfFrame);
                      break;
			default : printf("Wrong Input !\n");
		}
		printf("Do you want to continue(Y/N) ?");
		scanf(" %c",&choice);
	}while(choice == 'Y' || choice == 'y');
	return 0;
}