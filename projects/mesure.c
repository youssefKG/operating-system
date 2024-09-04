
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000000000

int table[SIZE] = {0};
void printArray(int *arr, int size);

int main() {
  clock_t begin = clock();

  pthread_t thread1;
  pthread_t thread2;

  for (int i = 0; i < SIZE; i += 1) {
    table[i]++;
  }

  clock_t end = clock();

  double time_spent = (double)(end - begin);

  printf("the time is spent in this program %lf \n", time_spent);

  return 0;
}

void printArray(int *arr, int size) {
  for (int i = 0; i < size; i++) {
    printf("[%d]", arr[i]);
  }
  printf("\n");
}
