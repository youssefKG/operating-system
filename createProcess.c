#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
  pid_t pid;

  /* fork a child process */
  pid = fork();

  if (pid < 0){ /* error accurred */
    fprintf(stderr, "fork failed\n");
    return 1;
  }else if (pid == 0) { /* child process */
    execlp("/bin/ls", "ls", NULL);
  }else { /* parent process */
    /* child will wait for the parent to complete */
    wait(NULL);
    printf("chidl complet\n");
  }

  return 0;
}
