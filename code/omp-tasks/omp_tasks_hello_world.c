#include <stdio.h>
#include <omp.h>

int main() {
    omp_set_num_threads(2);
    #pragma omp parallel default(none) num_threads(2)
    {
        #pragma omp tied task
        printf("T%d: A\n", omp_get_thread_num());
        #pragma omp tied task
        printf("T%d: B\n", omp_get_thread_num());
    }

    return 0;
}
