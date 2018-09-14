#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

typedef struct{
    int value;
    pthread_mutex_t m;
    pthread_cond_t c;
}sema_t;

//sema_t t12;
//sema_t t13;
sema_t t1;
sema_t t2;
sema_t t3;

void sema_init(sema_t *s,int value){
   s->value=value;
   pthread_mutex_init(&s->m,NULL);
   pthread_cond_init(&s->c,NULL);
}

void sema_wait(sema_t *s){
   pthread_mutex_lock(&s->m);
   s->value--;
   while(s->value<0)
       pthread_cond_wait(&s->c,&s->m);
   pthread_mutex_unlock(&s->m);
}

void sema_post(sema_t *s){
   pthread_mutex_lock(&s->m);
   s->value++;
   pthread_cond_signal(&s->c);
   pthread_mutex_unlock(&s->m);
}

void *T1_entry(void *arg){
   puts("T1");
   sema_post(&t1);
   sema_post(&t1);
}

void *T2_entry(void *arg){
   sema_wait(&t1);
   puts("T2");
   sema_post(&t2);
   sema_post(&t1);
}

void *T3_entry(void *arg){
   sema_wait(&t1);
   puts("T3");
   sema_post(&t3);
   sema_post(&t1);
}

void *T4_entry(void *arg){
   sema_wait(&t2);
   sema_wait(&t3);
   puts("T4");
}

int main(){
   sema_init(&t1,0);
   sema_init(&t2,0);
   sema_init(&t3,0);
   pthread_t pt1,pt2,pt3,pt4;
   pthread_create(&pt1,NULL,T1_entry,NULL);
   pthread_create(&pt2,NULL,T2_entry,NULL);
   pthread_create(&pt3,NULL,T3_entry,NULL);
   pthread_create(&pt4,NULL,T4_entry,NULL);
   pthread_join(pt1,NULL);
   pthread_join(pt2,NULL);
   pthread_join(pt3,NULL);
   pthread_join(pt4,NULL);
   return 0;
}
