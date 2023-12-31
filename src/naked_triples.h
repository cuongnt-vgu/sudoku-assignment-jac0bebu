#pragma once

#include "sudoku.h"

typedef struct NakedTriples_impl
{
    Cell **p_cells;
    int index[3];
    int value[3];
} NakedTriples;

int is_in_triples(int triples[3], int value);
int check_naked_triples(Cell **p_cells, int triples_array[], int *index);
void find_naked_triples(Cell **p_cells, NakedTriples *p_naked_triples, int *p_counter);
int check_solved_triples(NakedTriples Naked_Triples_Address, NakedTriples *solved_triples, int solved_counter);
int naked_triples(SudokuBoard *p_board);
bool is_candidate(Cell *cell, int value);