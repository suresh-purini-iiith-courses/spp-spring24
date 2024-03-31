#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_ELEMENTS 10000000
#define NUM_THREADS 4

// Linked list node structure
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Function to create a new node
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a node at the end of the linked list
void insert(Node** head_ref, int data) {
    Node* newNode = createNode(data);
    newNode->next = *head_ref;
    *head_ref = newNode;
}

// Function to traverse the linked list in parallel
void traverseLinkedListParallel(Node* head) {
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        #pragma omp single
        {
            Node* current = head;
            while (current != NULL) {
                #pragma omp task
                {
                    // Access data of the current node
                   // printf("%d ", current->data);
                   current->data = current->data + 1;
                }
                current = current->next;
            }
        }
    }
}

// Function to traverse the linked list sequentially
void traverseLinkedListSequential(Node* head) {
    Node* current = head;
    while (current != NULL) {
        current->data = current->data + 1;
        //printf("%d ", current->data);
        current = current->next;
    }
}

int main() {
    Node* head = NULL;

    // Insert elements into the linked list
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        insert(&head, i);
    }

    printf("Parallel traversal:\n");
    double start_parallel = omp_get_wtime();
    traverseLinkedListParallel(head);
    double end_parallel = omp_get_wtime();
    printf("\nParallel traversal time: %.6f seconds\n", end_parallel - start_parallel);

    printf("\nSequential traversal:\n");
    double start_sequential = omp_get_wtime();
    traverseLinkedListSequential(head);
    double end_sequential = omp_get_wtime();
    printf("\nSequential traversal time: %.6f seconds\n", end_sequential - start_sequential);

    // Calculate and print the ratio of traversal times
    double ratio = (end_sequential - start_sequential) / (end_parallel - start_parallel);
    printf("\nRatio (Sequential Time / Parallel Time): %.2f\n", ratio);

    return 0;
}
