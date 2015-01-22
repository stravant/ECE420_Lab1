
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "lab1IO"
#include "sys/time.h"
#include "timer.h"
#include <math.h>

#define SZ 44100

/* Matrices and sizes. Calculating C = A*B, where A,B,C are of size NxN */
int N;
int A[SZ], B[SZ], C[SZ];

/* Thread info, an array of threads that we use for the calculation */
pthread_t *thread_array;
int thread_count;

/* Single threaded implementation */
void multiply_single() {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			int sum = 0;
			for (int k = 0; k < N; ++k) {
				sum += A[i*N + k] * B[k*N + j];
			}
			C[i*N + j] = sum;
		}
	}
}

/* Single worker task for multi threaded implemenation */
void *multiply_block(void *rank_p) {
	int rank = (int)rank_p;
	int root_p = sqrt(thread_count);
	int blk_sz = N / root_p;

	/* Tile we are calculating */
	int x = rank / root_p;
	int y = rank % root_p;

	/* Calculate */
	for (int i = y*blk_sz; i < (y+1)*blk_sz; ++i) {
		for (int j = x*blk_sz; j < (x+1)*blk_sz; ++j) {
			int sum = 0;
			for (int k = 0; k < N; ++k) {
				sum += A[i*N + k] * B[k*N + j];
			}
			C[i*N + j] = sum;			
		}
	}

	/* Done */
	pthread_exit(NULL);
}

/* Multi threaded implementation */
void multiply_multi() {
	thread_array = malloc(sizeof(pthread_t)*thread_count);
	for (int i = 0; i < thread_count; ++i) {
		pthread_create(&thread_array[i], NULL, multiply_block, (void*)i);
	}
	for (int i = 0; i < thread_count; ++i) {
		pthread_join(thread_array[i], NULL);
	}
	free(thread_array);
}

int main(int argc, char *argv[]) {
	/* What mode? */
	if (argc == 1) {
		thread_count = 1;
	} else if (argc == 2) {
		if (sscanf(argv[1], "%d", &thread_count) < 1) {
			printf("Bad thread count: `%s'\n", argv[1]);
			return EXIT_FAILURE;
		}
	}

	/* Load in the input */
	if (Lab1_loadinput(A, B, &N)) {
		return EXIT_FAILURE;
	}

	/* Check that the dimensions work */
	{
		int count_root = sqrt(thread_count);
		int count_is_square = (count_root*count_root == thread_count);
		if (!count_is_square || (N % count_root) != 0) {
			printf("Dimension (%d) / Thread Count (%d) mismatch.\n",
				N, thread_count);
			return EXIT_FAILURE;
		}
	}

	/* Start time */
	double start, end;
	GET_TIME(start);

	if (thread_count == 1) {
		multiply_single();
	} else {
		multiply_multi();
	}

	/* End time */
	GET_TIME(end);

	/* Print perf */
	printf("Calculation took: %fs\n", (end - start));
	
	/* Save output */
	Lab1_saveoutput(C, &N);

	return EXIT_SUCCESS;
}