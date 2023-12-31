#include "hidden_singles.h"
#include <stdlib.h>

// find hidden single values (the value that is unique among all the candidates for a row, a collumn, or a box)
int find_hidden_single_values(Cell **p_cells, int *hidden_single_values){
    int candidate_count[BOARD_SIZE] = {0};
    int hidden_num = 0;
    for(int i = 0; i < BOARD_SIZE; i++ ){
        if(p_cells[i]->num_candidates > 1){
            int* candidates = get_candidates(p_cells[i]);
            for(int j = 0; j < p_cells[i]->num_candidates; j++){
                candidate_count[candidates[j]-1] += 1;
            }
            free(candidates);
        }
    }
    for(int i = 0; i < BOARD_SIZE; i++){
        if (candidate_count[i] == 1){
            hidden_single_values[hidden_num] = i + 1;
            hidden_num++;
        } 
    }
    return hidden_num;
}

int is_in_list_hidden_single(HiddenSingle *p_hidden_singles, int *p_counter, Cell* p_celll){
    for (int i = 0; i < *p_counter; i++){
        if(p_hidden_singles[i].p_cell == p_celll){
            return 0;
            
        }
    }
    return 1;
}

// find hidden single cells in a row, in a collumn or in a box
void find_hidden_single(Cell **p_cells, HiddenSingle *p_hidden_singles, int *p_counter){
    int hidden_value_array[BOARD_SIZE];// array contain hidden values
    int num_of_hidden_value = find_hidden_single_values(p_cells, hidden_value_array); //number of hidden value
    //counting hidden single
    for(int i = 0; i < num_of_hidden_value; i++){
        for(int k = 0; k < BOARD_SIZE; k++){
            if(p_cells[k] -> num_candidates > 1){
                int* candidates = get_candidates(p_cells[k]);// candidates cua o
                for (int j = 0; j < p_cells[k]->num_candidates; j++){
                    if(candidates[j] ==  hidden_value_array[i] && is_in_list_hidden_single(p_hidden_singles, p_counter,p_cells[k])){
                        p_hidden_singles[*p_counter].p_cell = p_cells[k];
                        p_hidden_singles[*p_counter].value = hidden_value_array[i];
                        (*p_counter)++;
                    }
                }
                free(candidates);
            }
        }
    }
}


// find hidden single cells in a board, return the number of hidden single cells
int hidden_singles(SudokuBoard *p_board){
    int hidden_single_count = 0;
    HiddenSingle hidden_single_address[BOARD_SIZE * BOARD_SIZE];
    // Iterate through rows, columns, and boxes
    for (int i = 0; i < BOARD_SIZE; i++) {
        find_hidden_single(p_board->p_rows[i], hidden_single_address, &hidden_single_count);
        find_hidden_single(p_board->p_cols[i], hidden_single_address, &hidden_single_count);
        find_hidden_single(p_board->p_boxes[i], hidden_single_address, &hidden_single_count);
    }
    for(int i = 0; i < hidden_single_count; i++){
        set_candidates(hidden_single_address[i].p_cell, (int*)(int[]){hidden_single_address[i].value}, 1);
    }
    return hidden_single_count;
}
