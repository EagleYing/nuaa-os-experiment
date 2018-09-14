#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

#define SIZE 4
#define TIME 5

char buf1[SIZE];

int in1;
int out1;

typedef struct {
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}sema_t;

sema_t m1;
sema_t f1;
sema_t e1;

void sema_init(sema_t *sema, int value)
{
	sema->value=value;
	pthread_mutex_init(&sema->mutex,NULL);
	pthread_cond_init(&sema->cond,NULL);
}

void sema_wait(sema_t *sema)
{
	pthread_mutex_lock(&sema->mutex);
	sema->value--;
	while(sema->value<0)
		pthread_cond_wait(&sema->cond,&sema->mutex);
    pthread_mutex_unlock(&sema->mutex);
}

void sema_post(sema_t *sema)
{
	pthread_mutex_lock(&sema->mutex);
	sema->value++;
	pthread_cond_signal(&sema->cond);
	pthread_mutex_unlock(&sema->mutex);
}

int get_item1()
{
    int item;
	item = buf1[out1];
	out1 = (out1+1) % SIZE;
	return item;
}

void put_item1(int item)
{
	buf1[in1] = item;
	in1 = (in1+1) % SIZE;
}

void *produce(void * arg)
{
	int i,item;
	for(i=0;i<TIME;i++)
	{
		sema_wait(&e1);
		sema_wait(&m1);
        item = i + 'a';
		printf("put %c into buf1\n",item);
		put_item1(item);
		sema_post(&m1);
		sema_post(&f1);
	}
	return NULL;
}

void *custom(void *arg)
{
	int i,item;
	for(i=0;i<TIME;i++)
	{
		sema_wait(&f1);
		sema_wait(&m1);
		item = get_item1();
		printf("get %c from bu1\n",item);
		sema_post(&e1);
		sema_post(&m1);
	}
	return NULL;
}

int main()
{
	sema_init(&m1, 1);
	sema_init(&e1, SIZE);
	sema_init(&f1, 0);

	pthread_t p,c;
	pthread_create(&p,NULL,produce,NULL);
	pthread_create(&c,NULL,custom,NULL);
	pthread_join(p,NULL);
	pthread_join(c,NULL);
	return 0;
}



