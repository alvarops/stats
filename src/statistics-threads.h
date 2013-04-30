/*
 * statistics-threads.h
 *
 *  Created on: Apr 29, 2013
 *      Author: alvaroperedasancho
 */

#ifndef STATISTICS_THREADS_H_
#define STATISTICS_THREADS_H_
#define ARRAY_SIZE		500
#define NUM_THREADS     5

typedef struct {
	float avg;
	int max;
	int min;
} result;

result stats(int *array, int size);
void build(int *array, int size);
/*
void *thread_stats(int *array);
void *thread_build(int **array);
*/
int thread_starter (void *exe);
void *pt_build(void *threadid);
void *pt_stats(void *threadid);
result pt_summarize();

#endif /* STATISTICS_THREADS_H_ */
