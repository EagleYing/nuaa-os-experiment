#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

#define NR_TOTAL 20000
#define NR_CPU 2
#define NR_CHILD (NR_TOTAL/NR_CPU)

struct param {
    int start;
    int end;
};

struct result {
    float sum;
};

void *Leibniz(void *arg)
{
    struct param *param;
    struct result *result;
    float sum = 0,buf = 0;
    int i;

    param = (struct param *)arg;
    for (i = param->start; i < param->end; i++)
	{
		if(i%2 == 0)
		{
			buf = 1.0/((i + 1)*2.0 - 1);
		}else if(i%2 != 0)
		{
			buf = -1.0/((i + 1)*2.0 - 1);
		}
		sum += buf;
	}
    result = malloc(sizeof(struct result));
    result->sum = sum;
    return result;
}

int main()
{ 
    pthread_t workers[NR_CPU];
    struct param params[NR_CPU]; 
    int i;

    for (i = 0; i < NR_CPU; i++) {
        struct param *param;
        param = &params[i];
        param->start = i * NR_CHILD; 
        param->end = (i + 1) * NR_CHILD;
        pthread_create(&workers[i], NULL, Leibniz, param);
    }

    float sum = 0;
    for (i = 0; i < NR_CPU; i++) {
        struct result *result;
        pthread_join(workers[i], (void **)&result);
        sum += result->sum;
        free(result);
    }
	float PI = sum*4;

    printf("sum = %f and PI = %f \n", sum, PI);
    return 0;
}