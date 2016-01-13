#include <pthread.h> 
#include <stdio.h> 
#include <sys/time.h> 
#include <string.h> 
#define MAX 10 

pthread_t thread[2]; 
pthread_mutex_t mut; 
int number=0, i; 

void *thread1() 
{ 
        printf ("thread1 : I'm thread 1\n"); 

        for (i = 0; i < MAX; i++) 
        { 
                printf("thread1 : number = %d\n",number); 
                pthread_mutex_lock(&mut); 
                        number++; 
                pthread_mutex_unlock(&mut); 
                sleep(2); 
        } 


        printf("thread1 : Is the main function waiting?\n"); 
        pthread_exit(NULL); 
} 

void *thread2() 
{ 
        printf("thread2 : I'm thread 2.\n"); 

        for (i = 0; i < MAX; i++) 
        { 
                printf("thread2 : number = %d\n",number); 
                pthread_mutex_lock(&mut); 
                        number++; 
                pthread_mutex_unlock(&mut); 
                sleep(3); 
        } 


        printf("thread2 :Is the main function waiting？\n"); 
        pthread_exit(NULL); 
} 

void thread_create(void) 
{ 
        int temp; 
        memset(&thread, 0, sizeof(thread));          
        
        if((temp = pthread_create(&thread[0], NULL, thread1, NULL)) != 0)       //comment2 
                printf("Failed to create thread 1\n"); 
        else 
                printf("thread 1 has been created\n"); 

        if((temp = pthread_create(&thread[1], NULL, thread2, NULL)) != 0)  //comment3 
                printf("Failed to create thread 2"); 
        else 
                printf("thread 2 has been created\n"); 
} 

void thread_wait(void) 
{ 
        if(thread[0] !=0) {                  
                pthread_join(thread[0],NULL); 
                printf("thread 1 is finished\n"); 
        } 
        if(thread[1] !=0) {                
                pthread_join(thread[1],NULL); 
                printf("thread 2 is finished\n"); 
        } 
} 

int main() 
{ 

        pthread_mutex_init(&mut,NULL); 

        printf("I am the main function creating threads...\n"); 
        thread_create(); 
        printf("I am the main function waiting for the threads to finish...\n"); 
        thread_wait(); 

        return 0; 
} 

