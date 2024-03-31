#include <stdio.h>
#include <omp.h>

int main() {
    omp_set_num_threads(2);
    #pragma omp parallel default(none) num_threads(4)
    {
        #pragma omp single 
        {
            #pragma omp task
            printf("T%d: A\n", omp_get_thread_num());
            #pragma omp taskwait
            #pragma omp task
            printf("T%d: B\n", omp_get_thread_num());
        }
    }

    return 0;
}
