#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <pthread.h>

#define INIT_BALANCE 50
#define NUM_TRANS 100

int balance = INIT_BALANCE;

int credits = 0;
int debits = 0;

pthread_mutex_t b_lock,c_lock,d_lock;

void * transactions(void * args) {
	int i,v;

	for (i=0; i<NUM_TRANS; i++) {
		//choose a random value
		srand(time(NULL));
		v = rand() % NUM_TRANS;

		//randomnly choose to credit or debit
		if (rand() % 2) {
			//credit
			pthread_mutex_lock(&b_lock);
			balance = balance + v;
			pthread_mutex_unlock(&b_lock);

			pthread_mutex_lock(&c_lock);
			credits = credits + v;
			pthread_mutex_unlock(&c_lock);
		} else {
			//debit
			pthread_mutex_lock(&b_lock);
			balance = balance - v;
			pthread_mutex_unlock(&b_lock);

			pthread_mutex_lock(&d_lock);
			debits = debits + v;
			pthread_mutex_unlock(&d_lock);
		}
	}
	return 0;
}

int main(int argc, char * argv[]) {
	//time start
	struct timeval start, end;
	gettimeofday(&start, NULL);

	int n_threads,i;
	pthread_t * threads;
	pthread_mutex_init(&b_lock, NULL);
	pthread_mutex_init(&c_lock, NULL);
	pthread_mutex_init(&d_lock, NULL);

	//error check
	if (argc < 2) {
		fprintf(stderr, "ERROR: Require number of threads\n");
		exit(1);
	}

	//convert string to int
	n_threads = atol(argv[1]);

	//error check
	if (n_threads <= 0) {
		fprintf(stderr, "ERROR: Invalivd value for number of threads\n");
		exit(1);
	}

	//allocate array of thread identifiers
	threads = calloc(n_threads, sizeof(pthread_t));

	//start all threads
	for (i=0; i<n_threads; i++) {
		pthread_create(&threads[i], NULL, transactions, NULL);
	}

	//wait for all threads finish its jobs
	for (i=0; i<n_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("\tCredits:\t%d\n", credits);
	printf("\t Debits:\t%d\n\n", debits);
	printf("%d + %d - %d = \t%d\n", INIT_BALANCE,credits,debits,INIT_BALANCE+credits-debits);
	printf("\t Balance:\t%d\n", balance);

	//free array
	free(threads);
	pthread_mutex_destroy(&b_lock);
	pthread_mutex_destroy(&c_lock);
	pthread_mutex_destroy(&d_lock);

	//time end
	gettimeofday(&end, NULL);
	printf("\n Time:\t%lf (s)\n", (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0);

	return 0;
}