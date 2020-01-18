#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

const int WIDTH = 8;
const int HEIGHT = 4;
const int ITERATIONS = 10;

typedef struct game {
    int width;
    int height;
    int** board;
} game;

int* convert(char* c)
{
    int len = (strlen(c) / 2) + 1;
    int i;
    int idx = 0;
    int* a = (int*) malloc(len*sizeof(int));

    // TODO: Get rid of these magic numbers
    for(i = 0; i < strlen(c); i++)
    {
        if(c[i] != 32)
        {
            a[idx] = c[i] - 48;
            idx = idx + 1;
        }
    }
    return a;
}

void print_board(game* board)
{
    system("clear");
    int row, col;
    for(row = 0; row < board->height; row++)
    {
        for(col = 0; col < board->width; col++)
        {
            printf("%d ", board->board[row][col]);
            //fflush(stdout);
        }
        printf("\n");
    }
    printf("\n");
}

game* read_config_file(char* filename)
{
    // TODO: Get rid of these magic numbers
    FILE* fid;
    char str[1000];
    fid = fopen(filename, "r");
    int height = atoi(fgets(str, 1000, fid));
    int width = atoi(fgets(str, 1000, fid));
    int i;
    int** board = (int**) malloc(sizeof(int*)*height);

    for(i = 0; i < height; i++)
    {
        board[i] = convert(fgets(str, 1000, fid));
    }
    fclose(fid);

    game* file = (game*) malloc(sizeof(game));
    file->height = height;
    file->width = width;
    file->board = board;

    return file;
}

game* initialize_board(char* initial_state, int width, int height)
{
    int row, col;
    int** board;

    if(width == 0) { width = WIDTH; }
    if(height == 0) { height = HEIGHT; }

    // TODO: Refactor the duplication out of this
    if(strcmp(initial_state, "random") == 0)
    {
        time_t t;
        srand((unsigned) time(&t));

        board = (int**) malloc(sizeof(int*)*height);
        for(row = 0; row < height; row++)
        {
            board[row] = (int*) malloc(sizeof(int)*width);
        }

        for(row = 0; row < height; row++)
        {
            for(col = 0; col < width; col++)
            {
                board[row][col] = rand() % 2;
            }
        }
    }
    else if(strcmp(initial_state, "zero") == 0)
    {
        board = (int**) malloc(sizeof(int*)*height);
        for(row = 0; row < height; row++)
        {
            board[row] = (int*) malloc(sizeof(int)*width);
        }

        for(row = 0; row < height; row++)
        {
            for(col = 0; col < width; col++)
            {
                board[row][col] = 0;
            }
        }
    }
    else
    {
        int total_length = strlen("/home/twidis/conway/init_configs/.txt") + strlen(initial_state) + 1;
        char* filepath = (char*) malloc(total_length * sizeof(char));
        strcpy(filepath, "/home/twidis/conway/init_configs/");
        strcat(filepath, initial_state);
        strcat(filepath, ".txt\0");

        game* info = read_config_file(filepath);
        free(filepath);
        width = info->width;
        height = info->height;

        board = (int**) malloc(sizeof(int*)*height);
        for(row = 0; row < height; row++)
        {
            board[row] = (int*) malloc(sizeof(int) * width);
        }

        for(row = 0; row < height; row++)
        {
            for(col = 0; col < width; col++)
            {
                board[row][col] = info->board[row][col];
            }
        }
    }

    game* new_game = (game*) malloc(sizeof(game));
    new_game->width = width;
    new_game->height = height;
    new_game->board = board;
    //print_board(new_game);
    return new_game;
}

void delete_board(game* board)
{
    //free(*board);
    free(board);
}

// TODO: Stop using i and j
int get_num_neighbors(game* board, int i, int j)
{
    int num_neighbors = 0;

    if(i > 0) { num_neighbors += board->board[i-1][j]; }
    if(j > 0) { num_neighbors += board->board[i][j-1]; }
    if(i < board->height-1) { num_neighbors += board->board[i+1][j]; }
    if(j < board->width-1) { num_neighbors += board->board[i][j+1]; }
    if(i > 0 && j < board->width-1) { num_neighbors += board->board[i-1][j+1]; }
    if(i < board->height-1 && j > 0) { num_neighbors += board->board[i+1][j-1]; }
    if(i < board->height-1 && j < board->width-1) { num_neighbors += board->board[i+1][j+1]; }
    if(i > 0 && j > 0) { num_neighbors += board->board[i-1][j-1]; }

    return num_neighbors;
}

void iterate(game* curr_state, game* next_state)
{
    int row, col, iter;

    for(iter = 0; iter < ITERATIONS; iter++)
    {
        for(row = 0; row < curr_state->height; row++)
        {
            for(col = 0; col < curr_state->width; col++)
            {
                int neighbors = get_num_neighbors(curr_state, row, col);

                if(curr_state->board[row][col] == 1) // alive
                {
                    if(neighbors == 2 || neighbors == 3) { next_state->board[row][col] = 1; }
                    else { next_state->board[row][col] = 0; }
                }
                else // dead
                {
                    if(neighbors == 3) { next_state->board[row][col] = 1; }
                }
            }
        }

        print_board(curr_state);
        delete_board(curr_state);
        curr_state = next_state;
        next_state = initialize_board("zero", curr_state->width, curr_state->height);
        sleep(1);
    }
}

int main(int argc, char* argv[])
{
    game* curr_state;
    game* next_state;

    if(argc != 2)
    {
        curr_state = initialize_board("random", 0, 0);
        next_state = initialize_board("zero", 0, 0);
    }
    else
    {
        curr_state = initialize_board(argv[1], 0, 0);
        next_state = initialize_board("zero", curr_state->width, curr_state->height);
    }

    iterate(curr_state, next_state);

    // I think this has already been freed?
    //delete_board(curr_state);
    //delete_board(next_state);
}

