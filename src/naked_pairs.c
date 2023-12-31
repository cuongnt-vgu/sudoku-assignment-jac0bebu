
#include "naked_pairs.h"
#include <stdlib.h>


void eliminate_naked_pair(Cell **p_cell){
    for (int i = 0; i < BOARD_SIZE; i ++){
        if (p_cell[i]->num_candidates == 2) {
            int *candidates_i = get_candidates(p_cell[i]);

            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (p_cell[j]->num_candidates == 2 && i != j) {
                    int *candidates_j = get_candidates(p_cell[j]);

                    if (candidates_i[0] == candidates_j[0] && candidates_i[1] == candidates_j[1]){
                        for (int k = 0; k < BOARD_SIZE; k++){
                            if (k != i && k !=j && p_cell[k]->num_candidates > 1){
                                for (int l = 0; l < BOARD_SIZE; l++){
                                    if (p_cell[k]->candidates[l] == 1 && (l + 1 == candidates_i[0] || l + 1 == candidates_i[1])){
                                        unset_candidate(p_cell[k], l + 1);
                                    }
                                }
                            }
                        }    
                    }
                    free(candidates_j);
                }                
            }
            free(candidates_i);
        }
    }
}
void find_naked_pair(Cell **p_cell, int *p_counter, NakedPair *naked_pairs_address) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (p_cell[i]->num_candidates == 2) {
            int *candidates_i = get_candidates(p_cell[i]);

            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (p_cell[j]->num_candidates == 2 && i != j) {
                    int *candidates_j = get_candidates(p_cell[j]);

                    if (candidates_i[0] == candidates_j[0] && candidates_i[1] == candidates_j[1]) {
                        // Check if the naked pair already exists
                        int is_duplicate = 0;
                        for (int k = 0; k < *p_counter; ++k) {
                            if ((naked_pairs_address[k].p_cell[0] == p_cell[i] && naked_pairs_address[k].p_cell[1] == p_cell[j]) ||
                                (naked_pairs_address[k].p_cell[0] == p_cell[j] && naked_pairs_address[k].p_cell[1] == p_cell[i])) {
                                is_duplicate = 1;
                                break;
                            }
                        }

                        if (!is_duplicate) {
                            naked_pairs_address[*p_counter].p_cell[0] = p_cell[i];
                            naked_pairs_address[*p_counter].p_cell[1] = p_cell[j];
                            
                            // Increment naked pair count
                            (*p_counter)++;
                        }
                    }
                    free(candidates_j);
                }
            }
            free(candidates_i);
        }
    }
}

// Function to find and eliminate naked pairs within rows, columns, and boxes of a Sudoku board
int naked_pairs(SudokuBoard *p_board) {
    int total_naked_pairs = 0;
    NakedPair Naked_Pair_Address[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; ++i) {
        // Check rows
        find_naked_pair(p_board->p_rows[i], &total_naked_pairs, Naked_Pair_Address);

        // Check columns
        find_naked_pair(p_board->p_cols[i], &total_naked_pairs, Naked_Pair_Address);

        // Check boxes
        find_naked_pair(p_board->p_boxes[i], &total_naked_pairs, Naked_Pair_Address);
    }
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            eliminate_naked_pair(p_board->p_rows[i]);
            eliminate_naked_pair(p_board->p_cols[i]);
            eliminate_naked_pair(p_board->p_boxes[i]);
        }

    }
    return total_naked_pairs;
}