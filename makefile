all: dining philo

dining: dining.c sem.c
	gcc -o dining dining.c sem.c -pthread

philo: philo.c sem.c
	gcc -o philo philo.c sem.c -pthread

clean:
	rm master barrier dining philo semap*