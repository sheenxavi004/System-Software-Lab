// Name   : disk.c
// Date   : 08/09/2020
// Aim    : To simulate the disk scheduling methods such as
//		    FIFO, SCAN, CSCAN.
// Input  : Accepts a request queue and the current position.
//			of the head pointer and the total no of cylinders in the system.
// Output : Display the path of the head pointer and the 
//          total head movement.
// Author : Sheen Xavier A

#include<stdio.h>
//Function returns the absolute value of difference
int absolute(int a,int b)
{
	if(a - b < 0)
		return b - a;
	return a - b;
}
//Partition function of Quick Sort
int partition(int arr[], int l, int r)
{
    int count = 0, temp, i = 0, j = r, pivot = l;
    for(i = l + 1 ; i <= r ; i++)
        if(arr[i] <= arr[pivot])
            count++;
    temp = arr[l + count];
    arr[l + count] = arr[l];
    arr[l] = temp;
    pivot = count + l;
    i = l;
    j = r;
    while(i != pivot && j != pivot)
    {
        if(arr[i] <= arr[pivot])
            i++;
        else if(arr[j] > arr[pivot])
            j--;
        else
        {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
        
    }
    return pivot;

}
//Quick Sort
void sort(int arr[], int l, int r)
{
    if(l < r)
    {
        int q = partition(arr, l, r);
        sort(arr, l, q - 1);
        sort(arr, q + 1, r);
    }
}
//Function which perform FCFS Scheduling
void FCFS(int seekTime, int request[], int noOfRequests,int initialPos)
{
	int i = 0, totalMovement = 0,origin = initialPos;
	while(i < noOfRequests)
	{
		totalMovement += absolute(initialPos,request[i]);
		initialPos = request[i];
		i++;
	}
	printf("\nHead Movement : %d -> ",origin);
	for(i = 0;i < noOfRequests; i++ )
	{
		if(i == noOfRequests - 1)
			printf("%d\n",request[i]);
		else
			printf("%d -> ",request[i]);
	}
	printf("\nTotal Head Movement : %d",totalMovement);
    printf("\nTotal Seek Time : %d ms", seekTime * totalMovement);
    printf("\nAverage Seek Time: %.2f ms\n", (float)(seekTime * totalMovement) / noOfRequests);
}
//Function which perform SCAN Scheduling
void SCAN(int request[], int noOfRequests,int totalCylinders,int seekTime, int initialPos, int direction)
{
	int i = 0, totalMovement = 0, pos;
	sort(request, 0, noOfRequests - 1);
	if(direction == 'L')
	{
        while(request[i] <= initialPos && i < noOfRequests)
		    i++;
		pos = i - 1;
        if(initialPos != request[pos])
		    printf("\nHead Movement : %d -> ",initialPos);
        else
		    printf("\nHead Movement : ");   
		for(i = pos ; i >= 0 ; i--)
		{
            if(pos == noOfRequests - 1 && i == 0)
			    printf("%d\n",request[i]);
            else
                printf("%d -> ",request[i]);
			totalMovement += absolute(initialPos,request[i]);
			initialPos = request[i];
		}
        if(pos != noOfRequests - 1)
        {
            totalMovement += absolute(initialPos, 0);
            initialPos = 0;
            printf("0 -> ");
            for(i = pos + 1; i < noOfRequests ;i++)
            {
                if(i == noOfRequests - 1)
                    printf("%d\n",request[i]);
                else
                    printf("%d -> ",request[i]);
                totalMovement += absolute(initialPos,request[i]);
                initialPos = request[i];
            }
        }
		printf("\nTotal Head Movement : %d",totalMovement);
        printf("\nTotal Seek Time : %d ms", seekTime * totalMovement);
        printf("\nAverage Seek Time: %.2f ms\n", (float)(seekTime * totalMovement) / noOfRequests);
	}
	else
	{
        i = noOfRequests - 1;
        while(request[i] >= initialPos && i >= 0)
		    i--;
		pos = i + 1;
        if(initialPos != request[pos])
		    printf("\nHead Movement : %d -> ",initialPos);
        else
		    printf("\nHead Movement : ");  
		for(i = pos ; i < noOfRequests ; i++)
		{
            if(pos == 0 && i == noOfRequests - 1)
			    printf("%d\n",request[i]);
            else
                printf("%d -> ",request[i]);
			totalMovement += absolute(initialPos,request[i]);
			initialPos = request[i];
		}
        if(pos != 0)
        {
            totalMovement += absolute(initialPos, totalCylinders - 1);
            initialPos = totalCylinders - 1;
            printf("%d -> ",totalCylinders - 1);
            for(i = pos - 1; i >= 0 ;i--)
            {
                if(i == 0)
                    printf("%d\n",request[i]);
                else
                    printf("%d -> ",request[i]);
                totalMovement += absolute(initialPos,request[i]);
                initialPos = request[i];
            }
        }
		printf("\nTotal Head Movement : %d",totalMovement);	
        printf("\nTotal Seek Time : %d ms", seekTime * totalMovement);
        printf("\nAverage Seek Time: %.2f ms\n", (float)(seekTime * totalMovement) / noOfRequests);
	}
}
//Function which perform CSCAN Scheduling
void CSCAN(int request[], int noOfRequests,int totalCylinders, int seekTime, int initialPos, int direction)
{
	int i = 0, totalMovement = 0, pos;
	sort(request, 0, noOfRequests - 1);
	if(direction == 'L')
	{
        while(request[i] <= initialPos && i < noOfRequests)
		    i++;
		pos = i - 1;
        if(initialPos != request[pos])
		    printf("\nHead Movement : %d -> ",initialPos);
        else
		    printf("\nHead Movement : ");     
		for(i = pos ; i >= 0 ; i--)
		{
            if(pos == noOfRequests - 1 && i == 0)
			    printf("%d\n",request[i]);
            else
                printf("%d -> ",request[i]);
			totalMovement += absolute(initialPos,request[i]);
			initialPos = request[i];
		}
        if(pos != noOfRequests - 1)
        {
            totalMovement += absolute(initialPos, 0);
            totalMovement += absolute(0, totalCylinders - 1);
            printf("0 -> %d ->", totalCylinders - 1);
            initialPos = totalCylinders - 1;
            for(i = noOfRequests - 1; i > pos ;i--)
            {
                if(i == pos + 1)
                    printf("%d\n",request[i]);
                else
                    printf("%d -> ",request[i]);
                totalMovement += absolute(initialPos,request[i]);
                initialPos = request[i];
            }
        }
		printf("\nTotal Head Movement : %d",totalMovement);
        printf("\nTotal Seek Time : %d ms", seekTime * totalMovement);
        printf("\nAverage Seek Time: %.2f ms\n", (float)(seekTime * totalMovement) / noOfRequests);	
	}
	else
	{
        i = noOfRequests - 1;
        while(request[i] >= initialPos && i >= 0)
		    i--;
		pos = i + 1;
        if(initialPos != request[pos])
		    printf("\nHead Movement : %d -> ",initialPos);
        else
		    printf("\nHead Movement : ");        
		for(i = pos ; i < noOfRequests ; i++)
		{
            if(pos == 0 && i == noOfRequests - 1)
			    printf("%d\n",request[i]);
            else
                printf("%d -> ",request[i]);
			totalMovement += absolute(initialPos,request[i]);
			initialPos = request[i];
		}
        if(pos != 0)
        {
            totalMovement += absolute(initialPos, totalCylinders - 1);
            totalMovement += absolute(totalCylinders - 1, 0);
            initialPos = 0;
            printf("%d -> 0 -> ",totalCylinders - 1);
            for(i = 0; i < pos ; i++)
            {
                if(i == pos - 1)
                    printf("%d\n",request[i]);
                else
                    printf("%d -> ",request[i]);
                totalMovement += absolute(initialPos,request[i]);
                initialPos = request[i];
            }
        }
		printf("\nTotal Head Movement : %d",totalMovement);	
        printf("\nTotal Seek Time : %d ms", seekTime * totalMovement);
        printf("\nAverage Seek Time: %.2f ms\n", (float)(seekTime * totalMovement) / noOfRequests);
	}
}
int main()
{
	int request[1000], noOfRequests, totalCylinders, initialPos, i, option, seekTime;
	char choice, direction;
	printf("Menu\n1.FCFS\n2.SCAN\n3.CSCAN\n4.Exit\n");
	do{
		printf("\nChoice : ");
		scanf("%d",&option);
		if(option == 4)
			break;
		printf("Total Number of Cylinders : ");
		scanf("%d",&totalCylinders);
        printf("Seek Time per Cylinder in ms : ");
        scanf("%d",&seekTime);
		printf("Number of Requests : ");
		scanf("%d",&noOfRequests);
		printf("Request Queue : \n");
		for(i = 0 ; i < noOfRequests ;i++)
			scanf("%d",&request[i]);
		printf("Initial Position of Head : ");
		scanf("%d",&initialPos);
		switch(option)
		{
			case 1  : FCFS(seekTime,request,noOfRequests,initialPos);
					  break;
			case 2  : printf("Initial Direction(R/L) : "); //R - Left -> Right and L - Right -> Left
					  scanf(" %c",&direction);
					  SCAN(request,noOfRequests,totalCylinders,seekTime,initialPos,direction);
					  break;
			case 3  : printf("Initial Direction(R/L) : ");
					  scanf(" %c",&direction);
                      CSCAN(request,noOfRequests,totalCylinders,seekTime,initialPos,direction);
					  break;
			default : printf("Wrong Input !\n"); 
		}
		printf("Do you want to continue ?(Y/N)");
		scanf(" %c",&choice);
	}while(choice == 'Y' || choice == 'y');
	return 0;
}