//Name   : rrpre.c
//Desc   : To implemenet the Round Robin Pre-Emptive Scheduling Algorithm
//Input  : Set of n processes
//Output : The Turnaround and Waiting Time of Each Process, as well as the Average TAT and Average WT.
//Author : Sheen Xavier A
//Date   : 24/08/20

#include <stdio.h>
struct process{ // Structure of a process
    int pid;
    int burstTime;
    int arrivalTime;
    int turnAroundTime;
    int waitingTime;
    int inQueue;
};

int front = -1, rear = -1;

void enqueue(int queue[], int id, int size) //Enqueue operation of Circular Queue
{
    if((front == 0 && rear == size - 1) || (front == rear + 1))
    {
        return;
    }
    else
    {
        if(front == -1)
            front = 0;
        rear = (rear + 1) % size;
        queue[rear] = id;
    }
}

int dequeue(int queue[], int size) //Dequeue operation of Circular Queue
{
    int id = queue[front];
    if(front == rear)
    {
        front = -1;
        rear = -1;
    }
    else
        front = (front + 1) % size;
    return id; 
}

int isEmpty()
{
    if(front == -1)
        return 1;
    return 0;
}

int main()
{
    int n, completed = 0, time = 0, i, completionTime = 0, quantaSize, currProcess, avgTurnAroundTime = 0, avgWaitingTime = 0;
    printf("Enter the number of processes : "); //Reading the data of each process
    scanf("%d", &n);
    struct process processes[n];
    int queue[n];
    printf("Enter the details in the increasing order of arrival time: \n");
    for(i = 0; i < n; i++)
    {
        printf("---------------------------------\n");
        printf("PROCESS ID: ");
        scanf("%d", &processes[i].pid);
        printf("ARRIVAL TIME: ");
        scanf("%d", &processes[i].arrivalTime);
        printf("BURST TIME: ");
        scanf("%d", &processes[i].burstTime);
        printf("---------------------------------\n");
        processes[i].waitingTime = 0;
        processes[i].inQueue = 0;
    }

    printf("Enter the size of each Quanta : ");
    scanf("%d",&quantaSize);
    enqueue(queue, 0, n);
    processes[0].inQueue = 1;

    printf("The Blue Print of Gantt Chart : \n");
    printf("Process\t Time Slice \n");
    printf("---------------------------------\n");
    while(!isEmpty())
    {
        currProcess = dequeue(queue, n);
        processes[currProcess].inQueue = 0;
        if(processes[currProcess].burstTime > quantaSize) // If burst size of the process is greater than quanta size we move the process to the ready queue
        {
            completionTime = time + quantaSize;
            processes[currProcess].burstTime -= quantaSize;
            processes[currProcess].waitingTime -= quantaSize;
            printf("P%d    : %d - %d\n",currProcess, time, completionTime);
            time = completionTime;
        }
        else if(processes[currProcess].burstTime != -1) // If the process is complete we make burst size -1 to indicate the process is complete
        {
            completionTime = time + processes[currProcess].burstTime;
            processes[currProcess].turnAroundTime = completionTime - processes[currProcess].arrivalTime;
            processes[currProcess].waitingTime -=  processes[currProcess].burstTime - processes[currProcess].turnAroundTime;
            processes[currProcess].burstTime = -1;
            printf("P%d    : %d - %d\n",i, time, completionTime);
            time = completionTime;    
        }
        for(i = 0 ; i < n ; i++)
        {
            if(i == currProcess)
                continue;
            if(processes[i].burstTime != -1 && time >= processes[i].arrivalTime && processes[i].inQueue == 0) //The inQueue varible is used to check whether the process is already in the queue
            {
                enqueue(queue, i, n);
                processes[i].inQueue = 1;
            }
        }

        if(processes[currProcess].burstTime != -1)
        {
            enqueue(queue, currProcess, n);
            processes[currProcess].inQueue = 1;
        }
    }
    printf("---------------------------------\n");
    printf("The Turn Around Time and Waiting Time of all Processes \n"); //Displaying the WT and TAT of each process
    for(i = 0; i < n; i++)
    {
        printf("---------------------------------\n");
        printf("PROCESS P%d\n",processes[i].pid);
        printf("TURNAROUND TIME: %d\n", processes[i].turnAroundTime);
        avgTurnAroundTime += processes[i].turnAroundTime;
        printf("WAITING TIME: %d\n", processes[i].waitingTime);
        avgWaitingTime += processes[i].waitingTime;
        printf("---------------------------------\n");
    }
    printf("AVG TURNAROUND TIME : %.4f\nAVG WAITING TIME : %.4f\n", (float)(avgTurnAroundTime) / n , (float)(avgWaitingTime) / n);
}