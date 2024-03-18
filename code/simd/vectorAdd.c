#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void addVectorsBasic(float* a, float* b, float* c, int N) {
    for (int i = 0; i < N; ++i) {
        c[i] = a[i] + b[i];
    }
}

void addVectorsAVX2(float* a, float* b, float* c, int N) {
    int i = 0;
    for (; i <= N - 8; i += 8) {
        __m256 av = _mm256_loadu_ps(&a[i]);
        __m256 bv = _mm256_loadu_ps(&b[i]);
        __m256 cv = _mm256_add_ps(av, bv);
        _mm256_storeu_ps(&c[i], cv);
    }
    for (; i < N; ++i) { // handle remainder
        c[i] = a[i] + b[i];
    }
}

double computeGFLOPS(int N, double elapsedTime) {
    return (N * 1e-9) / elapsedTime;
}

double getTime() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return (double)t.tv_sec + (double)t.tv_nsec * 1e-9;
}

int main() {
    const int N = 10000000; // Number of elements in the vectors
    float *a = (float*)malloc(sizeof(float) * N);
    float *b = (float*)malloc(sizeof(float) * N);
    float *c = (float*)malloc(sizeof(float) * N);

    // Initialize vectors
    for(int i = 0; i < N; ++i) {
        a[i] = (float)i;
        b[i] = (float)i;
    }

    // Measure basic addition
    double startTime = getTime();
    addVectorsBasic(a, b, c, N);
    double endTime = getTime();
    double timeBasic = endTime - startTime;
    double gflopsBasic = computeGFLOPS(N, timeBasic);

    // Measure AVX2 addition
    startTime = getTime();
    addVectorsAVX2(a, b, c, N);
    endTime = getTime();
    double timeAVX2 = endTime - startTime;
    double gflopsAVX2 = computeGFLOPS(N, timeAVX2);

    printf("Basic Addition Time: %f s, GFLOPS: %f\n", timeBasic, gflopsBasic);
    printf("AVX2 Addition Time: %f s, GFLOPS: %f\n", timeAVX2, gflopsAVX2);
    printf("Speedup: %f x\n", timeBasic / timeAVX2);

    free(a);
    free(b);
    free(c);
    return 0;
}
