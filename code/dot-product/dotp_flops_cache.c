#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000 // Fit the arrays in 1MB cache considering single precision floats
#define REPETITIONS 1000
#define BILLION 1000000000.0

float a[SIZE], b[SIZE];

void initialize_arrays() {
    for (int i = 0; i < SIZE; i++) {
        a[i] = (float)rand() / RAND_MAX * 2.0f - 1.0f; // Range [-1, 1]
        b[i] = (float)rand() / RAND_MAX * 2.0f - 1.0f; // Range [-1, 1]
    }
}

float dot_product() {
    float dot = 0.0f;
    for (int i = 0; i < SIZE; i++) {
        dot += a[i] * b[i];
    }
    return dot;
}

int main() {
    struct timespec start, end;
    float dot;
    double time_spent, gflops;

    srand((unsigned int)time(NULL));
    initialize_arrays();

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < REPETITIONS; i++) {
        dot = dot_product();
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;
    gflops = (2.0 * SIZE * REPETITIONS) / (time_spent * 1e9); // 2 FLOPs (mul and add) per iteration

    printf("Last dot product result: %f\n", dot);
    printf("Time spent: %.8f seconds\n", time_spent);
    printf("Performance: %.8f GFLOPS\n", gflops);

    return 0;
}
