#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct personne {
  char name[100];
  int age;
};

typedef struct personne Personne;

void writeInFile(char *fileName, Personne student) {
  FILE *file = fopen(fileName, "a");
  if (file == NULL) {
    printf("Error opening file\n");
    return;
  }
  fprintf(file, "The student's name is %s and age is: %d\n", student.name,
          student.age);
  fclose(file);
}

int main() {
  Personne *personnes; // Pointer to array of 'Personne' structs, representing
                       // all students
  int n;               // Number of students
  int pid, pid2;       // Process IDs for child processes
  Personne student;    // Temporary variable to hold each student's data

  // Pipes for inter-process communication
  int fd1[2]; // Pipe 1: Parent to Child 1 (fd1[0] for reading, fd1[1] for
              // writing)
  int fd2[2]; // Pipe 2: Parent to Child 2 (fd2[0] for reading, fd2[1] for
              // writing)

  // Create the first pipe and check for errors
  if (pipe(fd1) == -1) {
    printf("Pipe 1 creation failed\n");
    return 3;
  }

  // Create the second pipe and check for errors
  if (pipe(fd2) == -1) {
    printf("Pipe 2 creation failed\n");
    return 4;
  }

  // Fork the first child process (Child 1)
  pid = fork();
  if (pid < 0) { // Check if fork failed
    printf("Fork failed\n");
    return 1;
  }

  if (pid == 0) {  // Child 1 process
    close(fd1[1]); // Close the unused write end of Pipe 1

    // Read data from the pipe and write to a file
    while (read(fd1[0], &student, sizeof(student)) > 0) {
      writeInFile("student_p1.txt", student);
    }

    close(fd1[0]); // Close the read end after processing
    exit(0);       // Terminate Child 1
  }

  // Fork the second child process (Child 2)
  pid2 = fork();
  if (pid2 < 0) { // Check if fork failed
    printf("Fork 2 failed\n");
    return 3;
  }

  if (pid2 == 0) { // Child 2 process
    close(fd2[1]); // Close the unused write end of Pipe 2

    // Read data from the pipe and write to a file
    while (read(fd2[0], &student, sizeof(student)) > 0) {
      writeInFile("student_p2.txt", student);
    }

    close(fd2[0]); // Close the read end after processing
    exit(0);       // Terminate Child 2
  }

  // Parent process
  close(fd1[0]); // Close the unused read end of Pipe 1
  close(fd2[0]); // Close the unused read end of Pipe 2

  // Get the number of students from user input
  printf("Enter the number of students: ");
  scanf("%d", &n);

  // Allocate memory for the array of students
  personnes = (Personne *)malloc(n * sizeof(Personne));
  if (personnes == NULL) {
    printf("Memory allocation failed\n");
    return 5;
  }

  // Get student details from user input
  for (int i = 0; i < n; i++) {
    printf("Enter the name of student %d: ", i + 1);
    scanf("%s", personnes[i].name);
    printf("Enter the age of student %d: ", i + 1);
    scanf("%d", &personnes[i].age);

    // Send student data to the appropriate child process based on age
    if (personnes[i].age < 20) {
      // Send to Child 1 if age is less than 20
      if (write(fd1[1], &personnes[i], sizeof(personnes[i])) == -1) {
        printf("Error writing to Pipe 1\n");
        free(personnes);
        return 4;
      }
    } else {
      // Send to Child 2 if age is 20 or above
      if (write(fd2[1], &personnes[i], sizeof(personnes[i])) == -1) {
        printf("Error writing to Pipe 2\n");
        free(personnes);
        return 4;
      }
    }
  }

  // Close the write ends of both pipes after sending all data
  close(fd1[1]);
  close(fd2[1]);

  // Free the allocated memory
  free(personnes);

  // Wait for both child processes to finish
  wait(NULL);
  wait(NULL);

  return 0; // End of the program
}
