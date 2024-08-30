#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 12

int list[SIZE] = {23, 12, 1, 0, 45, 66, 34, 9, 11, 56, 775, 2};
int sortedList[SIZE];
typedef struct {
  int start;
  int end;
} Params;

void mergeSort(int arr[], int l, int r);
void printArray(int arr[]);

void *sort(void *args) {
  Params p = *(Params *)args;
  mergeSort(list, p.start, p.end);
  return NULL;
}

void *mergeSorting(void *args) {
  int l, r, k;
  l = 0;
  r = SIZE / 2;
  k = 0;

  while (l < SIZE / 2 && r < SIZE) {
    if (list[l] <= list[r]) {
      sortedList[k] = list[l];
      l++;
    } else {
      sortedList[k] = list[r];
      r++;
    }
    k++;
  }
  while (l < (SIZE / 2)) {
    sortedList[k] = list[l];
    l++;
    k++;
  }

  while (r < SIZE) {
    sortedList[k] = list[r];
    r++;
    k++;
  }
  return NULL;
}

int main() {
  pthread_t sortingThreads[2];
  pthread_t mergeThread;
  int mid = SIZE / 2;

  printf("before : ");
  printArray(list);

  for (int i = 0; i < 2; i++) {
    Params *p = malloc(sizeof(Params));
    if (i == 0) {
      p->start = 0;
      p->end = SIZE / 2;
    } else {
      p->start = SIZE / 2;
      p->end = SIZE - 1;
    }
    pthread_create(&sortingThreads[i], NULL, sort, p);
  }

  for (int i = 0; i < 2; i++) {
    pthread_join(sortingThreads[i], NULL);
  };

  pthread_create(&mergeThread, NULL, mergeSorting, NULL);
  pthread_join(mergeThread, NULL);

  printf("after : ");
  printArray(sortedList);

  return 0;
}

void merge(int arr[], int l, int m, int r) {
  int leftLength, rightLength;

  leftLength = m - l + 1;
  rightLength = r - m;

  int tempLeft[leftLength], tempRight[rightLength];

  for (int i = 0; i < rightLength; i++)
    tempRight[i] = arr[m + i + 1];

  for (int i = 0; i < leftLength; i++)
    tempLeft[i] = arr[l + i];

  int i, j, k;
  i = j = 0;
  k = l;

  while (i < leftLength && j < rightLength) {
    if (tempLeft[i] <= tempRight[j]) {
      arr[k] = tempLeft[i];
      i++;
    } else {
      arr[k] = tempRight[j];
      j++;
    }
    k++;
  }

  while (i < leftLength) {
    arr[k] = tempLeft[i];
    k++;
    i++;
  }
  while (j < rightLength) {
    arr[k] = tempRight[j];
    k++;
    j++;
  }
}

void mergeSort(int arr[], int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
  }
}

void printArray(int arr[]) {
  for (int i = 0; i < SIZE; i++)
    printf("[%d]", arr[i]);
  printf("\n");
}
