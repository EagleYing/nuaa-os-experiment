#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>

pthread_mutex_t m1;
pthread_mutex_t m2;
pthread_cond_t c2;
pthread_cond_t c3;
pthread_cond_t c4;

int t1=0,t2=0,t3=0;

void *T1(void *arg){
  pthread_mutex_lock(&m1);
  puts("T1");
  t1++;
  pthread_mutex_unlock(&m1);
  pthread_cond_signal(&c2);
  pthread_cond_signal(&c3);
}

void *T2(void *arg){
  pthread_mutex_lock(&m1);
  while(t1==0)
     pthread_cond_wait(&c2,&m1);
  puts("T2");
  pthread_mutex_unlock(&m1);

  pthread_mutex_lock(&m2);
  t2++;
  pthread_mutex_unlock(&m2);
  pthread_cond_signal(&c4);
  return NULL;
}

void *T3(void *arg){
  pthread_mutex_lock(&m1);
  while(t1==0)
     pthread_cond_wait(&c3,&m1);
  puts("T3");
  pthread_mutex_unlock(&m1);

  pthread_mutex_lock(&m2);
  t3++;
  pthread_mutex_unlock(&m2);
  pthread_cond_signal(&c4);
  return NULL;
}

void *T4(void *arg){
  pthread_mutex_lock(&m2);
  while(t2==0||t3==0)
      pthread_cond_wait(&c4,&m2);
  puts("T4");
  pthread_mutex_unlock(&m2);
}

int main(){
   pthread_t p1,p2,p3,p4;
   pthread_mutex_init(&m1,NULL);
   pthread_mutex_init(&m2,NULL);
   pthread_cond_init(&c2,NULL);
   pthread_cond_init(&c3,NULL);
   pthread_cond_init(&c4,NULL);
   pthread_create(&p1,NULL,T1,NULL);
   pthread_create(&p2,NULL,T2,NULL);
   pthread_create(&p3,NULL,T3,NULL);
   pthread_create(&p4,NULL,T4,NULL);
   pthread_join(p1,NULL);
   pthread_join(p2,NULL);
   pthread_join(p3,NULL);
   pthread_join(p4,NULL);
   return 0;
}
