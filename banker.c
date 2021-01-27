// Name  : banker.c
// Date  : 14/09/20
// Aim   : To implement Banker's algorithm to check whether a state is safe or not.
// Input : Reads an array, resources and two matrices(allocation and max) which stores the state to be checked. 
// Output: Display whether the entered state is safe or not.
// Author: Sheen Xavier A

#include<stdio.h>

//Function which returns the number of processes not yet allocated
int processed(int finish[], int length) 
{
	int i, processes = 0;
	for(i = 0; i < length ; i++)
		if(finish[i] == 0)
			processes ++;
	return processes;
}
//Function which compares need[i] and work[]
int check_true(int need[],int available[],int length)
{
	int i;
	for(i = 0; i < length; i++)
		if(need[i] > available[i])
			return 0;
	return 1;
}
//Function which adds the deallocated instances of resources to work
void add_available(int available[],int allocation[],int length)
{
	int i;
	for(i = 0; i < length; i++)
		available[i] = available[i] + allocation[i];
}
//Displays the initital values of max, allocation and availabale
void display_snapshot(int allocation[][10],int max[][10], int available[],int m,int n)
{
	int i,j;
	printf("Processes          Allocation              Max          Available\n\n");
	for(i = 0 ;i < n ; i++)
	{
		printf("    P%d               ",i);
		for(j = 0; j < m; j++)
			printf("%d ",allocation[i][j]);
		printf("            ");
		for(j = 0; j < m;j++ )
			printf("%d ",max[i][j]);
		printf("         ");
		if(i == 0)
		{
			for(j = 0; j < m;j++ )
				printf("%d ",available[j]);
		}
		printf("\n");
	}		
}
void main()
{
	int available[10], resources[10], max[10][10], allocation[10][10], need[10][10], finish[10], sequence[10], sum, i, j, k, m , n, processes, processes_previous;	
	printf("No of Processes    : ");
	scanf("%d", &n);

	printf("Types of Resources : ");
	scanf("%d", &m);

	//The Allocation Matrix shows the number of instances of each type of resource that is currently allocated to each process
	printf("Enter the Allocation Matrix   : \n");
	for(i = 0; i < n; i++)
		for(j = 0; j < m; j++)
			scanf("%d",&allocation[i][j]);

	//The Max Matrix matrix stores the maximum number of instances of each type of resource that can be requested by each processes
	printf("Enter the Max Matrix          : \n");
	for(i = 0; i < n; i++)
		for(j = 0; j < m; j++)
			scanf("%d",& max[i][j]);

	//The Resources array which stores the maximum number of instances of each resource
	printf("Enter the Resources Array     : \n");
	for(i = 0; i < m; i++)
		scanf("%d",&resources[i]);
	//Initializing the available array
	for(i = 0; i < m; i++)
	{
		sum = 0;
		for(j = 0;j < n; j++)
			sum += allocation[j][i];
		available[i] = resources[i] - sum;
	}
	//Need[i][j] = Max[i][j] - Allocation[i][j]
	for(i = 0 ;i < n; i++)
		for(j = 0 ;j < m ; j++)
			need[i][j] = max[i][j] - allocation[i][j];
	
	printf("\nSnapshot of Initital States \n\n");
	display_snapshot(allocation,max,available,m,n);

	//Making finish[i] = false;
	for(i = 0; i < n; i++)
		finish[i] = 0;

	k = 0;
	processes = processed(finish,n);
	while(processes > 0)
	{
		processes_previous = processes; 
		for(i = 0; i < n ; i++)
			if(finish[i] == 0 && check_true(need[i], available,m))
			{
				add_available(available,allocation[i],m);
				finish[i] = 1;
				sequence[k++] = i;
				processes--;
			}
		// If no change in the number of processes remaining before and after checking if any of the process can be accomodated then the system is in unsafe state
		if(processes_previous == processes)
			break;
	}
	if(processes == 0)
	{
		printf("\nThe System is in a Safe State and the corresponding sequence of execution is  : \n");
		// Displaying the sequence of safe state
		for(i = 0; i < n; i++)
		{
			if(i != n - 1)
				printf("P%d => ",sequence[i]);
			else
				printf("P%d\n",sequence[i]);
		}
	}
	else
		printf("\nThe System is in an Unsafe State\n");

}