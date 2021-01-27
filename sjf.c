//Name   : sjf.c
//Desc   : To implemenet the SJF Scheduling Algorithm
//Input  : Set of n processes
//Output : The Turnaround and Waiting Time of Each Process, as well as the Average TAT and Average WT.
//Author : Sheen Xavier A
//Date   : 24/08/20

#include <stdio.h>

struct process{ //Structure of a process 
    int pid;
    int burstTime;
    int arrivalTime;
    int turnAroundTime;
    int waitingTime;
};

int main()
{
    int n, completed = 0, time = -1 , i, completionTime = 0, currProcess = -1, flag, totalTime = 0, avgTurnAroundTime = 0, avgWaitingTime = 0;
    printf("Enter the number of processes : "); //Reading the details of a process
    scanf("%d", &n);
    struct process processes[n];
    printf("Enter the details in the increasing order of arrival time: \n");
    for(i = 0; i < n; i++)
    {
        printf("---------------------------------\n");
        printf("PROCESS ID: ");
        scanf("%d", &processes[i].pid);
        printf("ARRIVAL TIME: ");
        scanf("%d", &processes[i].arrivalTime);
        if(time == -1)
            time = processes[i].arrivalTime;
        printf("BURST TIME: ");
        scanf("%d", &processes[i].burstTime);
        printf("---------------------------------\n");
    }

    printf("The Blue Print of Gantt Chart : \n");
    printf("Process\t Time Slice \n");
    printf("---------------------------------\n");
    while(completed < n)
    {
        flag = 1;
        currProcess = -1;
        for(i = 0 ; i < n ; i++)
        {
            if(time >= processes[i].arrivalTime && processes[i].burstTime != -1) //Finding the smallest process at the present time
            {
                if(flag || processes[i].burstTime < processes[currProcess].burstTime)
                {
                    currProcess = i;
                    flag = 0;
                }
            }
        }
        if(currProcess != -1)
        {
            completionTime = time + processes[currProcess].burstTime; // Performing the required computation on the smallest process (at present)
            processes[currProcess].turnAroundTime = completionTime - processes[currProcess].arrivalTime;
            processes[currProcess].waitingTime = time - processes[currProcess].arrivalTime;
            printf("P%d    : %d - %d\n",currProcess, time, completionTime);
            time = completionTime;
            processes[currProcess].burstTime = -1;
            completed++;
        }
    }
    printf("---------------------------------\n");
    printf("The Turn Around Time and Waiting Time of all Processes");
    for(i = 0; i < n; i++)
    {
        printf("---------------------------------\n"); //Displaying the WT and TAT details of each process
        printf("PROCESS P%d\n",processes[i].pid);
        printf("TURNAROUNDTIME TIME: %d\n", processes[i].turnAroundTime);
        avgTurnAroundTime += processes[i].turnAroundTime;
        printf("WAITING TIME: %d\n", processes[i].waitingTime);
        avgWaitingTime += processes[i].waitingTime;
        printf("---------------------------------\n");
    }
    printf("AVG TURNAROUND TIME : %.4f\nAVG WAITING TIME : %.4f\n", (float)(avgTurnAroundTime) / n , (float)(avgWaitingTime) / n);
}