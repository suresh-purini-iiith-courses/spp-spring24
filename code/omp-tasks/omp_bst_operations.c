#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_OPERATIONS 1000
#define MAX_KEY 1000

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

// Function to search for a key in BST
Node* search(Node* root, int key) {
    if (root == NULL || root->data == key)
        return root;

    if (root->data < key)
        return search(root->right, key);

    return search(root->left, key);
}

// Function to find the minimum value node in a BST
Node* minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

// Function to delete a node from BST
Node* deleteNode(Node* root, int key) {
    if (root == NULL)
        return root;

    if (key < root->data)
        root->left = deleteNode(root->left, key);
    else if (key > root->data)
        root->right = deleteNode(root->right, key);
    else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }

        Node* temp = minValueNode(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }
    return root;
}

// Function to perform random insert, delete, and search operations on BST
void performOperations(Node** root) {
    #pragma omp parallel for
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        int operation = rand() % 3; // Randomly select an operation: 0 = insert, 1 = delete, 2 = search
        int key = rand() % (MAX_KEY + 1); // Generate a random key

        switch (operation) {
            case 0:
                #pragma omp task
                *root = insert(*root, key);
                break;
            case 1:
                #pragma omp task
                *root = deleteNode(*root, key);
                break;
            case 2:
                #pragma omp task
                if (search(*root, key) != NULL)
                    printf("Key %d found\n", key);
                else
                    printf("Key %d not found\n", key);
                break;
        }
    }
}

int main() {
    Node* root = NULL;

    // Perform random insert, delete, and search operations
    #pragma omp parallel
    {
        #pragma omp single nowait
        performOperations(&root);
    }

    return 0;
}
