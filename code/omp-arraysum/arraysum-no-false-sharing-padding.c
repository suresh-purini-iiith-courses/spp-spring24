#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define CACHE_LINE_SIZE 64                          // Common cache line size
#define PAD_SIZE (CACHE_LINE_SIZE / sizeof(double)) // Number of doubles to pad to the size of a cache line

int main()
{
    int n = 100000000; // Array size
    double *array = malloc(n * sizeof(double));
    double totalSum = 0.0, seqSum = 0.0;
    double startTime, endTime, parallelTime, seqTime, gflops, speedup;

    // Initialize the array with some values
    for (int i = 0; i < n; i++)
    {
        array[i] = 1.0; // Example: set each element to 1
    }

    // Sequential sum for comparison
    startTime = omp_get_wtime();
    for (int i = 0; i < n; i++)
    {
        seqSum += array[i];
    }
    endTime = omp_get_wtime();
    seqTime = endTime - startTime;

    int numThreads = omp_get_max_threads();
    // Allocating padded partial sums to potentially increase cache contention
    double *partialSums = malloc(numThreads * PAD_SIZE * sizeof(double));

    startTime = omp_get_wtime();
#pragma omp parallel shared(array, partialSums)
    {
        int tid = omp_get_thread_num();       // Thread ID
        int start = tid * n / numThreads;     // Start index for this thread
        int end = (tid + 1) * n / numThreads; // End index for this thread
        double localSum = 0.0;                // Initialize local sum

        for (int i = start; i < end; i++)
        {
            localSum += 1; //array[i];
        }
        partialSums[tid * PAD_SIZE] = localSum; // Store in padded position to avoid false sharing
    }

    for (int i = 0; i < numThreads; i++)
    {
        totalSum += partialSums[i * PAD_SIZE]; // Collect sums from padded positions
    }
    endTime = omp_get_wtime();
    parallelTime = endTime - startTime;

    // Calculations
    gflops = ((double)n - 1) / (parallelTime * 1e9); // Giga Floating Point Operations per Second
    speedup = seqTime / parallelTime;

    printf("Sequential sum: %f, Time: %f seconds\n", seqSum, seqTime);
    printf("Parallel sum: %f, Time: %f seconds\n", totalSum, parallelTime);
    printf("GFLOPS/s: %f\n", gflops);
    printf("Speedup: %f\n", speedup);

    free(array);
    free(partialSums);

    return 0;
}
