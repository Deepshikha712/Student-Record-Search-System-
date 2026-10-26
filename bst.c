#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    int id;
    char name[50];
    char department[50];
} Student;
typedef struct Node {
    Student data;
    struct Node *left;
    struct Node *right;
} Node;
Node *createNode(Student s) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = s;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}
Node *insertNode(Node *root, Student s, int *comparisons) {
    if (root == NULL)
        return createNode(s);
    if (comparisons != NULL)
        (*comparisons)++;
    if (s.id < root->data.id)
        root->left = insertNode(root->left, s, comparisons);
    else if (s.id > root->data.id)
        root->right = insertNode(root->right, s, comparisons);
    else
        printf("Duplicate enrolment ID! Record not inserted.\n");

    return root;
}
Node *searchNode(Node *root, int id, int *comparisons) {
    while (root != NULL) {
        (*comparisons)++;
        if (id == root->data.id)
            return root;
        if (id < root->data.id)
            root = root->left;
        else
            root = root->right;
    }
    return NULL;
}
void displayStudent(Student s) {
    printf("ID: %d | Name: %s | Department: %s\n",
           s.id, s.name, s.department);
}
Node *findMin(Node *root) {
    while (root != NULL && root->left != NULL)
        root = root->left;
    return root;
}
Node *deleteNode(Node *root, int id, int *found) {
    if (root == NULL)
        return NULL;
    if (id < root->data.id) {
        root->left = deleteNode(root->left, id, found);
    } else if (id > root->data.id) {
        root->right = deleteNode(root->right, id, found);
    } else {
        *found = 1;
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        if (root->left == NULL) {
            Node *temp = root->right;
            free(root);
            return temp;
        }
        if (root->right == NULL) {
            Node *temp = root->left;
            free(root);
            return temp;
        }
        Node *temp = findMin(root->right);
        root->data = temp->data;
        int ignored = 0;
        root->right = deleteNode(root->right, temp->data.id, &ignored);
    }
    return root;
}
void inorder(Node *root) {
    if (root != NULL) {
        inorder(root->left);
        displayStudent(root->data);
        inorder(root->right);
    }
}
void preorder(Node *root) {
    if (root != NULL) {
        displayStudent(root->data);
        preorder(root->left);
        preorder(root->right);
    }
}
void postorder(Node *root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        displayStudent(root->data);
    }
}
int treeHeight(Node *root) {
    if (root == NULL)
        return 0;
    int leftHeight = treeHeight(root->left);
    int rightHeight = treeHeight(root->right);

    return 1 + (leftHeight > rightHeight
                ? leftHeight : rightHeight);
}
void freeTree(Node *root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}
void insertSampleRecords(Node **root) {
    Student samples[] = {
        {50, "Aman", "CSE"},
        {30, "Priya", "AIDS"},
        {70, "Rahul", "CSE"},
        {20, "Neha", "ECE"},
        {40, "Simran", "AIDS"},
        {60, "Karan", "CSE"},
        {80, "Riya", "ECE"},
        {10, "Mohit", "CSE"},
        {35, "Anjali", "AIDS"},
        {65, "Arjun", "ECE"}
    };
    int count = sizeof(samples) / sizeof(samples[0]);
    int i;
    for(i = 0; i < count; i++) {
        *root = insertNode(*root, samples[i], NULL);
    }
    printf("Sample records processed: %d\n", count);
    printf("Use Inorder Traversal to verify the records.\n");
}
void compareTrees(void) {
    int balancedOrder[] = {5, 3, 8, 2, 4, 7, 9, 1, 6, 10};
    int sortedOrder[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Node *balanced = NULL;
    Node *sorted = NULL;
    int balancedComparisons = 0;
    int sortedComparisons = 0;
    int i;
    int id;
    for(i = 0; i < 10; i++) {
        Student s;
        s.id = balancedOrder[i];
        sprintf(s.name, "Student%d", s.id);
        strcpy(s.department, "CSE");
        balanced = insertNode(balanced, s, NULL);
    }
    for(i = 0; i < 10; i++) {
        Student s;
        s.id = sortedOrder[i];
        sprintf(s.name, "Student%d", s.id);
        strcpy(s.department, "CSE");
        sorted = insertNode(sorted, s, NULL);
    }
    for(id = 1; id <= 10; id++) {
        searchNode(balanced, id, &balancedComparisons);
        searchNode(sorted, id, &sortedComparisons);
    }
    printf("\n--- TREE EFFICIENCY COMPARISON ---\n");
    printf("\nBalanced-looking BST:\n");
    printf("Height: %d nodes\n", treeHeight(balanced));
    printf("Total search comparisons: %d\n", balancedComparisons);
    printf("Average comparisons: %.2f\n",
           balancedComparisons / 10.0);
    printf("\nSorted-insertion BST:\n");
    printf("Height: %d nodes\n", treeHeight(sorted));
    printf("Total search comparisons: %d\n", sortedComparisons);
    printf("Average comparisons: %.2f\n",
           sortedComparisons / 10.0);
    freeTree(balanced);
    freeTree(sorted);
}
int main(void) {
    Node *root = NULL;
    int choice, id, comparisons, found;
    Student s;
    Node *result;
    do {
        printf("\n===== STUDENT RECORD SEARCH SYSTEM =====\n");
        printf("1. Insert 10 sample records\n");
        printf("2. Insert one student record\n");
        printf("3. Search student by ID\n");
        printf("4. Delete student by ID\n");
        printf("5. Inorder traversal (sorted)\n");
        printf("6. Preorder traversal\n");
        printf("7. Postorder traversal\n");
        printf("8. Compare tree efficiency\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please restart the program.\n");
            break;
        }
        switch (choice) {
            case 1:
                insertSampleRecords(&root);
                break;
            case 2:
                printf("Enter enrolment ID: ");
                if (scanf("%d", &s.id) != 1) {
                    printf("Invalid ID input.\n");
                    freeTree(root);
                    return 0;
                }
                printf("Enter student name: ");
                if (scanf(" %49[^\n]", s.name) != 1) {
                    printf("Invalid name input.\n");
                    freeTree(root);
                    return 0;
                }
                printf("Enter department: ");
                if (scanf(" %49[^\n]", s.department) != 1) {
                    printf("Invalid department input.\n");
                    freeTree(root);
                    return 0;
                }
                root = insertNode(root, s, NULL);
                break;
            case 3:
                printf("Enter enrolment ID to search: ");
                if (scanf("%d", &id) != 1) {
                    printf("Invalid ID input.\n");
                    freeTree(root);
                    return 0;
                }
                comparisons = 0;
                result = searchNode(root, id, &comparisons);
                if (result != NULL) {
                    printf("\nStudent record found:\n");
                    displayStudent(result->data);
                } else {
                    printf("Student record not found!\n");
                }
                printf("Comparisons: %d\n", comparisons);
                break;
            case 4:
                printf("Enter enrolment ID to delete: ");
                if (scanf("%d", &id) != 1) {
                    printf("Invalid ID input.\n");
                    freeTree(root);
                    return 0;
                }
                found = 0;
                root = deleteNode(root, id, &found);
                if (found)
                    printf("Record deleted successfully.\n");
                else
                    printf("Student record not found!\n");
                break;
            case 5:
                printf("\n--- INORDER TRAVERSAL ---\n");
                if (root == NULL)
                    printf("No student records available.\n");
                else
                    inorder(root);
                break;
            case 6:
                printf("\n--- PREORDER TRAVERSAL ---\n");
                if (root == NULL)
                    printf("No student records available.\n");
                else
                    preorder(root);
                break;
            case 7:
                printf("\n--- POSTORDER TRAVERSAL ---\n");
                if (root == NULL)
                    printf("No student records available.\n");
                else
                    postorder(root);
                break;
            case 8:
                compareTrees();
                break;
            case 9:
                printf("Exiting Student Record Search System.\n");
                break;
            default:
                printf("Invalid choice! Please select 1 to 9.\n");
        }
    } while (choice != 9);
    freeTree(root);
    return 0;
}
