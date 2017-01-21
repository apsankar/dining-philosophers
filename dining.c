#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "sem.h"

//Arbitrator solution to the Dining Philosophers problem
int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: ./dining <Num of philosopher processes> <Num of iterations for each philosopher>\n");
		exit(1);
	}
	if (argv[1] <= 0 || argv[2] <= 0) {
		printf("Specify a positive integer for number of philosophers and number of iterations.\n");
		exit(1);
	}
	pid_t pid;
  int status;
	int numProcesses = atoi(argv[1]);
	int numIterations = atoi(argv[2]);
	//create N semaphores, arbitrator and barrier.
	semaphore_t *semap[numProcesses];
	semaphore_t *master;
	semaphore_t *barrier;
	int i;
	barrier = semaphore_create("barrier");
	 if (barrier == NULL) {
	 	printf("Barrier couldn't be created! Exiting...\n");
		exit(1);
	}
  barrier->count = numProcesses;
	master = semaphore_create("master");
 	if (master == NULL) {
	 	printf("Arbitrator couldn't be created! Exiting...\n");
		exit(1);
	}
	master->count = 1;
	for (i=0; i<numProcesses; i++) {
		char buf[12] = {};
		snprintf(buf,sizeof(buf),"semap%d",i);
	 	semap[i] = semaphore_create(buf);
	 	if (semap[i] == NULL) {
	 		printf("Semaphore couldn't be created! Exiting...\n");
	 		exit(1);
	 	}
	 	semap[i]->count = 1;
   }  //broken into 2 for loops, to eliminate a possibility of race!! Philo cannot see the (i+1)th fork's count updated.
  for (i=0; i<numProcesses; i++) {   
    char _i[7] = {};
	 	//forks and execs N philosopher processes.
	 	switch (pid = fork()) { 
      case 0:
        sprintf(_i,"%d",i);
        execlp("./philo","philo",argv[1],argv[2],_i,NULL);
        perror("execlp"); 
        exit(1);
	 		case -1:
	 			perror("fork");
	 			exit(1);
      default:
        break;
	 	}
	}
   for (i=0; i<numProcesses; i++) 
    while ((pid = wait(&status)) != -1);
  unlink("barrier");
  unlink("master");
  for (i=0; i<numProcesses; i++) {
    char buf[12] = {};
    snprintf(buf,sizeof(buf),"semap%d",i);
    unlink(buf);
  }
}
