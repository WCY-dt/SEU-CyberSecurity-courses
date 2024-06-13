/*************************************************************************************** 
  *Copyright(C),Chenyang
  *FileName: ProducerConsumerPthread.c
  *Author: 王晨阳
  *Date: 2021-06-01
  *Description: 
        Using Pthread to accomplish the task of PRODUCER_CONSUMER_PROBLEM.
        Input should be like [number of producer]
                             [number of consumer] 
                             [number of items every producer produced] 
                             [number of items every consumer consumed].
        Sample input and output are listed at the end of the program.
****************************************************************************************/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef int buffer_item;
#define BUFFER_SIZE 5                     //buffer can contain 5 items
#define MAX_SLEEP_TIME 500                //sleep for 500 ms

sem_t full, empty;                        //semaphore
pthread_mutex_t mutex;                    //mutex

buffer_item buffer[BUFFER_SIZE];          //a circular queue
int top, rear;                            //head and tail of the circular queue

int ProducerCount, ConsumerCount;         //number of producer/consumer
int ProducerItemCount, ConsumerItemCount; //number of items every producer produced/consumer consumed

int insert_item(buffer_item item)
{
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    //critical section
    buffer[rear] = item;
    rear = (rear == BUFFER_SIZE - 1) ? 0 : rear + 1;

    pthread_mutex_unlock(&mutex);
    sem_post(&full);

    return 0;
}

int remove_item(buffer_item *item)
{
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    //critical section
    *item = buffer[top];
    top = (top == BUFFER_SIZE - 1) ? 0 : top + 1;

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);

    return 0;
}

void *producer(void *Param)
{
    int User = *(int *)Param;
    srand(time(NULL) + User); //ensure randomness
    for (int i = 1; i <= ProducerItemCount; i++)
    {
        sleep((double)(rand() % MAX_SLEEP_TIME + 1) / 1000.000);
        buffer_item item = rand();
        if (insert_item(item))
            fprintf(stderr, "report error condition\n");
        else
            printf("producer %d produced %d\n", User, item);
    }
}

void *consumer(void *Param)
{
    int User = *(int *)Param;
    srand(time(NULL) + User + ProducerCount); //ensure randomness
    for (int i = 1; i <= ConsumerItemCount; i++)
    {
        sleep((double)(rand() % MAX_SLEEP_TIME + 1) / 1000.000);
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
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    pthread_mutex_init(&mutex, NULL);

    //create thread
    pthread_t PRODUCER[ProducerCount];
    pthread_t CONSUMER[ConsumerCount];
    int CurProducer[ProducerCount]; //record the producer/consumer the thread belongs to
    int CurConsumer[ConsumerCount];

    for (int i = 0; i < ProducerCount; i++)
    {
        CurProducer[i] = i;
        pthread_create(&PRODUCER[i], NULL, producer, &CurProducer[i]);
    }
    for (int i = 0; i < ConsumerCount; i++)
    {
        CurConsumer[i] = i;
        pthread_create(&CONSUMER[i], NULL, consumer, &CurConsumer[i]);
    }

    //terminate thread
    for (int i = 0; i < ProducerCount; i++)
        pthread_join(PRODUCER[i], NULL);
    for (int i = 0; i < ConsumerCount; i++)
        pthread_join(CONSUMER[i], NULL);

    return 0;
}
/*
EXAMPLE COMMAND TO RUN THE PROGRAM:
gcc ProducerConsumerPthread.c -o ProducerConsumerPthread -lpthread
./ProducerConsumerPthread 5 5 4 4

EXAMPLE OUTPUT (running on Ubuntu20.04 (WSL)):
producer 1 produced 1823051570
producer 0 produced 1641975433
producer 3 produced 34636998
consumer 0 consumed 1823051570
consumer 1 consumed 34636998
producer 3 produced 1964985380
consumer 2 consumed 1641975433
producer 2 produced 433018536
consumer 4 consumed 433018536
consumer 1 consumed 1592485399
consumer 2 consumed 145032469
producer 2 produced 1608767659
producer 0 produced 145032469
consumer 1 consumed 1362920205
consumer 2 consumed 1608767659
consumer 0 consumed 413485799
consumer 2 consumed 1275366272
producer 2 produced 1275366272
producer 4 produced 1592485399
consumer 3 consumed 1964985380
consumer 4 consumed 867469038
producer 3 produced 1362920205
producer 4 produced 223031996
producer 0 produced 170437731
producer 4 produced 1589508306
consumer 1 consumed 170437731
producer 2 produced 867469038
consumer 0 consumed 223031996
producer 1 produced 413485799
producer 0 produced 933701033
producer 4 produced 1127811455
consumer 0 consumed 1581603209
consumer 4 consumed 933701033
producer 3 produced 1581603209
producer 1 produced 896235234
consumer 3 consumed 1589508306
consumer 4 consumed 1127811455
producer 1 produced 1626311734
consumer 3 consumed 896235234
consumer 3 consumed 1626311734
*/