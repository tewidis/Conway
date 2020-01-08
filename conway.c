#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

const int WIDTH = 50;
const int HEIGHT = 50;
const int ITERATIONS = 10;

int** initialize_board(bool zero_initialize)
{
    int** board = (int**) malloc(sizeof(int*)*WIDTH);
    int row, col;
    for(row = 0; row < WIDTH; row++)
    {
        board[row] = (int*) malloc(sizeof(int)*HEIGHT);
    }

    time_t t;
    srand((unsigned) time(&t));

    for(row = 0; row < WIDTH; row++)
    {
        for(col = 0; col < HEIGHT; col++)
        {
            if(zero_initialize) { board[row][col] = 0; }
            else { board[row][col] = rand() % 2; }
        }
    }
    return board;
}

int** delete_board(int** board)
{
    free(*board);
    free(board);
}

void print_board(int** board)
{
    system("clear");
    int row, col;
    for(row = 0; row < WIDTH; row++)
    {
        for(col = 0; col < HEIGHT; col++)
        {
            printf("%d ", board[row][col]);
            //fflush(stdout);
        }
        printf("\n");
    }
    printf("\n");
}

int get_num_neighbors(int** board, int i, int j)
{
    int num_neighbors = 0;

    if(i > 0) { num_neighbors += board[i-1][j]; }
    if(j > 0) { num_neighbors += board[i][j-1]; }
    if(i < WIDTH-1) { num_neighbors += board[i+1][j]; }
    if(j < HEIGHT-1) { num_neighbors += board[i][j+1]; }
    if(i > 0 && j < HEIGHT-1) { num_neighbors += board[i-1][j+1]; }
    if(i < WIDTH-1 && j > 0) { num_neighbors += board[i+1][j-1]; }
    if(i < WIDTH-1 && j < HEIGHT-1) { num_neighbors += board[i+1][j+1]; }
    if(i > 0 && j > 0) { num_neighbors += board[i-1][j-1]; }

    return num_neighbors;
}

int main()
{
    int** curr_state = initialize_board(false);
    int** next_state = initialize_board(true);
    int row, col, iter;

    print_board(curr_state);

    for(iter = 0; iter < ITERATIONS; iter++)
    {
        for(row = 0; row < WIDTH; row++)
        {
            for(col = 0; col < HEIGHT; col++)
            {
                int neighbors = get_num_neighbors(curr_state, row, col);
                if(curr_state[row][col] == 1)
                {
                    if(neighbors == 2 || neighbors == 3)
                    {
                        next_state[row][col] = 1;
                    }
                    else
                    {
                        next_state[row][col] = 0;
                    }
                }
                else // currently dead
                {
                    if(neighbors == 3)
                    {
                        next_state[row][col] = 1;
                    }
                }
            }
        }

        print_board(curr_state);
        delete_board(curr_state);
        curr_state = next_state;
        next_state = initialize_board(true);
        sleep(1);
    }
    delete_board(curr_state);
    delete_board(next_state);
}

