#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 9

typedef struct {
  int col;
  int row;
} Parametres;

int sudoku[SIZE][SIZE] = {
    {5, 1, 4, 6, 7, 8, 9, 1, 2}, {6, 7, 2, 1, 9, 5, 3, 4, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 7}, {8, 5, 9, 7, 6, 1, 4, 2, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1}, {7, 1, 3, 9, 2, 4, 8, 5, 6},
    {9, 6, 1, 5, 3, 7, 2, 8, 4}, {2, 8, 7, 4, 1, 9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 7, 9}};

void *validateRow(void *args) {
  int *result = malloc(sizeof(int));
  int row = *(int *)args;
  int j;
  *result = 1;

  for (int i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++)
      if (sudoku[row][j] == i + 1)
        break;

    if (j == 9) {
      *result = 0;
      break;
    }
  }

  return (void *)result;
}

void *validateColumns(void *args) {
  int *result = malloc(sizeof(int));
  int col = *(int *)args;
  int increment = 0;
  *result = 1;

  int j;
  for (int i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++)
      if (sudoku[j][col] == i + 1)
        break;

    if (j >= 9) {
      *result = 0;
      break;
    }
  }

  return (void *)result;
}

void *validateGrid(void *args) {
  Parametres *params;
  params = (Parametres *)malloc(sizeof(Parametres));
  params = (Parametres *)args;
  int *result = malloc(sizeof(int));
  int colStart = params->col;
  int rowStart = params->row;

  int nums[10] = {0};
  *result = 1;

  for (int row = 0; row < 3; row++) {
    bool isValid = true;
    for (int col = 0; col < 3; col++) {
      int value = sudoku[rowStart + row][colStart + col];
      nums[value]++;
      if (nums[value] > 1) {
        *result = 0;
        break;
      }
    }
    if (result == 0)
      break;
  }

  return (void *)result;
}

int main() {
  int result[27];
  pthread_t threads[18];
  pthread_t gridThreads[9];
  int k = 0;

  // create row and column threads
  for (int i = 0; i < 9; i++) {
    int *value = malloc(sizeof(int));
    *value = i;
    pthread_create(&threads[i], NULL, validateRow, value);
    pthread_create(&threads[i + 9], NULL, validateColumns, value);
  }

  // create grid threads
  for (int i = 0; i < 9; i += 3) {
    for (int j = 0; j < 9; j += 3) {
      Parametres *params;
      params = (Parametres *)malloc(sizeof(Parametres));
      params->row = i;
      params->col = j;
      int index = k++;
      pthread_create(&gridThreads[index], NULL, validateGrid, params);
    }
  }

  for (int i = 0; i < 9; i++) {
    printf("hello %d\n", i);
    int *res = malloc(sizeof(int));
    int *rowRes = malloc(sizeof(int));
    int *colRes = malloc(sizeof(int));
    pthread_join(threads[i], (void **)&rowRes);
    pthread_join(threads[i + 9], (void **)&colRes);
    pthread_join(gridThreads[i], (void **)&res);

    printf(" grid : %d \n", *res);
    printf(" row  : %d \n", *rowRes);
    printf(" col  : %d \n", *rowRes);
    if (*rowRes == 0 || *rowRes == 0 || *res == 0) {
      printf("invalid soduko");
      free(rowRes);
      free(colRes);
      return 1;
    }
    free(rowRes);
    free(res);
    free(colRes);
  }
  printf("valid soduko \n");
  return 0;
}
