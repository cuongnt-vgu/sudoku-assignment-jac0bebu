#include "naked_triples.h"
#include <stdlib.h>
#include <stdio.h>

int is_in_triples(int triples[3], int value)
{
    for (int i = 0; i < 3; i++)
    {
        if (triples[i] == value) return 0;
    }
    return 1;
}

int check_naked_triples(Cell **p_cells, int triples_array[], int *index) {
    int cell_counter = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (!(p_cells[i]->num_candidates == 1 || p_cells[i]->num_candidates > 3)) {
            int* candidates = get_candidates(p_cells[i]);
            int candidate_flag = 1;
            for (int j = 0; j < p_cells[i]->num_candidates; j++) {
                if (is_in_triples(triples_array, candidates[j])) {
                    candidate_flag = 0;
                    break;
                }
            }
            free(candidates);

            if (candidate_flag) {
                index[cell_counter++] = i;
                if (cell_counter > 3) return 0; // More than 3 cells found, not a valid naked triple
            }
        }
    }
    return (cell_counter == 3); // Return true if exactly 3 cells with valid candidates are found
}

void find_naked_triples(Cell **p_cells, NakedTriples *p_naked_triples, int *p_counter)
{
    int all_candidate[BOARD_SIZE] = {0};

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        int* candidates = get_candidates(p_cells[i]);
        for (int j = 0; j < p_cells[i]->num_candidates; j++)
        {
            all_candidate[candidates[j]-1] += 1;
        }
        free(candidates);
    }

    int triples_array[BOARD_SIZE];
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (all_candidate[i] >= 2)
        {
            triples_array[count] = i+1;
            count++;
        }
    }

    if (count <= 2) return;

    for (int i = 0; i < count - 2; i++) {
    for (int j = i + 1; j < count - 1; j++) {
        for (int k = j + 1; k < count; k++) {
            int value[3] = {triples_array[i], triples_array[j], triples_array[k]};
            int index[BOARD_SIZE];
            if (check_naked_triples(p_cells, value, index)) {
                p_naked_triples[(*p_counter)++] = (NakedTriples){
                    p_cells,
                    {index[0], index[1], index[2]},
                    {triples_array[i], triples_array[j], triples_array[k]}
                };
            }
        }
    }
}
}

void eliminate_naked_triples(Cell **p_cells, NakedTriples *naked_triples, int total_naked_triples) {
    NakedTriples solved_triples[total_naked_triples];
    int solved_counter = 0;
    int offset = 0;

    for (int i = 0; i < total_naked_triples; i++) {
        if (check_solved_triples(naked_triples[i], solved_triples, solved_counter))
            offset++;
        else
            solved_triples[solved_counter++] = naked_triples[i];

        Cell **naked_cells = naked_triples[i].p_cells;
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (j != naked_triples[i].index[0] && j != naked_triples[i].index[1] && j != naked_triples[i].index[2]) {
                for (int k = 0; k < 3; k++) {
                    if (is_candidate(naked_cells[j], naked_triples[i].value[k])) {
                        unset_candidate(naked_cells[j], naked_triples[i].value[k]);
                    }
                }
            }
        }
    }
}

int check_solved_triples(NakedTriples Naked_Triples_Address, NakedTriples *solved_triples, int solved_counter)
{
    Cell **p_cells = Naked_Triples_Address.p_cells;
    for (int i = 0; i < solved_counter; i++)
    {
        Cell **solved_cells = solved_triples[i].p_cells;
        if (p_cells[Naked_Triples_Address.index[0]] == solved_cells[solved_triples[i].index[0]] &&
            p_cells[Naked_Triples_Address.index[1]] == solved_cells[solved_triples[i].index[1]] &&
            p_cells[Naked_Triples_Address.index[2]] == solved_cells[solved_triples[i].index[2]])
        {
            return 1;
        }
    }
    return 0;
}

int naked_triples(SudokuBoard *p_board)
{
    int total_naked_triples = 0;

    NakedTriples Naked_Triples_Address[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_naked_triples(p_board->p_rows[i], Naked_Triples_Address, &total_naked_triples);
        find_naked_triples(p_board->p_cols[i], Naked_Triples_Address, &total_naked_triples);
        find_naked_triples(p_board->p_boxes[i], Naked_Triples_Address, &total_naked_triples);
    }

    NakedTriples solved_triples[total_naked_triples];
    int solved= 0;
    int offset = 0;

    for (int i = 0; i < total_naked_triples; i++)
    {
        if (check_solved_triples(Naked_Triples_Address[i], solved_triples, solved)) offset++;
        else solved_triples[solved++] = Naked_Triples_Address[i];

        Cell** p_cells = Naked_Triples_Address[i].p_cells;
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (j != Naked_Triples_Address[i].index[0] && j != Naked_Triples_Address[i].index[1] && j != Naked_Triples_Address[i].index[2])
            {
                for (int k = 0; k < 3; k++)
                {
                    if (is_candidate(p_cells[j], Naked_Triples_Address[i].value[k]))
                    {
                        unset_candidate(p_cells[j], Naked_Triples_Address[i].value[k]);
                    }
                }
            }
        }

    }
    return total_naked_triples - offset;
}