/*************************************************************************************** 
  *Copyright(C),Chenyang
  *FileName: ProducerConsumerWin32.c
  *Author: 王晨阳
  *Date: 2021-06-01
  *Description: 
        Using Win32 api to accomplish the task of PRODUCER_CONSUMER_PROBLEM.
        Input should be like [number of producer]
                             [number of consumer] 
                             [number of items every producer produced] 
                             [number of items every consumer consumed].
        Sample input and output are listed at the end of the program.
****************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

typedef int buffer_item;
#define BUFFER_SIZE 5                     //buffer can contain 5 items
#define MAX_SLEEP_TIME 500                //sleep for 500 ms

HANDLE full, empty, mutex;                //semaphore & mutex

buffer_item buffer[BUFFER_SIZE];          //a circular queue
int top, rear;                            //head and tail of the circular queue

int ProducerCount,     ConsumerCount;     //number of producers/consumers
int ProducerItemCount, ConsumerItemCount; //number of items every producer produced/consumer consumed

int insert_item(buffer_item item)
{
    WaitForSingleObject(empty, INFINITE);
    WaitForSingleObject(mutex, INFINITE);

    //critical section
    buffer[rear] = item;
    rear = (rear == BUFFER_SIZE - 1) ? 0 : rear + 1;

    ReleaseMutex(mutex);
    ReleaseSemaphore(full,  1, NULL);

    return 0;
}

int remove_item(buffer_item *item)
{
    WaitForSingleObject(full,  INFINITE);
    WaitForSingleObject(mutex, INFINITE);

    //critical section
    *item = buffer[top];
    top = (top == BUFFER_SIZE - 1) ? 0 : top + 1;

    ReleaseMutex(mutex);
    ReleaseSemaphore(empty, 1, NULL);

    return 0;
}

DWORD WINAPI producer(LPVOID Param)
{
    DWORD User = *(DWORD *)Param;
    srand(time(NULL) + User); //ensure randomness
    for (DWORD i = 1; i <= ProducerItemCount; i++)
    {
        Sleep(rand() % MAX_SLEEP_TIME + 1);
        buffer_item item = rand();
        if (insert_item(item))
            fprintf(stderr, "report error condition\n");
        else
            printf("producer %d produced %d\n", User, item);
    }
}

DWORD WINAPI consumer(LPVOID Param)
{
    DWORD User = *(DWORD *)Param;
    srand(time(NULL) + User + ProducerCount); //ensure randomness
    for (DWORD i = 1; i <= ConsumerItemCount; i++)
    {
        Sleep(rand() % MAX_SLEEP_TIME + 1);
        buffer_item item;
        if (remove_item(&item))
            fprintf(stderr, "report error condition\n");
        else
            printf("consumer %d consumed %d\n", User, item);
    }
}

int main(int argc, char *argv[])
{
    //check for parameter
    if (argc != 5)
    {
        fprintf(stderr, "Parameter is required\n");
        return -1;
    }

    ProducerCount     = atoi(argv[1]);
    ConsumerCount     = atoi(argv[2]);
    ProducerItemCount = atoi(argv[3]);
    ConsumerItemCount = atoi(argv[4]);

    if (ProducerCount * ProducerItemCount < ConsumerCount * ConsumerItemCount)
    {
        fprintf(stderr, "There should be more items produced than consumed\n");
        return -1;
    }

    //initialize
    memset(buffer, 0, sizeof(buffer));
    top = rear = 0;

    //create semaphore
    full  = CreateSemaphore(NULL, 0, BUFFER_SIZE, NULL);
    empty = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL);
    mutex = CreateMutex(NULL, FALSE, NULL);

    //create thread
    DWORD  ProducerId [ProducerCount];
    DWORD  ConsumerId [ConsumerCount];
    HANDLE PRODUCER   [ProducerCount];
    HANDLE CONSUMER   [ConsumerCount];
    DWORD  CurProducer[ProducerCount]; //record the producer/consumer the thread belongs to
    DWORD  CurConsumer[ConsumerCount];

    for (DWORD i = 0; i < ProducerCount; i++)
    {
        CurProducer[i] = i;
        PRODUCER[i] = CreateThread(NULL, 0, producer, &CurProducer[i], 0, &ProducerId[i]);
    }
    for (DWORD i = 0; i < ConsumerCount; i++)
    {
        CurConsumer[i] = i;
        CONSUMER[i] = CreateThread(NULL, 0, consumer, &CurConsumer[i], 0, &ConsumerId[i]);
    }

    //terminate thread
    for (DWORD i = 0; i < ProducerCount; i++)
        if (PRODUCER[i] != NULL)
        {
            WaitForSingleObject(PRODUCER[i], INFINITE);
            CloseHandle(PRODUCER[i]);
        }
    for (DWORD i = 0; i < ConsumerCount; i++)
        if (CONSUMER[i] != NULL)
        {
            WaitForSingleObject(CONSUMER, INFINITE);
            CloseHandle(CONSUMER);
        }

    return 0;
}
/*
EXAMPLE COMMAND TO RUN THE PROGRAM (system: Windows10 21H2, software: Powershell):
gcc ProducerConsumerWin32.c -o ProducerConsumerWin32
./ProducerConsumerWin32 5 5 4 4

EXAMPLE OUTPUT:
producer 2 produced 11459
consumer 4 consumed 11459
producer 1 produced 711
producer 0 produced 22730
producer 3 produced 22208
consumer 3 consumed 711
consumer 1 consumed 22730
producer 4 produced 188
consumer 0 consumed 22208
consumer 2 consumed 188
producer 2 produced 3150
consumer 4 consumed 3150
producer 0 produced 20559
consumer 1 consumed 20559
producer 0 produced 22627
producer 3 produced 27213
producer 1 produced 11854
producer 2 produced 30481
consumer 3 consumed 22627
consumer 2 consumed 27213
producer 4 produced 18508
consumer 0 consumed 11854
consumer 4 consumed 30481
producer 2 produced 9851
producer 0 produced 15706
consumer 1 consumed 18508
producer 1 produced 26554
consumer 2 consumed 9851
consumer 4 consumed 15706
consumer 0 consumed 26554
producer 3 produced 1639
consumer 3 consumed 1639
producer 4 produced 5566
consumer 1 consumed 5566
producer 3 produced 6924
consumer 2 consumed 6924
producer 1 produced 12778
consumer 0 consumed 12778
producer 4 produced 3996
consumer 3 consumed 3996
*/