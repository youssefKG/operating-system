#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>

int main() {
  int pid = fork();

  if (pid < 0) {
    printf("fork failer! ");
    return 1;
  } else if (pid == 0) { // child process
    char *args[10] = {"gcc", "prog.c", "-o", "prog", NULL};
    execvp(args[0], args);
  } else { // parent process
    wait(NULL);
    char *args[2] = {"./prog", NULL};
    execvp(args[0], args);
  }
  return 0;
}
