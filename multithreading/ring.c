#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

#define N 5
#define addnum 12

int count = 1;

struct param {
    int n;
};

struct result {
    float sum;
};

void *Add(void *arg)
{
    struct param *param;
    struct result *result;
    int sum = 0;
    param = (struct param *)arg;
	printf("%d + 1",count);
	count++;
	param->n = count;
	sum = param->n;
    result = malloc(sizeof(struct result));
    result->sum = sum;
	printf("= %d\n", sum);
    return result;
}

int main()
{ 
    pthread_t workers[N];
    struct param params[N]; 
    int i,sum;

    for (i = 0; i < addnum; i++) 
	{
		int pid = i % N;
        struct param *param;
		struct result *result;
        param = &params[pid];
		printf("T%d :", pid);
        pthread_create(&workers[pid], NULL, Add, param);
		pthread_join(workers[pid], (void **)&result);
		sum = result->sum;
	    free(result);
    }
	
    printf("sum = %d\n", sum);
    return 0;
}