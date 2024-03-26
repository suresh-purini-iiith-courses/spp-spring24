#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

double arraySum(int tid, double *A, int n)
{
    double psum = 0.0;
    int numThreads = omp_get_num_threads();
    int start = tid * n / numThreads;     // Start index for this thread
    int end = (tid + 1) * n / numThreads; // End index for this thread
    for (int i = start; i < end; i++)
    {
        psum += A[i];
    }
    return psum;
}

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

    startTime = omp_get_wtime();
#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        double psum = 0;
        psum = arraySum(tid, array, n);
#pragma omp atomic
        totalSum += psum;
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

    if (totalSum != seqSum)
        printf("Error!\n");
    else
        printf("Correct computation!\n");

    free(array);
    return 0;
}
