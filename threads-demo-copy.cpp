#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/param.h>
#include <pthread.h>

#define DATA_TYPE int
#define LENGTH_LIMIT 10000000

class task_part{
    public:
        int id;
        int from;
        int length;
        DATA_TYPE *data;
        DATA_TYPE max;

        task_part(int myId, int first, int num, DATA_TYPE *ptr) : 
            id(myId), from(first), length(num), data(ptr){}

        DATA_TYPE get_result() //return max num in array
        {
            return max;
        }

        DATA_TYPE search_max() //find max num in array
        {
            DATA_TYPE max_element = data[from];
            for(int i = 1; i < length; i++)
            {
                if(max_element < data[from+i])
                {
                    max_element = data[from+i];                    
                }
            }
            return max_element;
        }
};


//Funkce pro vlánka vytvočí objekt třídy task_part a předá ji argumenty
//Poté vypíše informace o objektu
//Pak zavolá funkci search_max pro nalezení nejvetsiho cisla v poli
//Poté jej vypíše
void *my_thread(void *void_arg)
{
    task_part *ptr_task = (task_part *) void_arg;

    printf("THREAD %d started from %d with length %d...\n",
        ptr_task->id,ptr_task->from,ptr_task->length);
    
    ptr_task->max = ptr_task->search_max();

    printf( "Found maximum in thread %d is %d\n", ptr_task->id, ptr_task->max);

    return NULL;
}

//Funkce počítající rozdíl v ms mezi dvěma časy
int timeval_to_ms(timeval *befor, timeval *after)
{
    timeval res;
    timersub(after,befor,&res);
    return 1000*res.tv_sec + res.tv_usec/1000;
}

//MAIN
int main(int na, char **arg)
{
    if(na != 2)
    {
        printf("Specify number of elements, at least %d.\n", LENGTH_LIMIT);
    }

    int my_length = atoi(arg[1]);
    if(my_length < LENGTH_LIMIT)
    {
        printf("The number of elements must be at least %d.\n", LENGTH_LIMIT);
        return 0;
    }

    //array allocation
    DATA_TYPE *my_array = new DATA_TYPE[my_length];
    if(!my_array)
    {
        printf("Not enought memory for array!\n");
        return 1;
    }

    //initialization of random number generator
    srand((int) time(NULL));
    printf("\nRandom numbers generation started...\n");
    for(int i = 0; i < my_length; i++)
    {
        my_array[i] = rand() % (my_length*10);
        
        //Vypisuje tečky dokud se nevygeneruje celé pole ... 
        /*
        if(!(i%LENGTH_LIMIT))
        {
            printf(".");
            fflush(stdout);
        }
        */
    }



    //ALGORITHM USING TWO THREADS
    printf("\nMaximum number search using two threads...\n");
    pthread_t pt1,pt2;
    task_part tp1(1,0,my_length/2,my_array);
    task_part tp2(2,my_length/2,my_length-my_length/2, my_array);

    timeval time_before,time_after;
    gettimeofday(&time_before,NULL);

    pthread_create(&pt1,NULL,my_thread,&tp1);
    pthread_create(&pt2,NULL,my_thread,&tp2);

    pthread_join(pt1, NULL);
    pthread_join(pt2, NULL);

    gettimeofday(&time_after,NULL);
    
    printf("The found maximum: %d\n", MAX(tp1.get_result(),tp2.get_result()));
    printf("The search time: %d [ms]\n", timeval_to_ms(&time_before, &time_after));



    //ALGORITHM USING ONE THREAD
    printf("\nMaximum number search using one thread...\n");

    gettimeofday(&time_before,NULL);

    task_part single(333,0,my_length,my_array);
    DATA_TYPE res = single.search_max();
    
    gettimeofday(&time_after,NULL);

    printf("The found maximum: %d\n", res);
    printf("The search time: %d [ms]\n", timeval_to_ms(&time_before, &time_after));
    
    
    //ALGORITHM USING 4 THREADS
    printf("\nMaximum number search using four threads...\n");
    pthread_t pt3,pt4,pt5,pt6;
    task_part tp3(3,0,my_length/4,my_array);
    task_part tp4(4,my_length/4,my_length/4,my_array);
    task_part tp5(5,my_length/2,my_length/4,my_array);
    task_part tp6(6,(my_length/4)*3,my_length/4,my_array);

    gettimeofday(&time_before,NULL);

    pthread_create(&pt3,NULL,my_thread,&tp3);
    pthread_create(&pt4,NULL,my_thread,&tp4);
    pthread_create(&pt5,NULL,my_thread,&tp5);
    pthread_create(&pt6,NULL,my_thread,&tp6);

    pthread_join(pt3,NULL);
    pthread_join(pt4,NULL);
    pthread_join(pt5,NULL);
    pthread_join(pt6,NULL);

    gettimeofday(&time_after,NULL);

    printf("The found maximum: %d\n", MAX(MAX(tp3.get_result(),tp4.get_result()),MAX(tp5.get_result(),tp6.get_result())));
    printf("The search time: %d [ms]\n", timeval_to_ms(&time_before, &time_after));
    

}















