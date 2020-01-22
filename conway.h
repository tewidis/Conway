#ifndef CONWAY_H_
#define CONWAY_H_

#include "game.h"

int* convert(char* c);

void print_board(game* board);

game* read_config_file(char* initial_state);

game* initialize_board(char* initial_state, int width, int height, int iterations);

void delete_board(game* board);

int get_num_neighbors(game* board, int row, int col);

void iterate(game* curr_state, game* next_state);

#endif
