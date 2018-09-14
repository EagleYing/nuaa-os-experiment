#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<stdlib.h>

char buff1[5];
char buff2[5];
int index1 = 0;
int index2 = 0;

//mutex
sem_t mutex1;
sem_t mutex2;

sem_t full1;
sem_t empty1;
sem_t full2;
sem_t empty2;

void init_mutex(){
    sem_init(&mutex1,0,1);
    sem_init(&mutex2,0,1);
    sem_init(&full1,0,0);
    sem_init(&empty1,0,4);
    sem_init(&full2,0,0);
    sem_init(&empty2,0,4);
}
void destroy_mutex(){
    sem_destroy(&mutex1);
    sem_destroy(&mutex2);
    sem_destroy(&full1);
    sem_destroy(&empty1);
    sem_destroy(&full2);
    sem_destroy(&empty2);
}


void *producer(void *arg){
    int index;
    for(index=0;index<8;index++){
        sem_wait(&empty1);
        sem_wait(&mutex1);
        buff1[index1] = index+'a';
        printf("put %c into buf1\n", index+'a');
        index1++; 
        sem_post(&mutex1);
        sem_post(&full1);
    }
    return (void*)0;

}

void *lowToUp(void *arg){
    int index;
    for(index=0;index<8;index++){
        sem_wait(&full1);
        sem_wait(&empty2);
        sem_wait(&mutex1);
        sem_wait(&mutex2);
        index1--;
        buff2[index2] = buff1[index1]-32;
        printf("Change %c to %c \n", buff1[index1], buff2[index2]);
        index2++;
        sem_post(&mutex2);
        sem_post(&mutex1);
        sem_post(&full2);
        sem_post(&empty1);
    }
    return (void*)0;
}

void *consume(void *arg){
    int index;
    for(index=0;index<8;index++){
        sem_wait(&full2);
        sem_wait(&mutex2);
        index2--;
        printf("Get %c from buf1 to buf2\n", buff2[index2]);
        sem_post(&mutex2);
        sem_post(&empty2);
    }
    return (void*)0;
}


int main(){
    pthread_t produce_thread;
    pthread_t calc_thread;
    pthread_t consume_thread;
  
    init_mutex();

    pthread_create(&produce_thread,NULL,producer,NULL);
    pthread_create(&calc_thread,NULL,lowToUp,NULL);
    pthread_create(&consume_thread,NULL,consume,NULL);
	
    pthread_join(produce_thread,NULL);
    pthread_join(calc_thread,NULL);
    pthread_join(consume_thread,NULL);
	
    destroy_mutex();
    return 0;
}