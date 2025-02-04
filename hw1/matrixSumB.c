/* matrix summation using pthreads

   features: uses a barrier; the Worker[0] computes
             the total sum from partial sums computed by Workers
             and prints the total sum to the standard output

   usage under Linux:
     gcc matrixSum.c -lpthread
     a.out size numWorkers

*/
#ifndef _REENTRANT 
#define _REENTRANT 
#endif 
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 10000  /* maximum matrix size */
#define MAXWORKERS 10   /* maximum number of workers */

pthread_mutex_t barrier;  /* mutex lock for the barrier */
pthread_cond_t go;        /* condition variable for leaving */
int numWorkers;           /* number of workers */ 
int numArrived = 0;       /* number who have arrived */

/* a reusable counter barrier */
void Barrier() {
  pthread_mutex_lock(&barrier);
  numArrived++;
  if (numArrived == numWorkers) {
    numArrived = 0;
    pthread_cond_broadcast(&go);
  } else
    pthread_cond_wait(&go, &barrier);
  pthread_mutex_unlock(&barrier);
}

/* timer */
double read_timer() {
    static bool initialized = false;
    static struct timeval start;
    struct timeval end;
    if( !initialized )
    {
        gettimeofday( &start, NULL );
        initialized = true;
    }
    gettimeofday( &end, NULL );
    return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

double start_time, end_time; /* start and end times */
int size, stripSize;  /* assume size is multiple of numWorkers */
//int sums[MAXWORKERS]; /* partial sums */
/*int maxs[MAXWORKERS];
int mins[MAXWORKERS];
int max_is[MAXWORKERS];
int min_is[MAXWORKERS];
int max_js[MAXWORKERS];
int min_js[MAXWORKERS];*/
int global_min = 100;
int global_max = 0;
int global_max_i = 0;
int global_max_j = 0;
int global_min_i = 0;
int global_min_j = 0;
int global_total = 0;


int matrix[MAXSIZE][MAXSIZE]; /* matrix */

void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
  int i, j;
  long l; /* use long in case of a 64-bit system */
  pthread_attr_t attr;
  pthread_t workerid[MAXWORKERS];
  time_t t;
  srand((unsigned) time(&t));


  /* set global thread attributes */
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

  /* initialize mutex and condition variable */
  pthread_mutex_init(&barrier, NULL);
  pthread_cond_init(&go, NULL);

  /* read command line args if any */
  size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE) size = MAXSIZE;
  if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;
  stripSize = size/numWorkers;

  /* initialize the matrix */
  for (i = 0; i < size; i++) {
	  for (j = 0; j < size; j++) {
          matrix[i][j] = rand()%99;
	  }
  }

  /* print the matrix */
#ifdef DEBUG
  for (i = 0; i < size; i++) {
	  printf("[ ");
	  for (j = 0; j < size; j++) {
	    printf(" %d", matrix[i][j]);
	  }
	  printf(" ]\n");
  }
#endif

  /* do the parallel work: create the workers */
  start_time = read_timer();
  for (l = 0; l < numWorkers; l++)
    pthread_create(&workerid[l], &attr, Worker, (void *) l);
  pthread_exit(NULL);
}

/* Each worker sums the values in one strip of the matrix.
   After a barrier, worker(0) computes and prints the total */
void *Worker(void *arg) {
  long myid = (long) arg;
  int total, i, j, first, last, max, max_index_i, max_index_j, min, min_index_i, min_index_j;

#ifdef DEBUG
  printf("worker %d (pthread id %d) has started\n", myid, pthread_self());
#endif

  /* determine first and last rows of my strip */
  first = myid*stripSize;
  last = (myid == numWorkers - 1) ? (size - 1) : (first + stripSize - 1);

  /* sum values in my strip */
  max = 0;  
  min = 100;  
  max_index_i = max_index_j = min_index_i = min_index_j = -1;
 
  for (i = first; i <= last; i++)
    for (j = 0; j < size; j++){
      total += matrix[i][j];
      if (matrix[i][j] > max){
        max = matrix[i][j];
        max_index_i = i;
        max_index_j = j;}
      if (matrix[i][j] < min){
        min = matrix[i][j];
        min_index_i = i;
        min_index_j = j;}}
  if (global_max < max){
    global_max = max;
    global_max_i = max_index_i;
    global_max_j = max_index_j;
    printf("global max changed to %d \n", global_max);
  }
  if (global_min > min){
    global_min = min;
    global_min_i = min_index_i;
    global_min_j = min_index_j;
  }
  global_total += total;
  numArrived++;
  /*sums[myid] = total;
  mins[myid] = min;
  maxs[myid] = max;
  max_is[myid] = max_index_i;
  max_js[myid] = max_index_j;
  min_is[myid] = min_index_i;
  min_js[myid] = min_index_j;
*/
  //Barrier();
  if (myid == 0) {
    while(numArrived != numWorkers){

    }
    
    /*max, max_index_i, max_index_j, min_index_i, min_index_j = 0;
    for (i = 0; i < numWorkers; i++){
      total += sums[i];
      if (min > mins[i]){
        min = mins[i];
        min_index_i = min_is[i];
        min_index_j = min_js[i];}
      if (max < maxs[i]){
        max = maxs[i];
        max_index_i = max_is[i];
        max_index_j = max_js[i];}}*/
    /* get end time */
    end_time = read_timer();
    /* print results */
    printf("The total is %d\n", global_total);
    printf("Max: %d at [%d, %d]\n", global_max, global_max_i, global_max_j);
    printf("Min: %d at [%d, %d]\n", global_min, global_min_i, global_min_j);
    printf("The execution time is %g sec\n", end_time - start_time);
  }
}
