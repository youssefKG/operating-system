#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define MAX_LINE 80
#define MAX_COMMAND_HISTORY 10
#define COMMAND_LENGTH 40
#define COMMAND_DEPTH 10

char *commandsHistory[MAX_COMMAND_HISTORY]; // 10 latest command
char history[MAX_COMMAND_HISTORY][80];      // 10 latest command
int nextEmptyIndex;

void getString(char *s);
void parseCommand(char **args, char *s);
void freeMemoryOfArgs(char **args);
int addCommandToHistory(char *command);
void printCommandsHistory();
void initArgs(char **args);
void printArgs(char **args);
int addToHistory(char *command);
void printHistory();

int main() {
  int fd[2], fd2[2];
  int value = 0;

  if (pipe(fd) == -1) {
    printf("pipe failed\n");
    return 1;
  }
  if (pipe(fd2) == -1) {
    printf("creating pipe 2 failed");
    return 2;
  }

  while (1) {
    int pid = fork(); // create process
    char input[80];
    if (pid < 0) { // if creating process is fail
      printf("FORK ERROR!!");
    } else if (pid == 0) { // child process

      char *args[MAX_LINE / 2 + 1];

      scanf("%s", input);

      // write inputs in pipe 1
      if (write(fd[1], input, 80 * sizeof(char)) == -1) {
        printf("writing in pipe failed!\n");
        return 3;
      };

      if (read(fd2[0], commandsHistory, sizeof(commandsHistory)) == -1) {
        printf("failed reading from pipe 2\n");
        return 6;
      }
      exit(0);
      // if (strcmp(input, "history") == 0) {
      //   printCommandsHistory();
      // } else if (strcmp(input, "!!") == 0) {
      //   printf("the latest command is %s at index %d \n",
      //          commandsHistory[nextEmptyIndex - 1], nextEmptyIndex - 1);
      //   parseCommand(args, commandsHistory[nextEmptyIndex - 1]);
      //   execvp(args[0], args);
      // }
      // if (strcmp(input, "!!") != 0 && strcmp(input, "history") != 0) {
      //   nextEmptyIndex = addCommandToHistory(input);
      //   parseCommand(args, input);
      //   printCommandsHistory();
      //   if (execvp(args[0], args) == -1) {
      //     printf("error in execvp\n");
      //   }
      // }
    } else { // parent process
      wait(NULL);

      if (read(fd[0], input, 80 * sizeof(char)) == -1) {
        printf("reading from pipe failed!\n");
        return 4;
      };

      nextEmptyIndex = addToHistory(input);

      printHistory();
      if (write(fd2[1], history, sizeof(history)) == -1) {
        printf("writing commands in pipe 2 failed!\n");
        return 5;
      }
      printf("the value from pipe is : %s\n", input);
    }
  }
  return 0;
}

void getString(char *s) {
  int i = 0;
  char c;

  s = (char *)malloc(sizeof(char));

  printf("osh>");
  while (c != '\n') {
    scanf("%c", &c);
    s[i] = c;
    s = (char *)realloc(s, (i + 1) * sizeof(char));
    i++;
  }

  s[i - 1] = '\0';
}

void freeMemoryOfArgs(char **args) {
  int i = 0;
  while (args[i] != NULL) {
    free(args[i]);
  }
}

void parseCommand(char **args, char *s) {
  int i, j, k;

  i = j = k = 0;

  args[j] = (char *)malloc(MAX_LINE * sizeof(char));

  while (s[i] != '\0') {
    if (s[i] == ' ') {
      args[j][k] = '\0';
      ++j;
      k = 0;
      args[j] = (char *)malloc(MAX_LINE * sizeof(char));
    } else if (s[i] != ' ') {
      args[j][k] = s[i];
      k++;
    }

    i++;
  }

  args[j][k] = '\0';
  args[j + 1] = NULL;
}

// ["ls", "dir", "ls -la", "tree", "top"]
//
int addToHistory(char *command) {
  if (nextEmptyIndex == MAX_COMMAND_HISTORY) {
    printf("is full\n");
    for (int i = 0; i < MAX_COMMAND_HISTORY - 1; i++) {
      strcpy(history[i], history[i + 1]);
    }

    strcpy(history[nextEmptyIndex - 1], command);
    return nextEmptyIndex;
  }

  strcpy(history[nextEmptyIndex], command);
  return nextEmptyIndex + 1;
}

int addCommandToHistory(char *command) {
  if (nextEmptyIndex == MAX_COMMAND_HISTORY) {
    printf("is full\n");
    for (int i = 0; i < MAX_COMMAND_HISTORY - 1; i++) {
      strcpy(commandsHistory[i], commandsHistory[i + 1]);
    }

    strcpy(commandsHistory[nextEmptyIndex - 1], command);
    return nextEmptyIndex;
  }

  commandsHistory[nextEmptyIndex] =
      (char *)malloc(COMMAND_DEPTH * sizeof(char));
  strcpy(commandsHistory[nextEmptyIndex], command);
  return nextEmptyIndex + 1;
}

void initArgs(char *args[]) {
  int i = 0;
  while (i < MAX_LINE / 2) {
    args[i] = NULL;
    i++;
  }
}

void printHistory() {
  if (nextEmptyIndex < 0)
    printf("the commads history is empty");

  else
    for (int i = 0; i <= nextEmptyIndex; i++)
      printf("%d: %s\n", i, history[i]);

  printf("\n");
}
void printCommandsHistory() {
  if (nextEmptyIndex < 0)
    printf("the commads history is empty");

  else
    for (int i = 0; i <= nextEmptyIndex; i++)
      printf("%d: %s\n", i, commandsHistory[i]);

  printf("\n");
}

void printArgs(char *args[]) {
  int i = 0;
  while (args[i] != NULL) {
    printf("arg %d: %s ", i, args[i]);
    i++;
  }

  printf("\n");
}
