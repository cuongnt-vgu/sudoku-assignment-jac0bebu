#include "hidden_pairs.h"
#include "sudoku.h"
#include <stdlib.h>

bool is_hidden_pair(Cell **p_cells, int value1, int value2){
    int count = 0;
    for (int i =0; i < BOARD_SIZE; i++){
        if(is_candidate(p_cells[i], value1) && is_candidate(p_cells[i], value2)){
            count ++;
        }
    }
    return (count ==2);
}


bool is_in_cell(Cell *p_cell, int value1, int value2){
    return (is_candidate(p_cell, value1) && is_candidate(p_cell, value2));
}

int find_hidden_double_values(Cell **p_cells, int *hidden_double_values)
{
    int hidden_double_count = 0;
    int hidden_double_array[BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++){
        hidden_double_array[i] = 0;
    }
    for (int i = 0; i < BOARD_SIZE; i++){
        if (p_cells[i]->num_candidates >= 2){
            int* candidate_array = get_candidates(p_cells[i]); 
            for (int j = 0; j < p_cells[i]->num_candidates; j++){
                hidden_double_array[candidate_array[j]-1] += 1; 
            }
            free(candidate_array);
        }
    }
    for (int i = 0; i < BOARD_SIZE; i++){
        if (hidden_double_array[i] == 2){
                hidden_double_values[hidden_double_count++] = i+1;                  
        }
    }
    return hidden_double_count;
}



void find_hidden_double(Cell **p_cells, HiddenDouble *Hidden_pair_address,
                        int *count)
{       
      int hidden_double_value_array[BOARD_SIZE];
      int num_value = find_hidden_double_values(p_cells, hidden_double_value_array);

      for (int i = 0; i < num_value-1; i++){
            for (int j = i+1; j < num_value; j++){
                if(is_hidden_pair(p_cells, hidden_double_value_array[i], hidden_double_value_array[j])){
                    for (int k = 0; k < BOARD_SIZE; k++){
                        if (is_in_cell(p_cells[k], hidden_double_value_array[i], hidden_double_value_array[j])){
                        HiddenDouble hidden_double_object;
                        hidden_double_object.value1 = hidden_double_value_array[i];
                        hidden_double_object.value2 = hidden_double_value_array[j];
                        hidden_double_object.p_cell = p_cells[k];
                        Hidden_pair_address[(*count)++] = hidden_double_object;
                        }                    
                    }
                }
            }
      }

}

int hidden_pairs(SudokuBoard *p_board)
{
    HiddenDouble Hidden_pair_address[BOARD_SIZE*BOARD_SIZE];
    int count = 0;
    for (int i = 0; i< BOARD_SIZE; i++){
        find_hidden_double(p_board->p_cols[i], Hidden_pair_address, &count);
        find_hidden_double(p_board->p_rows[i], Hidden_pair_address, &count);
        find_hidden_double(p_board->p_boxes[i], Hidden_pair_address, &count);
    }
    int overlap = count;
    for (int i = 0; i < count; i++){
        int not_overlap = 0;
        Cell *p_cell = Hidden_pair_address[i].p_cell;
        int* candidate_array = get_candidates(p_cell);
        int num_candidates = p_cell->num_candidates;
        for (int j = 0; j < num_candidates; j++){
            if ((candidate_array[j]!= Hidden_pair_address[i].value1)&&(candidate_array[j]!=Hidden_pair_address[i].value2)){
                unset_candidate(p_cell, candidate_array[j]);
                not_overlap = 1;
            }
        }
        overlap -= not_overlap;
        free(candidate_array);

    }

    return (count - overlap)/2;
}
