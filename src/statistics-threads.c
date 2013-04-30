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
float avgs[NUM_THREADS];
int maxs[NUM_THREADS];
int mins[NUM_THREADS];

int main() {
	int i;
	clock_t tic = clock();

	printf("Start\n");

	build(array, ARRAY_SIZE);
//	for (i = 0; i < ARRAY_SIZE; i++) {
//		printf("\nArray %d, %d", i, array[i]);
//	}
	clock_t toc = clock();
	printf("static build: Elapsed: %f seconds\n",
			(double) (toc - tic) / CLOCKS_PER_SEC);
	tic = clock();

	result r = stats(array, ARRAY_SIZE);

	printf("Avg = %.4f\n", r.avg);
	printf("Max = %d\n", r.max);
	printf("Min = %d\n", r.min);

	toc = clock();

	printf("Static stats: Elapsed: %f seconds\n",
			(double) (toc - tic) / CLOCKS_PER_SEC);

	/*tic = clock();
	thread_starter(pt_build);
	toc = clock();
	printf("dynamic build: Elapsed: %f seconds\n",
			(double) (toc - tic) / CLOCKS_PER_SEC);
*/
	tic = clock();
	thread_starter(pt_stats);
	r = pt_summarize();
	printf("Avg = %.4f\n", r.avg);
	printf("Max = %d\n", r.max);
	printf("Min = %d\n", r.min);

	toc = clock();
	printf("dynamic stats: Elapsed: %f seconds\n",
			(double) (toc - tic) / CLOCKS_PER_SEC);

	/* Last thing that main() should do */
	pthread_exit(NULL );

	return 0;
}

result stats(int *array, int size) {
	result r = { 0.0, 0, 1000 };
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
		a[i] = rand() % 1000;
	}
}

int thread_starter(void *exe) {
	pthread_t threads[NUM_THREADS];
	void * status;

	for (int t = 0; t < NUM_THREADS; t++) {
		int rc = pthread_create(&threads[t], NULL, exe, (void *) t);
	}

	for (int j = 0; j < NUM_THREADS; j++) {
		pthread_join(threads[j], &status);
	}

//	for (int i = 0; i < ARRAY_SIZE; i++) {
//		printf("\n Array built %d", array[i]);
//	}

	return 0;
}

void *pt_build(void *threadid) {
	int iter = ARRAY_SIZE / NUM_THREADS;
	int id = (int) threadid;
	for (int i = 0; i < iter; i++) {
		array[id * iter + i] = rand() % 1000;
	}
	pthread_exit(threadid);
	return threadid;
}

void *pt_stats(void *threadid) {
	int iter = ARRAY_SIZE / NUM_THREADS;
	int id = (int) threadid;
	result r = stats(&array[id * iter], iter);
	avgs[id] = r.avg;
	maxs[id] = r.max;
	mins[id] = r.min;

	pthread_exit(threadid);
	return threadid;
}

result pt_summarize() {
	result r = { 0, 0, 100 };
	int i;
	for (i = 0; i < NUM_THREADS; i++) {
		if (r.max < maxs[i]) {
			r.max = maxs[i];
		}
		if (r.min > mins[i]) {
			r.min = mins[i];
		}
		r.avg += avgs[i];
	}

	r.avg /= NUM_THREADS;

	return r;
}

