#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        #pragma omp single
        {
            // Create two tasks
            #pragma omp task
            {
                printf("T%d: Task 1\n", omp_get_thread_num());
            }
            #pragma omp task
            {
                printf("T%d: Task 2\n", omp_get_thread_num());
            }

            // Wait for both tasks to complete
            #pragma omp taskwait

            // This will be executed after both tasks are completed
            printf("All tasks completed\n");
        }
    }

    return 0;
}
