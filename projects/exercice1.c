#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100000000

int table[SIZE] = {0};

void *manipulerTable(void *args) {
  int start = *(int *)args;

  printf("the start is %d\n", start);

  for (int i = start; i < SIZE; i += 2) {
    table[i]++;
  }

  return NULL;
}

void printArray(int *arr, int size);

int main() {
  clock_t begin = clock();

  pthread_t thread1;
  pthread_t thread2;

  int start1 = 0;
  int start2 = 1;
  // printArray(table, SIZE);
  pthread_create(&thread1, NULL, manipulerTable, &start1);
  pthread_create(&thread2, NULL, manipulerTable, &start2);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  // printArray(table, SIZE);

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
