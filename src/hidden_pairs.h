#pragma once

#include "sudoku.h"

int hidden_pairs(SudokuBoard *p_board);

typedef struct hidden_pairs_implement
{
    Cell* p_cell;
    int value1;
    int value2;
}HiddenDouble;

bool is_candidate(Cell *cell, int value);