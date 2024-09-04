#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

sem_t teacherRom;

void *teacherAssistent(void *args) {
  sem_wait(&teacherRom);
  int *n = (int *)malloc(sizeof(int));
  n = (int *)args;

  printf("the number of student in the rom  is %d \n", *n);
  free(n);
  sleep(2);
  sem_post(&teacherRom);
  return NULL;
}

int main() {
  sem_init(&teacherRom, 0, 1);
  pthread_t student[4];

  for (int i = 0; i < 4; i++) {
    int *n = (int *)malloc(sizeof(int));
    *n = i;
    pthread_create(student + i, NULL, teacherAssistent, n);
  }

  for (int i = 0; i < 4; i++) {
    pthread_join(student[i], NULL);
  }

  sem_destroy(&teacherRom);
  return 0;
}
