#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>

typedef struct {
  int value;
  pthread_mutex_t m;
  pthread_cond_t c;
}sema_t;

sema_t m;
sema_t e;
sema_t o;
sema_t even;

int in,out;
int countodd = 0;
int counteven = 0;

#define SIZE 5
#define TIME 5

int buf[SIZE];


void sema_init(sema_t *s,int value)
{
   s->value=value;
   pthread_mutex_init(&s->m,NULL);
   pthread_cond_init(&s->c,NULL);
}

void sema_wait(sema_t *s)
{
	pthread_mutex_lock(&s->m);
	while(s->value<0)
		pthread_cond_wait(&s->c,&s->m);
	s->value--;
    pthread_mutex_unlock(&s->m);
}

void sema_post(sema_t *s)
{
	pthread_mutex_lock(&s->m);
	s->value++;
	pthread_cond_signal(&s->c);
	pthread_mutex_unlock(&s->m);
}

int get_item()
{
    int item;
	item = buf[out];
	out = (out+1) % SIZE;
	return item;
}

void put_item(int item)
{
	buf[in] = item;
	in = (in+1) % SIZE;
}

void *p1(void *arg)
{
	int i,item;
	for(i=0;i<TIME;i++)
	{
		sema_wait(&e);
		sema_wait(&m);
		put_item(i);
		printf("put %d into buf\n",i);
		if(i%2==0){
			sema_post(&even);
		}else{
			sema_post(&o);
		}
		sema_post(&m);
	}
	return NULL;
}

void *p2(void *arg)
{
	int i,item;
	for(i=0;i<TIME/2;i++)
	{
		sema_wait(&o);
		sema_wait(&m);
		item = get_item();
		printf("get odd num %d\n",item);
		countodd++;
		sema_post(&e);
		sema_post(&m);
	}
	return NULL;
}

void *p3(void *arg)
{
	int i,item;
	for(i=0;i<TIME/2+1;i++)
	{
		sema_wait(&even);
		sema_wait(&m);
		item = get_item();
		printf("get even num %d\n",item);
		counteven++;
		sema_post(&e);
		sema_post(&m);
	}
	return NULL;
}

int main()
{
	sema_init(&m,1);
	sema_init(&e,SIZE);
	sema_init(&o,0);
	sema_init(&even,0);
	
	pthread_t pt1,pt2,pt3;
	pthread_create(&pt1,NULL,p1,NULL);
	pthread_create(&pt2,NULL,p2,NULL);
	pthread_create(&pt3,NULL,p3,NULL);
	pthread_join(pt1,NULL);
	pthread_join(pt2,NULL);
	pthread_join(pt3,NULL);
	printf("odd %d, even %d\n",countodd,counteven);
	return 0;
}
			
