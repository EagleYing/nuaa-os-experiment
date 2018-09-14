#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

char buf1[5];
char buf2[5];
int index1 = 0;
int index2 = 0;

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_cond_t empty1;
pthread_cond_t full1;
pthread_cond_t empty2;
pthread_cond_t full2;

void Init_mutex()
{
    pthread_mutex_init(&mutex1,NULL);
    pthread_mutex_init(&mutex2,NULL);
    pthread_cond_init(&empty1,NULL);
    pthread_cond_init(&full1,NULL);
    pthread_cond_init(&empty2,NULL);
    pthread_cond_init(&full2,NULL);
}

void Destory_mutex()
{
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    pthread_cond_destroy(&empty1);
    pthread_cond_destroy(&full1);
    pthread_cond_destroy(&empty2);
    pthread_cond_destroy(&full2);
}

void *Producer(void *arg)
{
    int index;
    for(index=0;index<8;index++){
        pthread_mutex_lock(&mutex1);
        while(index1>=1){
            pthread_cond_wait(&empty1,&mutex1);
        }
        buf1[index1] = index+'a';
        printf("Put %c into buffer1\n", buf1[index1]);
        index1++;
        pthread_cond_signal(&full1);
        pthread_mutex_unlock(&mutex1);
    }
    return (void*)0;
}

void *Transfer(void *arg){

    int index;
    for(index=0;index<8;index++){
        pthread_mutex_lock(&mutex1);
        pthread_mutex_lock(&mutex2);
        while(index1<1){
            pthread_cond_wait(&full1,&mutex1);
        }
        while(index2>=1){
            pthread_cond_wait(&empty2,&mutex2);
        }
        index1--;
        buf2[index2] = buf1[index1]-32;
		printf("Transfer %c to %c\n", buf1[index1], buf1[index1] - 32);
        index2++;
        pthread_cond_signal(&empty1);
        pthread_cond_signal(&full2);      
        pthread_mutex_unlock(&mutex2);
        pthread_mutex_unlock(&mutex1);
    }
    return (void*)0;
}

void *Consume(void *arg){
    int index;
    for(index=0;index<8;index++){
        pthread_mutex_lock(&mutex2);
        while(index2<1){
            pthread_cond_wait(&full2,&mutex2);
        }
        index2--;
        printf("Get %c from buffer2\n",buf2[index2]);
        pthread_cond_signal(&empty2);
        pthread_mutex_unlock(&mutex2);
    }
    return (void*)0;
}

int main(){
    Init_mutex();
    pthread_t producer;
    pthread_t transfer;
    pthread_t consume;
    pthread_create(&producer,NULL,Producer,NULL);
    pthread_create(&transfer,NULL,Transfer,NULL); 
    pthread_create(&consume,NULL,Consume,NULL);

    pthread_join(producer,NULL);
    pthread_join(transfer,NULL);
    pthread_join(consume,NULL);
    Destory_mutex();
    return 0;
}