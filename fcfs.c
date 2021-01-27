//Name   : fsfs.c
//Desc   : To implemenet the FCFS Scheduling Algorithm
//Input  : Set of n processes
//Output : The Turnaround and Waiting Time of Each Process, as well as the Average TAT and Average WT.
//Author : Sheen Xavier A
//Date   : 24/08/20

#include <stdio.h>
struct process{  //Structure of Process
    int pid;
    int burstTime;
    int arrivalTime;
    int turnAroundTime;
    int waitingTime;
};

int main()
{
    int n, completed = 0, time = 0, i, completionTime = 0, avgTurnAroundTime = 0, avgWaitingTime = 0;
    printf("Enter the number of processes : "); //Inputting the processes
    scanf("%d", &n);
    struct process processes[n];
    printf("Enter the details in the increasing order of arrival time: \n");
    for(i = 0; i < n; i++)
    {
        printf("---------------------------------\n");
        printf("PROCESS ID: ");
        scanf("%d",&processes[i].pid);
        printf("ARRIVAL TIME: ");
        scanf("%d", &processes[i].arrivalTime);
        printf("BURST TIME: ");
        scanf("%d", &processes[i].burstTime);
        printf("---------------------------------\n");
    }

    printf("The Blue Print of Gantt Chart : \n");
    printf("Process\t Time Slice \n");
    printf("---------------------------------\n");
    for(int i = 0 ; i < n ; i++)
    {
        completionTime = time + processes[i].burstTime;        // Computing the TAT and WT for each process
        processes[i].turnAroundTime = completionTime - processes[i].arrivalTime;
        processes[i].waitingTime = time - processes[i].arrivalTime;
        printf("P%d    : %d - %d\n",i, time, completionTime);
        time = completionTime; 
    }
    printf("---------------------------------\n");
    printf("The Turn Around Time and Waiting Time of all Processes");
    for(i = 0; i < n; i++)      // Displaying the WT and TAT data of each process
    {
        printf("---------------------------------\n");
        printf("PROCESS P%d\n",processes[i].pid);
        printf("TURNAROUNDTIME TIME: %d\n", processes[i].turnAroundTime);
        avgTurnAroundTime += processes[i].turnAroundTime;
        printf("WAITING TIME: %d\n", processes[i].waitingTime);
        avgWaitingTime += processes[i].waitingTime;
        printf("---------------------------------\n");
    }
    printf("AVG TURNAROUND TIME : %.4f\nAVG WAITING TIME : %.4f\n", (float)(avgTurnAroundTime) / n , (float)(avgWaitingTime) / n);
}