#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

const int WIDTH = 4;
const int HEIGHT = 4;
const int ITERATIONS = 1;

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
int* convert(char* c)
{
    int len = strlen(c),i;
    int* a = (int*) malloc(len*sizeof(int));
    for(i = 0; i < len; i++)
    {
        a[i] = c[i] - 48;
        printf("%d\n", a[i]);
    }
    return a;
}

typedef struct fileformat {
    int rows;
    int cols;
    int** initialize;
} fileformat;

fileformat* read_config_file(char* filename)
{
    FILE* fid;
    char str[1000];
    fid = fopen(filename, "r");
    int rows = atoi(fgets(str, 1000, fid));
    int cols = atoi(fgets(str, 1000, fid));
    int i;
    int** board = (int**) malloc(sizeof(int*)*rows);

    for(i = 0; i < rows; i++)
    {
        board[i] = convert(fgets(str, 1000, fid));
    }
    fclose(fid);

    fileformat* file = (fileformat*) malloc(sizeof(fileformat));
    file->rows = rows;
    file->cols = cols;
    file->initialize = board;

    return file;
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

// TODO: Make zero an initial state, not a boolean
int** initialize_board(char* initial_state, bool zero_init)
{
    int width, height, row, col;
    int** board = (int**) malloc(sizeof(int*)*width);
    for(row = 0; row < width; row++)
    {
        board[row] = (int*) malloc(sizeof(int)*height);
    }

    if(strcmp(initial_state, "random") == 0)
    {
        width = WIDTH;
        height = HEIGHT;
        time_t t;
        srand((unsigned) time(&t));

        for(row = 0; row < width; row++)
        {
            for(col = 0; col < height; col++)
            {
                if(zero_init)
                {
                    board[row][col] = 0;
                }
                else
                {
                    board[row][col] = rand() % 2;
                }
            }
        }
    }
    else
    {
        fileformat* info = read_config_file("/home/twidis/conway/init_configs/block.txt");
        width = info->cols;
        height = info->rows;
        //board = info->initialize;
        for(row = 0; row < width; row++)
        {
            for(col = 0; col < height; col++)
            {
                board[row][col] = info->initialize[row][col];
            }
        }
    }

    return board;
}

int** delete_board(int** board)
{
    free(*board);
    free(board);
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
        next_state = initialize_board("random", true);
        sleep(1);
    }
}

int main(int argc, char* argv[])
{
    int** curr_state;
    int** next_state;

    if(argc != 2)
    {
        curr_state = initialize_board("random", false);
        next_state = initialize_board("random", true);
    }
    else
    {
        curr_state = initialize_board(argv[1], false);
        next_state = initialize_board(argv[1], true);
    }
    return(1);

    iterate(curr_state, next_state);

    delete_board(curr_state);
    delete_board(next_state);
}

