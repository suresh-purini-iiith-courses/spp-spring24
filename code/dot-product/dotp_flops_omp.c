#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

#define SIZE 1000000 // Size of vectors

// Function to compute dot product of two vectors
float dot_product(const float *a, const float *b, int size) {
    float result = 0.0f;
    int num_threads, i;
    
    #pragma omp parallel private(i) shared(a, b) reduction(+:result)
    {
        num_threads = omp_get_num_threads();
        #pragma omp for
        for (i = 0; i < size; ++i) {
            result += a[i] * b[i];
        }
    }
    printf("Number of threads: %d\n", num_threads);
    return result;
}

// Function to get current time in microseconds
long long current_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000000LL + (long long)tv.tv_usec;
}

int main() {
    // Allocate memory for vectors
    float *a = (float *)malloc(SIZE * sizeof(float));
    float *b = (float *)malloc(SIZE * sizeof(float));
    if (a == NULL || b == NULL) {
        printf("Error: Memory allocation failed!\n");
        return -1;
    }

    // Initialize vectors with random values
    for (int i = 0; i < SIZE; ++i) {
        a[i] = (float)rand() / RAND_MAX;
        b[i] = (float)rand() / RAND_MAX;
    }

    // Benchmarking
    long long start_time = current_time();
    float result = dot_product(a, b, SIZE);
    long long end_time = current_time();
    double elapsed_time = (end_time - start_time) / 1000000.0; // Convert to seconds

    // Calculate GFLOPS/s
    double gflops_per_sec = (2.0 * SIZE) / (elapsed_time * 1e9);

    // Output result, benchmarking information, and GFLOPS/s
    printf("Dot product: %.6f\n", result);
    printf("Time elapsed: %.3f milliseconds\n", elapsed_time*1000);
    printf("GFLOPS/s: %.2f\n", gflops_per_sec);

    // Free allocated memory
    free(a);
    free(b);

    return 0;
}
