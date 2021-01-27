//Name   : sprocon.c
//Desc   : Implementation of producer consumer problem using semaphores.
//Input  : Accepts an input which denotes whether we are producing or consuming.
//Output : Generates a message which denotes whether the production or consumption of item was successfull.
//Author : Sheen Xavier A
//Date   : 21 / 09 / 2020

#include <stdio.h>
#define SIZE 5

//Buffer Array
int BUFF[SIZE];
//Keeps track of number of items produced
int full = 0;
//Keeps track of number of position in Buffer which are vacant
int empty = SIZE;
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

//Producer function
void producer()
{
    //Producing an Item
    int prodItem = full + 1;
    printf("Produced the Product No %d\n", prodItem);

    wait(&empty);
    wait(&mutex);

    //Placing the item into the buffer
    BUFF[full] = prodItem;


    signal(&mutex);
    signal(&full);
}

//Consumer Function
void consumer()
{
    int consumItem;

    wait(&full);
    wait(&mutex);

    //Removing an item from buffer

    consumItem = BUFF[full];

    signal(&mutex);
    signal(&empty);

    //Consumed the product consumItem

    printf("Consumed the Product No %d\n", consumItem);
}
int main()
{
    int choice;
    printf("1.PRODUCE\n2.CONSUME\n3.EXIT\n");
    while(1)
    {
        printf("CHOICE : ");
        scanf("%d", &choice);
        if(choice == 3)
        {
            break;
        }
        switch(choice)
        {
            case 1  :   if( mutex && empty != 0 ) //Access allowed only when you have both mutex lock and the the buffer is not full
                            producer();
                        else
                            printf("The Buffer is currently full or you don't have authority of MUTEX lock\n"); 
                        break;

            case 2  :   if( mutex && full != 0 ) //Access allowed only when you have both mutex lock and the the buffer is not empty
                            consumer();
                        else
                            printf("The Buffer is currently empty or you don't have authority of MUTEX lock\n");                             
                        break;

            default : printf("Wrong Input"); 
        }
    }
    return 0;
}