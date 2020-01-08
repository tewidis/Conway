#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

const int WIDTH = 50;
const int HEIGHT = 50;
const int ITERATIONS = 10;

/***
  Initial Configurations:
    Block
    Beehive
    Loaf
    Boat
    Tub
    Blinker
    Toad
    Beacon
    Pulsar
    Pentadecathlon
    Glider
    LWSS
    MWSS
    HWSS
    Rpentomino
    Diehard
    Acorn
***/

int** initialize_board(char* initial_state)
{
    int** board = (int**) malloc(sizeof(int*)*WIDTH);
    time_t t;
    srand((unsigned) time(&t));
    int row, col;
    for(row = 0; row < WIDTH; row++)
    {
        board[row] = (int*) malloc(sizeof(int)*HEIGHT);
    }

    for(row = 0; row < WIDTH; row++)
    {
        for(col = 0; col < HEIGHT; col++)
        {
            if(strcmp(initial_state, "zero") == 0) { board[row][col] = 0; }
            else if(strcmp(initial_state, "random") == 0) { board[row][col] = rand() % 2; }
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

void iterate(int** curr_state, int** next_state)
{
    int row, col, iter;

    for(iter = 0; iter < ITERATIONS; iter++)
    {
        for(row = 0; row < WIDTH; row++)
        {
            for(col = 0; col < HEIGHT; col++)
            {
                int neighbors = get_num_neighbors(curr_state, row, col);

                if(curr_state[row][col] == 1) // alive
                {
                    if(neighbors == 2 || neighbors == 3) { next_state[row][col] = 1; }
                    else { next_state[row][col] = 0; }
                }
                else // dead
                {
                    if(neighbors == 3) { next_state[row][col] = 1; }
                }
            }
        }

        print_board(curr_state);
        delete_board(curr_state);
        curr_state = next_state;
        next_state = initialize_board("zero");
        sleep(1);
    }
}

int main(int argc, char* argv[])
{
    int** curr_state;
    int** next_state;

    if(argc != 2)
    {
        curr_state = initialize_board("random");
    }
    else
    {
        curr_state = initialize_board(argv[1]);
    }

    next_state = initialize_board("zero");

    iterate(curr_state, next_state);

    delete_board(curr_state);
    delete_board(next_state);
}

