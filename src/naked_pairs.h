#pragma once

#include "sudoku.h"

int naked_pairs(SudokuBoard *p_board);

typedef struct implementNakedPair{
    Cell *p_cell[2];
} NakedPair;


