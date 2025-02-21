#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_THREADS 8
#define MAX_IN_LINE 3

int in_bathroom = 0;
int in_line = 0;
int first = 1;
sem_t f_occupies;
sem_t m_occupies;
sem_t read_write;

void *Worker(void* args){
    long id = (long)args;

    if(id % 2 == 0){
        //declare variables and announce ID
        printf("I am male with id: %ld \n", id);
        int work_time;
        int bathroom_time;
        
        //start the loop
        while (1)
        {
            //Employee is working
            work_time = rand() % 3 + 3;
            sleep(work_time);
        
        
        //enter line
        sem_wait(&read_write);
        in_line++;
        sem_post(&read_write);

        //wait until no women are in the bathroom
        sem_wait(&f_occupies);  //locks for other men auto

        //if first man in, close for women
        if(in_bathroom == 0){
            sem_wait(&m_occupies);
        }
        
        //leave line and unlock for other men if not max in line
        sem_wait(&read_write);
        in_line--;
        if(in_line <= MAX_IN_LINE){
            sem_post(&f_occupies);
        }

        //enter bathroom and print stats
        in_bathroom++;
        printf("Male %ld is using the bathroom\n", id);
        printf("In bathroom %d people\nIn line %d\n", in_bathroom, in_line);
        sem_post(&read_write);

        //use bathroom and leaves
        bathroom_time = rand() % 2 + 1;
        sleep(bathroom_time);
        printf("Male %ld is leaving\n", id);
        sem_wait(&read_write);
        in_bathroom--;
        printf("Bathroom is used by %d people\n", in_bathroom);

        //if last male, unlock for women
        if(in_bathroom == 0){
            sem_post(&m_occupies);
        }
        sem_post(&read_write);

        }

    }
    else {
        printf("I am female with id: %ld \n", id);
        int work_time;
        int bathroom_time;
        
        //start the loop
        while (1)
        {
            //Employee is working
            work_time = rand() % 3 + 3;
            sleep(work_time);
        
        
        //enter line
        sem_wait(&read_write);
        in_line++;
        sem_post(&read_write);

        //wait until no women are in the bathroom
        sem_wait(&m_occupies);  //locks for other men auto

        //if first man in, close for women
        if(in_bathroom == 0){
            sem_wait(&f_occupies);
        }
        
        //leave line and unlock for other men if not max in line
        sem_wait(&read_write);
        in_line--;
        if(in_line <= MAX_IN_LINE){
            sem_post(&m_occupies);
        }

        //enter bathroom and print stats
        in_bathroom++;
        printf("Female %ld is using the bathroom\n", id);
        printf("In bathroom %d people\nIn line %d\n", in_bathroom, in_line);
        sem_post(&read_write);

        //use bathroom and leaves
        bathroom_time = rand() % 2 + 1;
        sleep(bathroom_time);
        printf("Female %ld is leaving\n", id);
        sem_wait(&read_write);
        in_bathroom--;
        printf("Bathroom is used by %d people\n", in_bathroom);

        //if last male, unlock for women
        if(in_bathroom == 0){
            sem_post(&f_occupies);
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
