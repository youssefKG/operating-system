#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>

struct Command {
  char commandsHistory[10][80];
  char nextEmptyIndex;
};

typedef struct Command command;

command myCommands;

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

int addCommandToHistory(char *command) {
  if (myCommands.nextEmptyIndex == 10) {
    printf("is full\n");
    for (int i = 0; i < 9; i++) {
      strcpy(myCommands.commandsHistory[i], myCommands.commandsHistory[i + 1]);
    }

    strcpy(myCommands.commandsHistory[myCommands.nextEmptyIndex - 1], command);
    return myCommands.nextEmptyIndex;
  }

  strcpy(myCommands.commandsHistory[myCommands.nextEmptyIndex], command);
  return myCommands.nextEmptyIndex + 1;
}

void printCommandsHistory(char commands[10][80], int length) {
  if (length < 0) {
    printf("the commads history is empty\n");
    return;
  }

  printf("Commands history: \n");
  for (int i = 0; i <= length; i++)
    printf("%d: %s\n", i, commands[i]);
}

void parseCommand(char **args, char *s) {
  int i, j, k;

  i = j = k = 0;

  args[j] = (char *)malloc(80 * sizeof(char));

  while (s[i] != '\0') {
    if (s[i] == ' ') {
      args[j][k] = '\0';
      ++j;
      k = 0;
      args[j] = (char *)malloc(80 * sizeof(char));
    } else if (s[i] != ' ') {
      args[j][k] = s[i];
      k++;
    }

    i++;
  }

  args[j][k] = '\0';
  args[j + 1] = NULL;
}

int main() {

  while (1) {
    int fd[2];  // child => parent
    int fd2[2]; // parent => child
    char input[80];

    if (pipe(fd) == -1) {
      printf("pipe 1 failed !\n");
      return 1;
    }
    if (pipe(fd2) == -1) {
      printf("pipe number 2 failed !\n");
      return 1;
    }

    int pid = fork(); // fork my program

    // if creating process failed
    if (pid < 0)
      printf("fork failed!\n");
    else if (pid == 0) {
      char *args[40];

      command myHistory; // command history

      close(fd[0]);  // Close unused read end of pipe 1
      close(fd2[1]); // Close unused write end of pipe 2

      // get command from cli
      printf("enter a command: ");
      scanf("%s", input);

      // send the command to parent process throw pipe 1
      if (write(fd[1], input, 80 * sizeof(char)) == -1) {
        printf("error in writing in pipe");
        return 1;
      }

      // receive the command histroy from parent process throw pipe 2
      if (read(fd2[0], &myHistory, sizeof(myHistory)) == -1) {
        printf("failed reading from pipe 2\n");
        return 4;
      }

      // check if the input is history
      if (strcmp(input, "history") == 0)
        // print command history
        printCommandsHistory(myHistory.commandsHistory,
                             myHistory.nextEmptyIndex);

      // if the user input "!!" parese the last command in history
      else if (strcmp(input, "!!") == 0) {
        printf("parse last command \n");
        parseCommand(args,
                     myHistory.commandsHistory[myHistory.nextEmptyIndex - 1]);

      } else // if the input not equal "history" or "!!" parse the input itself
        parseCommand(args, input);

      printf("the last command index %d \n", myHistory.nextEmptyIndex - 1);

      // execte the parse input(args) using system call "execvp"
      execvp(args[0], args);

      close(fd[1]);  // Close write end after using it
      close(fd2[0]); // Close the read in pipe 2 after using it
      exit(0);
    } else {         // parent process
      close(fd[1]);  // Close unused write end of pipe 1
      close(fd2[0]); // Close unused read end of pipe 2

      // read user input in parent process throw pipe 1
      if (read(fd[0], input, 80 * sizeof(char)) == -1) {
        printf("error in reading from pipe\n");
        return 2;
      }

      // store the user input in command history if not equel either "history"
      // or "!!"
      if (strcmp(input, "history") != 0 && strcmp(input, "!!") != 0)
        myCommands.nextEmptyIndex = addCommandToHistory(input);

      // send the history to child process throw pipe 2
      if (write(fd2[1], &myCommands, sizeof(myCommands)) == -1) {
        printf("pipe failed in writing history\n");
        return 3;
      }

      wait(NULL);

      close(fd[0]);  // Close read end after using it
      close(fd2[1]); // Close write end after using it
    }
  }
  return 0;
}
