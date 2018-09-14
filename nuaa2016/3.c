#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

#define TOTAL 100
#define CPU 10
#define CHILD (TOTAL/CPU)

struct param{
    int start;
    int end;
};
 
struct result{
    int sum;
};

void *add(void *arg){
    struct param *p;
    struct result *r;
    int sum = 0;
    int i;
    p = (struct param *)arg;
    for(i=p->start+1;i<p->end+1;i++){
        sum+=i;
    }
    r = malloc(sizeof(struct result));
    r->sum = sum;
}

int main(){
    pthread_t w[CPU];
    struct param params[CPU];
    int i;
    for(i=0;i<CPU;i++){
        struct param *p;
        p = &params[i];
        p->start = i*CHILD;
        p->end = (i+1)*CHILD;
        pthread_create(&w[i],NULL,add,p);
    }

    int sum = 0;
    for(i=0;i<CPU;i++){
       struct result *r;
       pthread_join(w[i],(void **)&r);
       sum += r->sum;
       free(r);
    }
    
    printf("%d\n",sum);
    return 0;
}
