#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/param.h>
#include <pthread.h>
#include <iostream>

#define DATA_TYPE long
#define LENGTH_LIMIT 1000000
#define SWITCH true

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

//Tridici algoritmus BubbleSort
void bubbleSort(DATA_TYPE *data,int from,int length)
{

    DATA_TYPE x;
    for(int i = from; i < length; i++)
    {
        for(int j = from; j < length; j++)
        {
            if(data[j] > data[j+1])
            {
                x = data[j+1];
                data[j+1] = data[j];
                data[j] = x;
            }
        }
    }
}

void insertionSort(DATA_TYPE *data, int from, int length,bool asc)
{
/*
    if(from >= length)
    {
        printf("Invalid input!");
        return;
    }
*/

    int i, j;  
    DATA_TYPE key;
    if(asc)
    {
        for (i = from; i < length+from; i++) 
        {  
            key = data[i];  
            j = i - 1;  

            while (j >= from && data[j] > key) 
            {  
                data[j + 1] = data[j];  
                j = j - 1;  
            }  
            data[j + 1] = key;  
        }
    }
    else
    {
        for (i = from; i < length+from; i++) 
        {  
            key = data[i];  
            j = i - 1;  

            while (j >= from && data[j] < key) 
            {  
                data[j + 1] = data[j];  
                j = j - 1;  
            }  
            data[j + 1] = key;  
        }
    }
    

}

void selectionSort(DATA_TYPE *data, int from, int length)
{
/*
    if(from >= length)
    {
        printf("Invalid input!");
        return;
    }
*/
    int i, j,min_idx;
    DATA_TYPE key;
    
    // One by one move boundary of unsorted subarray  
    for (i = from; i < length; i++)  
    {  
        // Find the minimum element in unsorted array  
        min_idx = i;  
        for (j = i+1; j < length; j++)
        {
            if (data[j] < data[min_idx])
            {
                min_idx = j;
                key = data[min_idx];
                data[min_idx] = data[i];
                data[i] = key;
            }  

        }
    }  

}

DATA_TYPE * mergeArrays(DATA_TYPE *data1, int length1, DATA_TYPE *data2, int length2)
{
    int lengthRes = length1 + length2;
    DATA_TYPE *arrayRes = new DATA_TYPE[lengthRes];
    int i = 0;
    int j = 0;
    int k = 0;

    while(i < length1 && j < length2)
    {
        if(data1[i] < data2[j])
        {
            arrayRes[k] = data1[i];
            i++;
            k++;
        }
        else
        {
            arrayRes[k] = data2[i];
        }
    }
    while(i < length1)
    {
        arrayRes[k++] = data1[i++];        
    }
    while(j < length2)
    {   
        arrayRes[k++] = data2[j++];        
    }
    return arrayRes;
}

void printArray(DATA_TYPE *data, int from, int length)
{
    if(from >= length)
    {
        return;
    }
    std::cout<<std::endl;
    for(int i = from; i < length; i++)
    {
        std::cout<<i<<" "<<data[i]<<std::endl;
    }
}

//MAIN
int main(int na, char **arg)
{
    //inicialization of values for time measurement
    timeval time_before,time_after;
    int length1 = 8;
    int length2 = 12;


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

    DATA_TYPE *array1 = new DATA_TYPE[length1];
    DATA_TYPE *array2 = new DATA_TYPE[length2];

    if(!array1 || !array2)
    {
        printf("Not enought memory for array!\n");  
    }
    

    //initialization of random number generator
    srand((DATA_TYPE) time(NULL));
    printf("\nRandom numbers generation started...\n");
    for(int i = 0; i < my_length; i++)
    {
        my_array[i] = rand() % (my_length*10);
        
        //Vypisuje tečky dokud se nevygeneruje celé pole ...         
        if(!(i%LENGTH_LIMIT))
        {
            printf(".");
            fflush(stdout);
        }
    }
    for(int i = 0; i < length1; i++)
    {
        array1[i] = rand() % (my_length*10);
    }
    for(int i = 0; i < length2; i++)
    {
        array2[i] = rand() % (my_length*10);
    }


    printArray(array2,0,length2);
    std::cout<<"-----------------------"<<std::endl;

    /*
    insertionSort(array2,0,length2/2,SWITCH);
    insertionSort(array2,length2/2,length2/2,SWITCH);
    printArray(array2,0,length2);
    std::cout<<"-----------------------"<<std::endl;    

    insertionSort(array1,0,length1,SWITCH);
    printArray(array1,0,length1);
    std::cout<<"-----------------------"<<std::endl;

    //DATA_TYPE *array12 = mergeArrays(array1,length1,array2,length2);
    //printArray(array12,0,length1+length2);
    */
   
    insertionSort(array2,0,length2/6,SWITCH);
    insertionSort(array2,length2/6,length2/6,SWITCH);
    insertionSort(array2,(length2/6)*2,length2/6,SWITCH);
    insertionSort(array2,(length2/6)*3,length2/6,SWITCH);
    insertionSort(array2,(length2/6)*4,length2/6,SWITCH);
    insertionSort(array2,(length2/6)*5,length2/6,SWITCH);
    
    printArray(array2,0,length2);
    
    /*
    int block_length = length2/6;
    //--
    DATA_TYPE *block1 = new DATA_TYPE[block_length];
    block1[0] = array2[0];
    block1[1] = array2[1];
    //--
    DATA_TYPE *block2 = new DATA_TYPE[block_length];
    block2[0] = array2[2];
    block2[1] = array2[3];
    //--
    DATA_TYPE *block3 = new DATA_TYPE[block_length];
    block3[0] = array2[4];
    block3[1] = array2[5];
    //--
    DATA_TYPE *block4 = new DATA_TYPE[block_length];
    block4[0] = array2[6];
    block4[1] = array2[7];
    //--
    DATA_TYPE *block5 = new DATA_TYPE[block_length];
    block5[0] = array2[8];
    block5[1] = array2[9];
    //--
    DATA_TYPE *block6 = new DATA_TYPE[block_length];
    block6[0] = array2[10];
    block6[1] = array2[11];

    
    DATA_TYPE *block12 = mergeArrays(block1,block_length,block2,block_length);
    std::cout<<"-----------------------"<<std::endl;
    printArray(block12,0,block_length*2);

    */
    
    

    printf("-------------------VLAKNA--------------------------------");

    //Sorting using bubblesort
    /*
    printf("Sorting using bubblesort started...\n");
    gettimeofday(&time_before,NULL);
    bubbleSort(my_array,0,my_length);
    gettimeofday(&time_after,NULL);
    printf("The sorting time: %d [ms]\n",timeval_to_ms(&time_before,&time_after));
    */

    //Sorting using isertionsort
    /*
    printf("Sorting using insertionsort started...\n");
    gettimeofday(&time_before,NULL);
    insertionSort(my_array,0,my_length);
    gettimeofday(&time_after,NULL);
    printf("The sorting time: %d [ms]\n",timeval_to_ms(&time_before,&time_after));
    */

    //Sorting using selectionsort
    /*
    printf("Sorting using selectionsort started...\n");
    gettimeofday(&time_before,NULL);
    insertionSort(my_array,0,my_length);
    gettimeofday(&time_after,NULL);
    printf("The sorting time: %d [ms]\n",timeval_to_ms(&time_before,&time_after));
    */

    //ALGORITHM USING TWO THREADS
    printf("\nMaximum number search using two threads...\n");
    pthread_t pt1,pt2;
    task_part tp1(1,0,my_length/2,my_array);
    task_part tp2(2,my_length/2,my_length-my_length/2, my_array);

    
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















