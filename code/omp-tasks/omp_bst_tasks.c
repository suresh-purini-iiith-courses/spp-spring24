#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_KEYS 10000
#define WORK 1000000
// Binary search tree node structure
typedef struct Node {
    int data;
    struct Node *left, *right;
} Node;

// Function to create a new node
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Function to insert a node into BST
Node* insert(Node* root, int data) {
    if (root == NULL)
        return createNode(data);

    if (data < root->data)
        root->left = insert(root->left, data);
    else if (data > root->data)
        root->right = insert(root->right, data);

    return root;
}

// Function for parallel inorder traversal of BST
void parallelInorder(Node* root) {
    if (root == NULL)
        return;

    #pragma omp task
    parallelInorder(root->left);
    
    //printf("%d ", root->data);
  //  printf("By T%d: \n", omp_get_thread_num());
    root->data++;
    for(int i=0; i < WORK; ++i){
        double d = 0;
        root->data += rand();
    }
    #pragma omp task
    parallelInorder(root->right);
}

// Function for sequential inorder traversal of BST
void sequentialInorder(Node* root) {
    if (root == NULL)
        return;

    sequentialInorder(root->left);
    //printf("%d ", root->data);
    root->data++;
    for(int i=0; i < WORK; ++i){
        double d = 0;
        root->data += rand();
    }
    sequentialInorder(root->right);
}

int main() {
    Node* root = NULL;

    // Insert elements into the BST
    for (int i = 0; i < NUM_KEYS; i++) {
        root = insert(root, i);
    }

    printf("Parallel inorder traversal:\n");
    double start_parallel = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        parallelInorder(root);
    }
    double end_parallel = omp_get_wtime();
    printf("\nParallel traversal time: %.6f seconds\n", end_parallel - start_parallel);

    printf("\nSequential inorder traversal:\n");
    double start_sequential = omp_get_wtime();
    sequentialInorder(root);
    double end_sequential = omp_get_wtime();
    printf("\nSequential traversal time: %.6f seconds\n", end_sequential - start_sequential);

    // Calculate and print the ratio of traversal times
    double ratio = (end_sequential - start_sequential) / (end_parallel - start_parallel);
    printf("\nRatio (Sequential Time / Parallel Time): %.2f\n", ratio);

    return 0;
}
