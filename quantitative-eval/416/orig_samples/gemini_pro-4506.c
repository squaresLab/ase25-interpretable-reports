//GEMINI-pro DATASET v1.0 Category: Matrix operations ; Style: Romeo and Juliet
#include <stdio.h>
#include <stdlib.h>

// Define the Romeo and Juliet matrices
int romeo[3][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};

int juliet[3][3] = {
    {10, 11, 12},
    {13, 14, 15},
    {16, 17, 18}
};

// Define the function to add the two matrices
int add_matrices(int a[3][3], int b[3][3]) {
    int c[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
    return c;
}

// Define the function to subtract the two matrices
int subtract_matrices(int a[3][3], int b[3][3]) {
    int c[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            c[i][j] = a[i][j] - b[i][j];
        }
    }
    return c;
}

// Define the function to multiply the two matrices
int multiply_matrices(int a[3][3], int b[3][3]) {
    int c[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            c[i][j] = 0;
            for (int k = 0; k < 3; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return c;
}

// Define the function to print the matrix
void print_matrix(int a[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
}

// Main function
int main() {
    // Add the two matrices
    int sum = add_matrices(romeo, juliet);

    // Subtract the two matrices
    int difference = subtract_matrices(romeo, juliet);

    // Multiply the two matrices
    int product = multiply_matrices(romeo, juliet);

    // Print the results
    printf("The sum of the two matrices is:\n");
    print_matrix(sum);

    printf("The difference of the two matrices is:\n");
    print_matrix(difference);

    printf("The product of the two matrices is:\n");
    print_matrix(product);

    return 0;
}