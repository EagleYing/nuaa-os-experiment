#include <stdio.h>

#define POINTERS 4
#define REGION_SIZE (1000)

char region[REGION_SIZE];

struct block {
    struct block *next;
    struct block *prev;
    int in_use;
    int size;
    char data[0];
};

struct block free_list;

void my_malloc_init()
{
    struct block *block = (struct block*)malloc(sizeof(struct block));

    block->in_use = 0;
    block->size = REGION_SIZE;
    block->next = NULL;
    block->prev = &free_list;
    // add block to free_list
    free_list.next = block;
}

void *my_malloc(int size)
{
	struct block *p=&free_list,*pr;
	struct block *newp = (struct block*)malloc(sizeof(struct block));
	pr=p;
	p=p->next;
	int flag=0;
	while(p!=NULL)
	{
		if(p->in_use==0&&p->size>=size)
		{
			flag=1;
			break;
		}
		pr=p;
		p=p->next;
	}
	if(flag==1)
	{
		if(p->size==size)
		{
			p->in_use=0;
			return p;
		}else{
			newp->in_use = 1;
			newp->size = size;
			newp->prev = p->prev;
			newp->next = p;
			p->size -= size;
			p->prev->next = newp;
			p->prev = newp;
			return newp;
		}
	}else{
		return NULL;
	}
}

void my_free(void *p)
{
	struct block *block = (struct block* )p,*pr,*ne;
	if((block->prev!=&free_list&&block->prev->in_use&&block->next->in_use)||(block->prev==&free_list&&block->next==NULL)||(block->prev==&free_list&&block->next->in_use==1))
	{
//		printf("ins\n");
		block->in_use=0;
	}else{
		if(block->next!=NULL&&block->next->in_use==0&&block->prev->in_use==0&&block->prev!=&free_list){
//			printf("in\n");
			pr=block->prev;
			ne=block->next;
			block->size = block->size + block->next->size + block->prev->size;
			block->prev->prev->next = block;
			if(block->next->next!=NULL)
				block->next->next->prev = block;
			block->prev = block->prev->prev;
			block->next = block->next->next;
			block->in_use=0;
			free(pr);
			free(ne);
		}else
		{
//			printf("iqwens\n");
			if(block->next!=NULL&&block->next->in_use==0)
			{
				block->next->size+=block->size;
			}
			else if(block->prev->in_use==0)
			{
				block->prev->size+=block->size;
			}
			block->prev->next = block->next;
			block->next->prev = block->prev;
			free(block);
		}

	} 
}

void printlist()
{
	struct block* block = free_list.next;
	printf("Now the list is \n");
	while(block!=NULL){
		printf("size :%d\n",block->size);
		block = block->next;
	}
}

void test()
{
    int size;
    void *p1,*p2,*p3;

    p1 = my_malloc(10);
    printlist();

    p2 = my_malloc(20);
    printlist();
	
    my_free(p1);
    printlist();	
	
	p3 = my_malloc(50);
    printlist();

    my_free(p3);
    printlist();
	
	my_free(p2);
    printlist();
}

void test1()
{
    void *array[POINTERS];
    int i;
    void *p;

    puts("Test1");
    for (i = 0; i < POINTERS; i++) {
        p = my_malloc(4);
        array[i] = p;
    }
// 	printlist();
    for (i = 0; i < POINTERS; i++) {
        p = array[i];
        my_free(p);
    }

    printlist();
}

void test2()
{
    void *array[POINTERS];
    int i;
    void *p;

    puts("Test2");
    for (i = 0; i < POINTERS; i++) {
        p = my_malloc(4);
        array[i] = p;
    }
// 	printlist();
    for (i = POINTERS - 1; i >= 0; i--) {
//    	printf("%d\n",i);
//    	printlist();
        p = array[i];
        my_free(p);
    }

    printlist();
}

void test3()
{
    void *array[POINTERS];
    int i;
    void *p;

    puts("Test3");
    for (i = 0; i < POINTERS; i++) {
        p = my_malloc(4);
        array[i] = p;
    }
// printlist();
    for (i = 0; i < POINTERS; i += 2) {
        p = array[i];
        my_free(p);
//        printf("test3 free %d\n",i);
//        printlist();
    }
// printlist();
    for (i = 1; i < POINTERS; i += 2) {
        p = array[i];
        my_free(p);
    }

    printlist();
}

int main()
{
    my_malloc_init();
    test();
    /*test1();
    test2();
    test3();
    puts("Finished");*/
    return 0;
}
