#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10

int arr[SIZE] = {23, -5, 7, 90, -5, -12, 6, 0, 12, -66};
void *displayTable(void *args) {
  char *s = (char *)args;

  if (strcmp(s, "positive") == 0) {
    for (int i = 0; i < SIZE; i++) {
      if (arr[i] >= 0)
        printf("%s %d \n", s, arr[i]);
    }
  } else {
    for (int i = 0; i < SIZE; i++) {
      if (arr[i] < 0)
        printf("%s %d\n", s, arr[i]);
    }
  }
  return NULL;
}
int main() {

  pthread_t positiveThread, negativeThread;

  pthread_create(&positiveThread, NULL, displayTable, "positive");
  pthread_create(&negativeThread, NULL, displayTable, "negative");

  pthread_join(positiveThread, NULL);
  pthread_join(negativeThread, NULL);

  return 0;
}
