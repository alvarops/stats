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
int array[ARRAY_SIZE];
result tr[NUM_THREADS];

int main() {
	int i;
	clock_t tic = clock();

	printf("Start");

	build(array, ARRAY_SIZE);
//	for (i = 0; i < ARRAY_SIZE; i++) {
//		printf("\nArray %d, %d", i, array[i]);
//	}
	result r = stats(array, ARRAY_SIZE);

	printf("\nAvg = %d\n", r.avg);
	printf("\nMax = %d\n", r.max);
	printf("\nMin = %d\n", r.min);

	clock_t toc = clock();

	printf("Static: Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);

	return 0;
}

result stats(int *array, int size) {
	result r = {0, 0, 100};
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
		a[i] = rand()%100;
	}
}

