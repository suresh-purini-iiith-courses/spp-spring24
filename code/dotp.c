#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define SIZE 1000000 // Size of vectors

// Function to compute dot product of two vectors
float dot_product(const float *a, const float *b, int size) {
    float result = 0.0f;
    for (int i = 0; i < size; ++i) {
        result += a[i] * b[i];
    }
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

    // Output result and benchmarking information
    printf("Dot product: %.6f\n", result);
    printf("Time elapsed: %.6f seconds\n", elapsed_time);

    // Free allocated memory
    free(a);
    free(b);

    return 0;
}
