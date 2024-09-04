#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define STUDENT_NUM 10

int availableChairs = 3;
int countStudent = 0;

sem_t chairs_sem;
sem_t t_a_sem;
pthread_mutex_t roomMutex;

void *teacherRoutine(void *args) {
  while (1) {
    if (countStudent == STUDENT_NUM - 1) {
      break;
    }
    if (availableChairs != 0) {
      printf("start teaching student \n");
      sem_wait(&t_a_sem);
    }
  }
  return NULL;
}

void *studentRoutine(void *args) {

  // student number
  int *n = malloc(sizeof(int));
  n = (int *)args;
  int timeStudying = rand() % 5 + 1;

  // check if the chairs are available if not wait
  sem_wait(&chairs_sem);
  pthread_mutex_lock(&roomMutex);

  availableChairs++;
  printf("the student number %d start studing programing for %d "
         "seconds \n",
         *n, timeStudying);

  sem_post(&t_a_sem);

  sleep(timeStudying);

  printf("the student number %d end his programming session with teacher "
         "assistant\n",
         *n);
  countStudent++;
  availableChairs--;
  pthread_mutex_unlock(&roomMutex);
  sem_post(&chairs_sem);

  free(n);
  return NULL;
}

int main() {
  sem_init(&chairs_sem, 0, 3);
  pthread_mutex_init(&roomMutex, NULL);
  sem_init(&t_a_sem, 0, 1);

  int n = 12;
  pthread_t studentThreads[n], teacherAssistantThread;

  pthread_create(&teacherAssistantThread, NULL, teacherRoutine, NULL);

  for (int i = 0; i < STUDENT_NUM; i++) {
    int *n = malloc(sizeof(int));
    *n = i;
    pthread_create(studentThreads + i, NULL, studentRoutine, n);
  }

  for (int i = 0; i < STUDENT_NUM; i++) {
    pthread_join(studentThreads[i], NULL);
  }

  pthread_join(teacherAssistantThread, NULL);

  pthread_mutex_destroy(&roomMutex);
  sem_destroy(&chairs_sem);
  sem_destroy(&t_a_sem);

  return 0;
}
