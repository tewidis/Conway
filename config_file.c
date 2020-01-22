#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "game.h"
#include "conway.h"

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

