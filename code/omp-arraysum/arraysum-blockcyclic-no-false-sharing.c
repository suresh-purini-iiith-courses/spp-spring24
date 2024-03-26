#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n = 100000000; // Array size
    double *array = malloc(n * sizeof(double));
    double totalSum = 0.0, seqSum = 0.0;
    double startTime, endTime, parallelTime, seqTime, gflops, speedup;
    int blockSize = 100; // Size of each block in block-cyclic distribution

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

    startTime = omp_get_wtime();
#pragma omp parallel
    {
        double localSum = 0.0; // Local sum for each thread to avoid false sharing
        int numThreads = omp_get_num_threads();
        int threadId = omp_get_thread_num();
        for (int blockStart = threadId * blockSize; blockStart < n; blockStart += blockSize * numThreads)
        {
            int blockEnd = blockStart + blockSize < n ? blockStart + blockSize : n;
            for (int i = blockStart; i < blockEnd; i++)
            {
                localSum += array[i];
            }
        }
#pragma omp atomic
        totalSum += localSum;
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
    return 0;
}
