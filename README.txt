-------------------------------------------------------
ARBITRATOR SOLUTION TO THE DINING PHILOSOPHER'S PROBLEM
-------------------------------------------------------
Reference: https://computing.llnl.gov/tutorials/pthreads/man/pthread_mutexattr_init.txt

Usage:

a)make clean
Removes any existing semaphores in the current working directory. Also removes
the binaries 'dining' and 'philo'.

b)make:
Compiles dining.c and philo.c into their binaries. Depends on sem.c.

c) ./dining <number of philo processes> <number of iterations for each philo>

Example: ./dining 4 2
4 Philosopher processes are spawned, each HUNGRY-EAT-THINKs twice.

Semaphores are deleted from disk by the parent program, after each run.
-----
SEM.c
-----
This contains the definitions for the semaphore functions we will use, such as
creation, opening, wait, post and barrier.

--------
DINING.c
--------
It creates and initializes a master, barrier, and N 'fork' semaphores, where
N is the number of philosophers. Master and fork semaphores are binary; the
barrier's count is the number of processes. a semaphore_barrier() function
was added in sem.h and sem.c to facilitate this.

Then, dining.c which is the parent process, forks and execs N philosopher 
processes. It waits until all philosopher processes have exited before exiting.
Finally, it unlinks the master, barrier and fork semaphores.

-------
PHILO.c
-------
The philosophers wait at a barrier until all philosopher processes have been
created and are also at the barrier. After which, each philosopher enters a 
HUNGRY state. In this state, the philosopher ask's the 'arbitrator' by grabbing
the 'master' semaphore. If successful, the i-th philosopher attempts to grab
the i-th and (i+1)%N-th fork. If it grabs both forks, it releases the master 
mutex and EATS for 100 us (represented by usleep). 

Then, it drops the 2 forks in any order, without need for the master semaphore.
It THINKS for 100 us before going back to HUNGRY state. Therefore, requests to 
the master are serialized. If a philosopher grabs the master, it will release it
only after it grabs both forks. This may result in reduced parallelism, but it
eliminates starvation.  