#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <immintrin.h>

// Convolution without SIMD
void convolution_basic(float* src, float* dst, int width, int height, const float* filter) {
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            float sum = 0.0f;
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    sum += src[(y + ky) * width + (x + kx)] * filter[(ky + 1) * 3 + (kx + 1)];
                }
            }
            dst[y * width + x] = sum;
        }
    }
}

// Convolution with SIMD (AVX2)
void convolution_avx2(float* src, float* dst, int width, int height, const float* filter) {
    for (int row = 1; row < height - 1; ++row) {
        for (int col = 1; col <= width - 9; col += 8) { // Process 8 pixels per iteration, ensuring we stay within bounds
            __m256 sum = _mm256_setzero_ps();
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    __m256 pixels = _mm256_loadu_ps(src + (row + ky) * width + (col + kx));
                    __m256 filterVal = _mm256_set1_ps(filter[(ky + 1) * 3 + (kx + 1)]);
                    sum = _mm256_fmadd_ps(pixels, filterVal, sum);
                }
            }
            _mm256_storeu_ps(dst + row * width + col, sum);
        }
    }
}

double get_time() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)t.tv_sec + (double)t.tv_nsec * 1e-9;
}

//return GFLOPS/s
double benchmark(float* src, float* dst, int width, int height, const float* filter, int use_simd) {
    double start_time, end_time;
    
    start_time = get_time();
    if (use_simd) {
        convolution_avx2(src, dst, width, height, filter);
    } else {
        convolution_basic(src, dst, width, height, filter);
    }
    end_time = get_time();
    
    double total_time = end_time - start_time;
    long long total_ops = (long long)width * height * 17; // 9 + 8 operations per pixel (for 3x3 filter)
    double gflops = (total_ops / 1e9) / total_time;
    
    printf("Method: %s, Time: %.5f seconds, GFLOPS: %.5f\n", use_simd ? "SIMD (AVX2)" : "Non-SIMD", total_time, gflops);

    return gflops;
}

int main() {
    const int width = 256, height = 256;
    float *src = malloc(width * height * sizeof(float));
    float *dst = malloc(width * height * sizeof(float));
    float filter[9] = {1/9.0, 1/9.0, 1/9.0, 1/9.0, 1/9.0, 1/9.0, 1/9.0, 1/9.0, 1/9.0};
    
    // Fill src with dummy data
    for (int i = 0; i < width * height; ++i) {
        src[i] = (float)i;
    }
    
    double non_simd_gflops, simd_gflops;

    non_simd_gflops = benchmark(src, dst, width, height, filter, 0); // Benchmark non-SIMD
    simd_gflops = benchmark(src, dst, width, height, filter, 1); // Benchmark SIMD (AVX2)
    
    printf("Speedup: %.2f\n", simd_gflops/non_simd_gflops);
    free(src);
    free(dst);
    return 0;
}
