#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#define N 9

int found = 0;

void print(int arr[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%d ", arr[i][j]);
        printf("\n");
    }
}

int isSafe(int grid[N][N], int row, int col, int num) {
    for (int x = 0; x <= 8; x++)
        if (grid[row][x] == num) return 0;

    for (int x = 0; x <= 8; x++)
        if (grid[x][col] == num) return 0;

    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j + startCol] == num) return 0;

    return 1;
}

int solveSudoku(int grid[N][N], int row, int col) {
    if (found) return 0;

    if (row == N - 1 && col == N) {
        #pragma omp critical
        {
            if (!found) {
                found = 1;
                printf("Solucao encontrada:\n");
                print(grid);
            }
        }
        return 1;
    }

    if (col == N) {
        row++;
        col = 0;
    }

    if (grid[row][col] > 0)
        return solveSudoku(grid, row, col + 1);

    for (int num = 1; num <= N; num++) {
        if (isSafe(grid, row, col, num)) {
            
            if (row == 0 && !found) {
                #pragma omp task firstprivate(grid, row, col, num)
                {
                    int local_grid[N][N];
                    memcpy(local_grid, grid, sizeof(int) * N * N);
                    local_grid[row][col] = num;
                    solveSudoku(local_grid, row, col + 1);
                }
            } else {
                grid[row][col] = num;
                if (solveSudoku(grid, row, col + 1)) return 1;
                grid[row][col] = 0;
            }
        }
    }
    return 0;
}

int main() {
    int grid[N][N] = { 
        { 5, 3, 0, 0, 7, 0, 0, 0, 0 },
        { 6, 0, 0, 1, 9, 5, 0, 0, 0 },
        { 0, 9, 8, 0, 0, 0, 0, 6, 0 },
        { 8, 0, 0, 0, 6, 0, 0, 0, 3 },
        { 4, 0, 0, 8, 0, 3, 0, 0, 1 },
        { 7, 0, 0, 0, 2, 0, 0, 0, 6 },
        { 0, 6, 0, 0, 0, 0, 2, 8, 0 },
        { 0, 0, 0, 4, 1, 9, 0, 0, 5 },
        { 0, 0, 0, 0, 8, 0, 0, 7, 9 } 
    };

    #pragma omp parallel
    {
        #pragma omp single
        {
            solveSudoku(grid, 0, 0);
        }
    }

    if (!found) printf("No solution exists");

    return 0;
}