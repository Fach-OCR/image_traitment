/*
 * =====================================================================================
 *
 *       Filename:  sudoku_file_manager.c
 *
 *    Description: This file will manage the reading and writing part
 *    of a txt file in order to import and export a sudoku grid.
 *
 *        Version:  1.0
 *        Created:  09/20/2022 09:42:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo Wattebled
 *   Organization:  Fach'OCR
 *
 * =====================================================================================
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**@brief Import a grid from a text file. The grid in the file will be the same
 * as the one in the cahier des charges.
 *
 * @param grid The sudoku grid to modify in place
 * @param filename The name of the file to import
 */
void import_grid_block(int **grid, char filename[]) {
  // Open the file of the argv (ie: the user input)
  FILE *fp = fopen(filename, "r");

  if (fp == NULL)
    printf("file can't be opened \n");

  char line[25];

  size_t bias_hori = 0;
  size_t j = 0;
  while (fgets(line, sizeof(line), fp)) {
    if (line[0] == '\n' || line[0] == '\r' ||
        line[0] == ' ') // To avoid the 2 horizontal lines on the grid file
      bias_hori++;
    else {
      size_t bias_verti = 0;
      for (size_t i = 0; strcmp(&line[i], "\n") == 0; i++) {
        if (line[i] == ' ') {
          // To avoid the 2 spaces on the vertical lines
          bias_verti++;
        } else if (line[i] == '.') {
          // Put a 0 because a dot is an empty case and
          // substract by the 2 bias to stay
          // in the bound of the matrix
          grid[j - bias_hori][i - bias_verti] = 0;
        } else {
          // Put the number of the file in the matrix. I'm using
          // -'0' because it converts the char to int
          grid[j - bias_hori][i - bias_verti] = line[i] - '0';
        }
      }
    }
    j++;
  }

  fclose(fp);
}

/**@brief Export the solved grid in a file.
 *
 * @param grid The sudoku grid to modify in place
 * @param filename The name of the file to import
 */
void export_grid(int **grid, char filename[]) {
  FILE *fp;
  fp = fopen(filename, "w");

  for (size_t i = 0; i < 9; i++) {
    for (size_t j = 0; j < 9; j++) {
      if (j == 2 || j == 5) {
        fputc(grid[i][j] + '0', fp);
        fputc(' ', fp);
      } else
        fputc(grid[i][j] + '0', fp);
    }
    fputc('\n', fp);
    if (i == 2 || i == 5)
      fputc('\n', fp);
  }

  fclose(fp);
}
