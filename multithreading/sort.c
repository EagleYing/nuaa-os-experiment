#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

#define NR_TOTAL 10
#define NR_CPU 2
#define NR_CHILD (NR_TOTAL/NR_CPU)

int array[] = {5,6,3,7,8,0,9,1,4,2};

struct param {
	int *array;
    int start;
    int end;
};

struct result {
    int *res;
};

void SelectionSort(int *num, int start, int end)  
{  
	int i;  
	int min;  
	int j;  
	int tmp;  
	int n = end - start;
	for(i = start; i < start + n; i++)  
	{  
		min = i;   
		for(j = i; j < start + n; j++)//find minnum   
		{  
			if(num[min]>num[j])  
			{  
				min = j;  
			}  
		}  
		if(min != i)//change
		{  
			tmp = num[min];  
			num[min] = num[i];  
			num[i] = tmp;  
		}  
	}  
}

void *Sort(void *arg)
{
	struct param *param;
    struct result *result;
    int i;
	int *res;

    param = (struct param *)arg;	
	SelectionSort(array, param->start, param->end);
	printf("selectsort %d - %d: ",param->start, param->end);

	res = array;
    result = malloc(sizeof(struct result));
    result->res = res;
	for(i=0;i<NR_TOTAL;i++){
		printf("%d ",res[i]);
	}
	printf("\n");
    return result;
}

void Merge(int A[], int B[], int L, int R, int rightend)
{
    int leftend = R-1;
    int p = L,i;
    int num = rightend-L+1;
    while(L <= leftend && R <= rightend)
        if(A[L] <= A[R])
            B[p++] = A[L++];
        else
            B[p++] = A[R++];
    while(L <= leftend)
        B[p++] = A[L++];
    while(R <= rightend)
        B[p++] = A[R++];
    for(i = 0; i < num; i++, rightend--)
        A[rightend] = B[rightend];
}
void MSort(int A[],int B[],int L, int rightend)
{
    int mid;
    if(L < rightend)
    {
        mid = (L + rightend)/2;
        MSort(A, B, L, mid);
        MSort(A, B, mid+1, rightend);
        Merge(A, B, L, mid+1, rightend);
    }
}

void Merge_sort(int A[],int N)
{
    int *Temp=(int *)malloc(N*sizeof(int));
    if(Temp)
    {
        MSort(A,Temp,0,N-1);
        free(Temp);
    }
    else
        printf("no space!\n");
}

int main()
{ 
    pthread_t workers[NR_CPU];
    struct param params[NR_CPU]; 
    int i,j;

    for (i = 0; i < NR_CPU; i++) {
        struct param *param;
        param = &params[i];
        param->start = i * NR_CHILD; 
        param->end = (i + 1) * NR_CHILD - 1;
		//printf("%d ",param->start);
		//printf("%d ",param->end);
        pthread_create(&workers[i], NULL, Sort, param);
    }

    for (i = 0; i < NR_CPU; i++) {
        struct result *result;
        pthread_join(workers[i], (void **)&result);
        free(result);
    }
	
	Merge_sort(array, NR_TOTAL);

	printf("Merge sort: ");
    for(i = 0; i < NR_TOTAL; i++)
	{
		printf("%d ", array[i]);
	}
	printf("\n");
    return 0;
}