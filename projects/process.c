#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>

struct personne {
  char name[100];
  int age;
};

typedef struct personne Personne;

void writeInFile(char *fileName, Personne student) {
  FILE *file = fopen(fileName, "a");
  if (file == NULL) {
    printf("error in file\n");
    return;
  }
  fprintf(file, "the student name is %s and age is : %d\n", student.name,
          student.age);
  fclose(file);
}

int main() {

  Personne *personnes;
  int pid;
  int n;
  Personne student;

  int fd1[2]; // p1 -> p2
  int fd2[2]; // p2 -> p3

  fflush(stdout);

  if (pipe(fd1) == -1) {
    printf("pipe 1 failed \n");
    return 3;
  }
  if (pipe(fd2) == -1) {
    printf("the pipe 2 failed \n");
    return 4;
  }

  pid = fork();

  FILE *fptr;
  if (pid < 0) {
    printf("fork failed !");
    return 1;
  }
  if (pid == 0) {
    close(fd1[1]);
    while (read(fd1[0], &student, sizeof(student)) > 0) {
      writeInFile("student.text", student);
    }
    close(fd1[0]);
    exit(0);
  }

  int pid2 = fork();

  if (pid2 < 0) {
    printf("fork 2 failed\n");
    return 3;
  } else if (pid2 == 0) {
    close(fd2[1]);
    while (read(fd2[0], &student, sizeof(student)) > 0)
      writeInFile("student.text", student);

    close(fd2[0]);
    exit(0);
  }

  close(fd1[0]);
  close(fd2[0]);
  printf("enter the numbers of student : ");
  scanf("%d", &n);

  personnes = (Personne *)malloc(n * sizeof(Personne));

  for (int i = 0; i < n; i++) {
    printf("enter the name of student %d: ", i + 1);
    scanf("%s", personnes[i].name);
    printf("enter you age of student %d : ", i + 1);
    scanf("%d", &personnes[i].age);
    if (personnes[i].age < 20) {
      if (write(fd1[1], &personnes[i], sizeof(personnes[i])) == -1) {
        printf("error write to pipe 1\n");
        return 4;
      }
    } else {
      if (write(fd2[1], &personnes[i], sizeof(personnes[i])) == -1) {
        printf("error write to pipe 2\n");
        return 4;
      }
    }
  }
  close(fd1[1]);
  close(fd2[1]);
  free(personnes);

  return 0;
}
