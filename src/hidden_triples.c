#include "hidden_triples.h"
#include <stdlib.h>
#include <stdio.h>

bool is_in_list_hidden_triples(HiddenTriples *p_array, int size, Cell *p1, Cell *p2, Cell *p3)
{
    for (int i = 0; i < size; i++)
    {
        if ((p_array[i].cell1 == p1) && (p_array[i].cell2 == p2) && (p_array[i].cell3 == p3))
        {
            return true;
        }
    }
    return false;
}

void find_and_store_hidden_triples(Cell **p_cells, HiddenTriples *p_hidden_triples, int *p_counter)
{
    int candidate_counter[BOARD_SIZE] = {0};

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        int* candidates = get_candidates(p_cells[i]);
        for (int j = 0; j < p_cells[i]->num_candidates; j++)
        {
            candidate_counter[candidates[j]-1]++;
        }
        free(candidates);
    }

    int triple_sets[BOARD_SIZE];
    int triple_sets_count = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (candidate_counter[i] == 2 || candidate_counter[i] == 3)
        {
            triple_sets[triple_sets_count++] = i+1;
        }
    }
    if (triple_sets_count < 3) return;

    for (int i = 0; i < triple_sets_count - 2; i++)
    {
        for (int j = i+1; j < triple_sets_count - 1; j++)
        {
            for (int k = j+1; k < triple_sets_count; k++)
            {
                int set[3] = {triple_sets[i], triple_sets[j], triple_sets[k]};
                int indices[BOARD_SIZE];
                int cell_counter = 0;

                for (int idx = 0; idx < BOARD_SIZE; idx++)
                {
                    if (p_cells[idx]->num_candidates == 1) continue;
                    else
                    {
                        for (int x = 0; x < 3; x++)
                        {
                            if (is_candidate(p_cells[idx], set[x]))
                            {
                                indices[cell_counter++] = idx;
                                break;
                            }
                        }
                    }
                }

                if (cell_counter == 3)
                {
                    if (p_cells[indices[0]]->num_candidates <=3 && p_cells[indices[1]]->num_candidates <=3 && p_cells[indices[2]]->num_candidates <=3) continue;
                    if(!is_in_list_hidden_triples(p_hidden_triples, *p_counter, p_cells[indices[0]], p_cells[indices[1]], p_cells[indices[2]]))
                    {
                        p_hidden_triples[*p_counter].cell1 = p_cells[indices[0]];
                        p_hidden_triples[*p_counter].cell2 = p_cells[indices[1]];
                        p_hidden_triples[*p_counter].cell3 = p_cells[indices[2]];
                        p_hidden_triples[*p_counter].num_1 = set[0];
                        p_hidden_triples[*p_counter].num_2 = set[1];
                        p_hidden_triples[*p_counter].num_3 = set[2];
                        (*p_counter)++;
                    }
                }
            }
        }
    }
}

int hidden_triples(SudokuBoard *p_board)
{
    int counter = 0;

    HiddenTriples p_hidden_triples[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_and_store_hidden_triples(p_board->p_rows[i], p_hidden_triples, &counter);
        find_and_store_hidden_triples(p_board->p_cols[i], p_hidden_triples, &counter);
        find_and_store_hidden_triples(p_board->p_boxes[i], p_hidden_triples, &counter);
    }
    
    for (int i = 0; i < counter; i++)
    {
        int num_1 = p_hidden_triples[i].num_1;
        int num_2 = p_hidden_triples[i].num_2;
        int num_3 = p_hidden_triples[i].num_3;
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (j != num_1-1 && j != num_2-1 && j != num_3-1)
            {
                if (is_candidate(p_hidden_triples[i].cell1, j+1)){
                    unset_candidate(p_hidden_triples[i].cell1, j+1);
                }
                if (is_candidate(p_hidden_triples[i].cell2, j+1)){
                    unset_candidate(p_hidden_triples[i].cell2, j+1);
                }
                if (is_candidate(p_hidden_triples[i].cell3, j+1)){
                    unset_candidate(p_hidden_triples[i].cell3, j+1);
                }
            }         
        }
    }

    return counter;
}
