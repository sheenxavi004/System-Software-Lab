//Name   : priority.c
//Desc   : To implemenet the Priority Scheduling Algorithm
//Input  : Set of n processes
//Output : The Turnaround and Waiting Time of Each Process, as well as the Average TAT and Average WT.
//Author : Sheen Xavier A
//Date   : 24/08/20


#include <stdio.h>

struct process{  //Structure of a process
    int pid;
    int burstTime;
    int arrivalTime;
    int turnAroundTime;
    int waitingTime;
    int priority;
};

int main()
{
    int n, completed = 0, time = -1 , i, completionTime = 0, currProcess = -1, flag, avgTurnAroundTime = 0, avgWaitingTime = 0;
    printf("Enter the number of processes : ");
    scanf("%d", &n);
    struct process processes[n];
    printf("Enter the details in the increasing order of arrival time: \n"); //Reading the details of each process
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
        printf("PRIORITY: ");
        scanf("%d", &processes[i].priority);       
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
            if(time >= processes[i].arrivalTime && processes[i].burstTime != -1) //Find the process which is having the highest priority at present
            {
                if(flag || processes[i].priority < processes[currProcess].priority)
                {
                    currProcess = i;
                    flag = 0;
                }
            }
        }
        if(currProcess != -1)
        {
            completionTime = time + processes[currProcess].burstTime; //Performing the required computation on the high priority process
            processes[currProcess].turnAroundTime = completionTime - processes[currProcess].arrivalTime;
            processes[currProcess].waitingTime = time - processes[currProcess].arrivalTime;
            printf("P%d    : %d - %d\n",currProcess, time, completionTime);
            time = completionTime;
            processes[currProcess].burstTime = -1;
            completed++;
        }
    }
    printf("---------------------------------\n");
    printf("The Turn Around Time and Waiting Time of all Processes"); //Displaying the WT and TAT details of each process
    for(i = 0; i < n; i++)
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