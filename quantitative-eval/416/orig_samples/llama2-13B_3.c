//LLAMA2-13B DATASET v1.0 Category: Matrix operations ; Style: standalone
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define M 5 // size of the matrix
#define N 3 // number of elements in each row

// Function to print the matrix
void print_matrix(int mat[M][N]) {
    int i, j;
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

// Function to perform matrix multiplication
int matrix_multiply(int mat1[M][N], int mat2[M][N], int mat3[M][N]) {
    int i, j, k;
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            int sum = 0;
            for (k = 0; k < N; k++) {
                sum += mat1[i][k] * mat2[k][j];
            }
            mat3[i][j] = sum;
        }
    }
    return 0;
}

// Function to perform matrix addition
int matrix_add(int mat1[M][N], int mat2[M][N], int mat3[M][N]) {
    int i, j, k;
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            int sum = 0;
            for (k = 0; k < N; k++) {
                sum += mat1[i][k] + mat2[i][k];
            }
            mat3[i][j] = sum;
        }
    }
    return 0;
}

// Function to perform matrix inversion
int matrix_invert(int mat[M][N]) {
    int i, j, k;
    int inv[M][N];
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            inv[i][j] = 0;
        }
    }
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                inv[i][j] += mat[i][k] * inv[k][j];
            }
        }
    }
    return inv;
}

int main() {
    int mat1[M][N] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}},
            mat2[M][N] = {{10, 20, 30}, {40, 50, 60}, {70, 80, 90}},
            mat3[M][N] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            i, j, k;

    // Perform matrix multiplication
    matrix_multiply(mat1, mat2, mat3);

    // Print the result
    print_matrix(mat3);

    // Perform matrix addition
    matrix_add(mat1, mat2, mat3);

    // Print the result
    print_matrix(mat3);

    // Perform matrix inversion
    int inv_mat = matrix_invert(mat1);

    // Print the result
    print_matrix(inv_mat);

    return 0;
}