#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE (1024*1024*1024) // 1GB
#define ELEMENTS (SIZE / sizeof(double))
#define BILLION 1000000000L

double data[ELEMENTS];

void init_array() {
    for (size_t i = 0; i < ELEMENTS; i++) {
        data[i] = (double)i;
    }
}

double read_array() {
    double sum = 0;
    for (size_t i = 0; i < ELEMENTS; i++) {
        sum += data[i];
    }
    return sum;
}

int main() {
    struct timespec start, end;
    double time_used, bandwidth;

    printf("Initializing array...\n");
    init_array();
    printf("Reading array...\n");

    clock_gettime(CLOCK_MONOTONIC, &start);
    double sum = read_array();
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate time taken in seconds
    time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / (double)BILLION;
    printf("Sum: %f\n", sum);
    printf("Time taken: %.8f seconds\n", time_used);

    // Calculate bandwidth
    // SIZE is the total bytes read, and time_used is in seconds
    bandwidth = SIZE / (time_used * 1024 * 1024 * 1024); // GB/s
    printf("Estimated Read Bandwidth: %.2f GB/s\n", bandwidth);

    return 0;
}
