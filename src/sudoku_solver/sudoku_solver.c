/*
 * =====================================================================================
 *
 *       Filename:  struc_sudoku_solver.c
 *
 *    Description: This file will contain the struct of the game and the
 *    functions to import and create the board to solve.
 *
 *        Version:  1.0
 *        Created:  09/13/2022 07:40:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo.Wattebled
 *   Organization: Fach'OCR
 *
 * =====================================================================================
 */

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/sudoku_solver/sudoku_file_manager.h"

bool line_abscent(int k, int **grid, int i)
{
    for (int j = 0; j < 9; j++)
        if (grid[i][j] == k)
            return false;

    return true;
}

bool col_abscent(int k, int **grid, int j)
{
    for (int i = 0; i < 9; i++)
        if (grid[i][j] == k)
            return false;

    return true;
}

bool block_abscent(int k, int **grid, int i, int j)
{
    int _i = i - (i % 3), _j = j - (j % 3);
    for (i = _i; i < _i + 3; i++)
        for (j = _j; j < _j + 3; j++)
            if (grid[i][j] == k)
                return false;

    return true;
}

bool solve_grid(int **grid, int pos)
{
    if (pos == 9*9)
        return true;

    int i = pos / 9, j = pos % 9;

    if (grid[i][j] != 0)
        return solve_grid(grid, pos+1);

    for (int k=1; k <= 9; k++)
    {
        if (line_abscent(k,grid,i) && col_abscent(k,grid,j) && block_abscent(k,grid,i,j))
        {
            grid[i][j] = k;

            if (solve_grid(grid, pos + 1))
                return true;
        }
    }
    grid[i][j] = 0;

    return false;
}


int main(int argc, char** argv)
{
    if (argc > 2)
        return 1;

    int **grid = (int**)calloc(10, sizeof(int*));
    for (size_t i = 0; i < 9; ++i)
        grid[i] = (int*)calloc(10, sizeof(int));

    //Import a grid from the file named by the user in the argv[1] slot.
    import_grid_block(grid, argv[1]);

    solve_grid(grid, 0);

    //Change the name of the file by adding .result at the end. Then the grid
    //will be exported in the new named file
    char *res = (char *)calloc(strlen(argv[1]) + 8, sizeof(char));
    int i = 0;
    for (; argv[1][i] != '\0'; ++i)
        res[i] = argv[1][i];

    res[i] = '.';
    res[i + 1] = 'r';
    res[i + 2] = 'e';
    res[i + 3] = 's';
    res[i + 4] = 'u';
    res[i + 5] = 'l';
    res[i + 6] = 't';
    res[i + 7] = '\0';
    
    export_grid(grid, res);


    for (size_t i = 0; i < 10; ++i)
        free(grid[i]);
    free(grid);
    free(res);

    return 0;
}





