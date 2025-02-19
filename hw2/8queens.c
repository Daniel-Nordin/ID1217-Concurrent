#include <omp.h>
#include <stdio.h>
#include <string.h>

#define SIZE 9

int numWorkers = 2;
int next_solution = 0;
double time;

// Function to check if a board is a valid N-Queens solution
int is_safe(int board[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (board[row][col] == 1) {
                // Check vertical and diagonals
                for (int i = 0; i < row; i++) {
                    int diag_left = col - (row - i);
                    int diag_right = col + (row - i);

                    if (board[i][col] == 1) return 0;  // Column conflict
                    if (diag_left >= 0 && board[i][diag_left] == 1) return 0;  // Left diagonal conflict
                    if (diag_right < SIZE && board[i][diag_right] == 1) return 0;  // Right diagonal conflict
                }
            }
        }
    }
    return 1;
}

// Function to validate a board in a separate task
void check_placement(int board[SIZE][SIZE]) {
    if (is_safe(board)) {
        #pragma omp critical
        {
        next_solution++;
        printf("Solution %d found by thread %d!\n", next_solution, omp_get_thread_num());
        }
    }
}

// Recursive function to generate all possible boards
void generate_placements(int board[SIZE][SIZE], int row) {
    if (row == SIZE) {
        // Create a new task to check this board
        int new_board[SIZE][SIZE];
        memcpy(new_board, board, sizeof(new_board));

        #pragma omp task firstprivate(new_board)
        check_placement(new_board);
        
        return;
    }

    for (int col = 0; col < SIZE; col++) {
        board[row][col] = 1;
        generate_placements(board, row + 1);
        board[row][col] = 0;
    }
}

int main() {
    int matrix[SIZE][SIZE] = {0};

    omp_set_num_threads(numWorkers);

    #pragma omp parallel
    {
        double start = omp_get_wtime();
        #pragma omp single
        generate_placements(matrix, 0);
        double end = omp_get_wtime();
        time = end - start;
    }
    #pragma omp taskwait  // Ensure all tasks finish before exiting
    printf("Total Solutions: %d in %f seconds, with %d threads\n", next_solution, time, numWorkers);
    
    return 0;
}
