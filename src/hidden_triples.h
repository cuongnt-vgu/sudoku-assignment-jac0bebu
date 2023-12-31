#pragma once

#include "sudoku.h"

typedef struct {
    Cell *cell1;
    Cell *cell2;
    Cell *cell3;
    int num_1;
    int num_2;
    int num_3;
} HiddenTriples;

bool is_in_list_hidden_triples(HiddenTriples *p_array, int size, Cell *p1, Cell *p2, Cell *p3);
int is_hidden_triples(Cell **p_cells, int triple_sets[], int *indices);
void find_hidden_triples(Cell **p_cells, HiddenTriples *p_hidden_triples, int *p_counter);
int hidden_triples(SudokuBoard *p_board);
bool is_candidate(Cell *cell, int value);