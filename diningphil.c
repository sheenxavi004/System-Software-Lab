//Name   : diningphil.c
//Desc   : Implementation of dining philosopher's problem using semaphores.
//Input  : Nill.
//Output : Generates a the situation of each philosopher with each pass.
//Author : Sheen Xavier A
//Date   : 21 / 09 / 2020

#include <stdio.h>

//Semaphore to keep track of chopsticks
int chopsticks[5];
//Mutex Lock
int mutex = 1;

//Wait function for busy waiting
void wait(int *S)
{
    (*S)--;
}

//Signal function
void signal(int *S)
{
    (*S)++;
}

int main()
{
    int philosophers_completed[5], finished_philosophers = 0, trials = 0, i;

    //Initially all philosopher have not dined
    for(i = 0 ; i < 5 ; i++)
        philosophers_completed[i] = 0;
    //Initializing the chopsticks
    for(int i = 0 ; i < 5 ; i++)
        chopsticks[i] = 1;

    while(trials < 5) //All philosophers should have dined by 5 trials else we have a deadlock
    {
        for(i = 0 ; i < 5 ; i++)
        {
            //Removing dined philosophers from eating
            if(philosophers_completed[i] == 1)
            {
                printf("The philosopher %d has finished eating ! \n", i + 1);
                continue;
            }
            
            //Think
            printf("The philosopher %d is thinking !\n", i + 1);
            
            //If mutex and the chopsticks on right and left are available we can eat
            if(mutex && chopsticks[i] && chopsticks[(i + 1) % 5])
            {
                wait(&mutex);
                wait(&chopsticks[i]);
                wait(&chopsticks[(i + 1) % 5]);

                
                //Eating
                printf("The philosopher %d is eating !\n", i + 1);
                philosophers_completed[i] = 1;
                finished_philosophers++;
                
                signal(&mutex);
            }
        }
        trials++;
        if(finished_philosophers == 5)
            break;
        
        //Dropping all picked up chopsticks
        for(i = 0 ; i < 5 ; i++)
        {
            if(!chopsticks[i])
                signal(&chopsticks[i]);
        }

    }
    if(finished_philosophers == 5)
        printf("All philosophers have dined !\n");
    else
        printf("We have encountered a deadlock \n");
    
    return 0;
}