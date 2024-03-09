#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MATRIX_SIZE 1000

double matrix[MATRIX_SIZE][MATRIX_SIZE];
double vector[MATRIX_SIZE];
double result[MATRIX_SIZE];

void initialize() {
    // Initialize matrix and vector with random values
    srand(time(NULL));
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] = (double)rand() / RAND_MAX;
        }
        vector[i] = (double)rand() / RAND_MAX;
    }
}

void matrix_vector_multiply() {
    // Perform matrix-vector multiplication
    for (int i = 0; i < MATRIX_SIZE; i++) {
        result[i] = 0.0;
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
}

int main() {
    initialize();

    clock_t start_time = clock();
    matrix_vector_multiply();
    clock_t end_time = clock();
    double elapsed_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    // Print time taken
    printf("Time taken: %f seconds\n", elapsed_time);

    // Calculate total floating point operations
    long long flops = 2 * MATRIX_SIZE * MATRIX_SIZE;

    // Calculate GFLOPS
    double gflops = flops / (elapsed_time * 1e9);
    printf("GFLOPS: %f\n", gflops);

    return 0;
}
