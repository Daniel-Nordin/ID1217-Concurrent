#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_THREADS 8
#define MAX_IN_LINE 4

int in_bathroom = 0;
int in_line = 0;
int first = 1;
sem_t f_occupies;
sem_t m_occupies;
sem_t read_write;

void *Worker(void* args){
    long id = (long)args;

    if(id % 2 == 0){
        printf("I am male with id: %ld \n", id);
        int work_time;
        int bathroom_time;
        
        while (1)
        {
            work_time = rand() % 3 + 3;
            sleep(work_time);
        
        

        sem_wait(&read_write);
        in_line++;
        /*if(first == 1){
            sem_post(&f_occupies);
            printf("f_unlocked");
            first = 0;
        }*/
        sem_post(&read_write);
        sem_wait(&f_occupies);
        sem_wait(&m_occupies);
        sem_wait(&read_write);
        in_line--;
        if(in_line <= MAX_IN_LINE){
            sem_post(&f_occupies);
            printf("f_unlocked");
        }
        in_bathroom++;
        printf("Male %ld is using the bathroom\n", id);
        printf("In bathroom %d people\nIn line %d\n", in_bathroom, in_line);
        sem_post(&read_write);
        bathroom_time = rand() % 2 + 1;
        sleep(bathroom_time);
        printf("Male %ld is leaving\n", id);
        sem_wait(&read_write);
        in_bathroom--;
        printf("Bathroom is used by %d people\n", in_bathroom);
        if(in_bathroom == 0){
            sem_post(&m_occupies);
            sem_post(&f_occupies);
            printf("m_unlocked");
        }
        sem_post(&read_write);

        }

    }
    else {
        printf("I am female with id: %ld \n", id);
        int work_time;
        int bathroom_time;
        
        while (1)
        {
            work_time = rand() % 3 + 3;
            sleep(work_time);
        
        

        sem_wait(&read_write);
        in_line++;
        /*if(first == 1){
            sem_post(&m_occupies);
            first = 0;
        }*/
        sem_post(&read_write);
        sem_wait(&m_occupies);
        if(in_bathroom == 0);{
            sem_wait(&f_occupies);
        }
        sem_wait(&read_write);
        in_line--;
        if(in_line <= MAX_IN_LINE){
            sem_post(&m_occupies);
        }
        in_bathroom++;
        printf("Female %ld is using the bathroom\n", id);
        printf("In bathroom %d people\nIn line %d\n", in_bathroom, in_line);
        sem_post(&read_write);
        bathroom_time = rand() % 2 + 1;
        sleep(bathroom_time);
        printf("Female %ld is leaving\n", id);
        sem_wait(&read_write);
        in_bathroom--;
        printf("Bathroom is used by %d people\n", in_bathroom);
        if(in_bathroom == 0){
            sem_post(&f_occupies);
            sem_post(&m_occupies);
        }
        sem_post(&read_write);

        }   
    }


}


int main(){
    pthread_t threads[NUM_THREADS];

    sem_init(&m_occupies, 0, 1);
    sem_init(&f_occupies, 0, 1);
    sem_init(&read_write, 0, 1);

    for(long l = 0; l < NUM_THREADS; l++){
        pthread_create(&threads[l], NULL, Worker, (void*)l);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    

    return 0;
}
