/*
 * statistics-threads.c
 *
 *  Created on: Apr 29, 2013
 *      Author: alvaroperedasancho
 */
#include "statistics-threads.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

int array[ARRAY_SIZE];
result tr[NUM_THREADS];
int avgs[NUM_THREADS];
int maxs[NUM_THREADS];
int mins[NUM_THREADS];

int main() {
	int i;
	clock_t tic = clock();

	printf("Start");

	build(array, ARRAY_SIZE);
	for (i = 0; i < ARRAY_SIZE; i++) {
		//printf("\nArray %d, %d", i, array[i]);
	}
	result r = stats(array, ARRAY_SIZE);

	printf("\nAvg = %d\n", r.avg);
	printf("\nMax = %d\n", r.max);
	printf("\nMin = %d\n", r.min);

	clock_t toc = clock();

	printf("Static: Elapsed: %f seconds\n",
			(double) (toc - tic) / CLOCKS_PER_SEC);

	tic = clock();
	thread_starter();
	toc = clock();
	printf("dynamic: Elapsed: %f seconds\n",
			(double) (toc - tic) / CLOCKS_PER_SEC);

	/* Last thing that main() should do */
	pthread_exit(NULL );

	return 0;
}

result stats(int *array, int size) {
	result r = { 0, 0, 100 };
	int i;
	for (i = 0; i < size; i++) {
		if (r.max < array[i]) {
			r.max = array[i];
		}
		if (r.min > array[i]) {
			r.min = array[i];
		}
		r.avg += array[i];
	}

	r.avg /= size;

	return r;
}

void build(int *a, int size) {
	int i;

	for (i = 0; i < size; i++) {
		a[i] = rand() % 100;
	}
}

int thread_starter() {
	pthread_t threads[NUM_THREADS];
	void * status;

	for (int t = 0; t < NUM_THREADS; t++) {
		//printf("hello world! from main(). Creating thread %i\n", t);
		int rc = pthread_create(&threads[t], NULL, pt_build, (void *) t);
	}

	for (int j = 0; j < NUM_THREADS; j++) {
		pthread_join(threads[j], &status);
	}

	for (int i = 0; i < ARRAY_SIZE; i++) {
		//printf("\n Array built %d", array[i]);
	}

	return 0;
}

void *pt_build(void *threadid) {
	int iter = ARRAY_SIZE / NUM_THREADS;
	int id = (int) threadid;
	//printf("hello world! From thread %i\n", id);
	for (int i = 0; i < iter; i++) {
		array[id * iter + i] = rand() % 100;
	}
	pthread_exit(threadid);
	return threadid;
}

