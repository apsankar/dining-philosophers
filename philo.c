#include <stdio.h>
#include <stdlib.h>
#include "sem.h"

//Each philosopher eats for 100 us, thinks for 100us.
int main(int argc, char *argv[]) {
  int numProcesses = atoi(argv[1]);
  int numIterations = atoi(argv[2]);
  int i = atoi(argv[3]);
  int j;
  semaphore_t *master;
  semaphore_t *barrier;
  semaphore_t *mutex;
  semaphore_t *semap[2];
  master = semaphore_open("master");
  barrier = semaphore_open("barrier");
  int _temp = i;
  for(j=0; j<2; j++) {
    char buf[50];
    snprintf(buf,sizeof(buf),"semap%d",_temp);
    semap[j] = semaphore_open(buf);
    if (semap[j] == NULL) {
      printf("Semaphore in philo not opening! Exiting...\n");
      exit(1);
    }
    _temp = (_temp+1)%numProcesses;
  }

  //printf("Philosopher %d at barrier\n",i);
  semaphore_barrier(barrier); //see internal implementation in sem.c
  //All philosophers are now at the table. i'th philo acquires i'th and (i+1)%N'th forks after grabbing master mutex.
  for (j=0; j< numIterations; j++) {
  printf("Philosopher %d is HUNGRY for %d'th time.\n",i+1,j+1);

  semaphore_wait(master);
  semaphore_wait(semap[0]);
  semaphore_wait(semap[1]); 
  semaphore_post(master);

  printf("Philosopher %d is EATING for %d'th time.\n",i+1,j+1);
  usleep(100);
  //mutex not required for dropping forks.
  semaphore_post(semap[0]);
  semaphore_post(semap[1]);

  printf("Philosopher %d is THINKING for %d'th time.\n",i+1,j+1);
  usleep(100);
  }
  exit(0);
}