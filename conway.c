#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <locale.h>

#include "game.h"
#include "config_file.h"
#include "conway.h"

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

game* initialize_board(char* initial_state, int width, int height, int iterations)
{
    int row, col;
    int** board;
    game* info;
    time_t t;
    srand((unsigned) time(&t));

    if(strcmp(initial_state, "random") != 0 && strcmp(initial_state, "zero") != 0)
    {
        info = read_config_file(initial_state);
        width = info->width;
        height = info->height;
    }

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

    if(strcmp(initial_state, "random") != 0 && strcmp(initial_state, "zero") != 0)
    {
        delete_board(info);
    }

    game* new_game = (game*) malloc(sizeof(game));
    new_game->width = width;
    new_game->height = height;
    new_game->board = board;
    new_game->iterations = iterations;
    return new_game;
}

void delete_board(game* del_game)
{
    int row;
    for(row = 0; row < del_game->height; row++)
        free(del_game->board[row]);

    free(del_game->board);
    free(del_game);
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

    for(iter = 0; iter < curr_state->iterations; iter++)
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
        next_state = initialize_board("zero", curr_state->width, curr_state->height, curr_state->iterations);
        sleep(1);
    }
    delete_board(next_state);
}

int main(int argc, char* argv[])
{
    game* curr_state;
    game* next_state;

    switch(argc)
    {
        case 3:
            if( (strcmp(argv[1], "block") == 0) ||
                (strcmp(argv[1], "beehive") == 0) ||
                (strcmp(argv[1], "loaf") == 0) ||
                (strcmp(argv[1], "boat") == 0) ||
                (strcmp(argv[1], "tub") == 0) ||
                (strcmp(argv[1], "blinker") == 0) ||
                (strcmp(argv[1], "beacon") == 0) ||
                (strcmp(argv[1], "pulsar") == 0) ||
                (strcmp(argv[1], "pentadecathlon") == 0) ||
                (strcmp(argv[1], "toad") == 0) ||
                (strcmp(argv[1], "rpentomino") == 0) ||
                (strcmp(argv[1], "acorn") == 0) ||
                (strcmp(argv[1], "diehard") == 0) ||
                (strcmp(argv[1], "glider") == 0) ||
                (strcmp(argv[1], "lwss") == 0) ||
                (strcmp(argv[1], "mwss") == 0) ||
                (strcmp(argv[1], "hwss") == 0) ||
                (strcmp(argv[1], "simkin_glider_gun") == 0) ||
                (strcmp(argv[1], "gosper_glider_gun") == 0) )
                {
                    int iterations = atoi(argv[2]);
                    curr_state = initialize_board(argv[1], 0, 0, iterations);
                    next_state = initialize_board("zero", curr_state->width, curr_state->height, curr_state->iterations);
                }
            else { puts(argv[1]); fprintf(stderr, "Invalid configuration string\n"); return(-1); }
            break;
        case 5:
            if(strcmp(argv[1], "random") == 0)
            {
                int width = atoi(argv[2]);
                int height = atoi(argv[3]);
                int iterations = atoi(argv[4]);
                curr_state = initialize_board("random", width, height, iterations);
                next_state = initialize_board("zero", width, height, iterations);
            }
            else { fprintf(stderr, "Invalid configuration string\n"); return(-1); }
            break;
        default:
            fprintf(stderr, "Invalid number of arguments\n");
            return(-1);
    }

    iterate(curr_state, next_state);

    delete_board(next_state);
}

