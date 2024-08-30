#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 12

// Global array to be sorted
int list[SIZE] = {23, 12, 1, 0, 45, 66, 34, 9, 11, 56, 775, 2};

// Array to store the final sorted output
int sortedList[SIZE];

// Structure to hold parameters for thread functions
typedef struct {
  int start; // Starting index of the subarray
  int end;   // Ending index of the subarray
} Params;

void mergeSort(int arr[], int l, int r);
void printArray(int arr[]);

// Function to merge two sorted subarrays
void *mergeTwoSortedArray(void *args);

// Function to sort a subarray using merge sort
void *sort(void *args);

int main() {
  pthread_t sortingThreads[2]; // Threads for sorting two halves
  pthread_t mergeThread;       // Thread for merging two sorted halves
  int mid = SIZE / 2;

  // Print the array before sorting
  printf("before : ");
  printArray(list);

  // Create threads to sort two halves of the array
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

  // Wait for the sorting threads to finish
  for (int i = 0; i < 2; i++) {
    pthread_join(sortingThreads[i], NULL);
  };

  // Create a thread to merge the two sorted halves
  pthread_create(&mergeThread, NULL, mergeTwoSortedArray, NULL);
  pthread_join(mergeThread, NULL);

  // Print the array after sorting and merging
  printf("after : ");
  printArray(sortedList);

  return 0;
}

// Function to merge two subarrays of arr[] into a single sorted array
void merge(int arr[], int l, int m, int r) {
  int leftLength, rightLength;

  leftLength = m - l + 1; // Length of the left subarray
  rightLength = r - m;    // Length of the right subarray

  int tempLeft[leftLength], tempRight[rightLength];

  // Copy data to temporary subarrays tempLeft[] and tempRight[]
  for (int i = 0; i < rightLength; i++)
    tempRight[i] = arr[m + i + 1];

  for (int i = 0; i < leftLength; i++)
    tempLeft[i] = arr[l + i];

  int i, j, k;
  i = j = 0;
  k = l;

  // Merge the temp arrays back into arr[l..r]
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

  // Copy the remaining elements of tempLeft[], if any
  while (i < leftLength) {
    arr[k] = tempLeft[i];
    k++;
    i++;
  }

  // Copy the remaining elements of tempRight[], if any
  while (j < rightLength) {
    arr[k] = tempRight[j];
    k++;
    j++;
  }
}

// Recursive function to implement merge sort on array arr[]
void mergeSort(int arr[], int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;

    // Sort first and second halves
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    // Merge the sorted halves
    merge(arr, left, mid, right);
  }
}

// Function to print an array
void printArray(int arr[]) {
  for (int i = 0; i < SIZE; i++)
    printf("[%d]", arr[i]);
  printf("\n");
}

// Function to merge two sorted halves of the list into sortedList
void *mergeTwoSortedArray(void *args) {
  int l, r, k;
  l = 0;
  r = SIZE / 2;
  k = 0;

  // Merge the two halves into sortedList
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

  // Copy any remaining elements from the left half
  while (l < (SIZE / 2)) {
    sortedList[k] = list[l];
    l++;
    k++;
  }

  // Copy any remaining elements from the right half
  while (r < SIZE) {
    sortedList[k] = list[r];
    r++;
    k++;
  }
  return NULL;
}

// Thread function to sort a portion of the array using mergeSort
void *sort(void *args) {
  Params p = *(Params *)args;
  mergeSort(list, p.start, p.end);
  return NULL;
}
