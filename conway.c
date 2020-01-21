#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <locale.h>

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
    setlocale(LC_CTYPE, "");
    wchar_t solid_box = 0x25A0;
    wchar_t empty_box = 0x25A1;
    int row, col;
    for(row = 0; row < board->height; row++)
    {
        for(col = 0; col < board->width; col++)
        {
            if(board->board[row][col]) { wprintf(L"%lc", solid_box); }
            else{ wprintf(L"%lc", empty_box); }
        }
        wprintf(L"\n");
    }
    wprintf(L"\n");
}

game* read_config_file(char* initial_state)
{
    int total_length = strlen("/home/twidis/conway/init_configs/.txt") + strlen(initial_state) + 1;
    char* filename = (char*) malloc(total_length * sizeof(char));
    strcpy(filename, "/home/twidis/conway/init_configs/");
    strcat(filename, initial_state);
    strcat(filename, ".txt\0");

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

    free(filename);
    return file;
}

game* initialize_board(char* initial_state, int width, int height)
{
    int row, col;
    int** board;
    game* info;

    if(strcmp(initial_state, "random") != 0 && strcmp(initial_state, "zero") != 0)
    {
        info = read_config_file(initial_state);
        width = info->width;
        height = info->height;
    }
    if(width == 0) { width = WIDTH; }
    if(height == 0) { height = HEIGHT; }

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
            if(strcmp(initial_state, "random") == 0) { board[row][col] = rand() % 2; }
            else if(strcmp(initial_state, "zero") == 0) { board[row][col] = 0; }
            else { board[row][col] = info->board[row][col]; }
        }
    }

    game* new_game = (game*) malloc(sizeof(game));
    new_game->width = width;
    new_game->height = height;
    new_game->board = board;
    return new_game;
}

void delete_board(game* board)
{
    free(board);
}

int get_num_neighbors(game* board, int row, int col)
{
    int num_neighbors = 0;

    if(row > 0) { num_neighbors += board->board[row-1][col]; }
    if(col > 0) { num_neighbors += board->board[row][col-1]; }
    if(row < board->height-1) { num_neighbors += board->board[row+1][col]; }
    if(col < board->width-1) { num_neighbors += board->board[row][col+1]; }
    if(row > 0 && col < board->width-1) { num_neighbors += board->board[row-1][col+1]; }
    if(row < board->height-1 && col > 0) { num_neighbors += board->board[row+1][col-1]; }
    if(row < board->height-1 && col < board->width-1) { num_neighbors += board->board[row+1][col+1]; }
    if(row > 0 && col > 0) { num_neighbors += board->board[row-1][col-1]; }

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

