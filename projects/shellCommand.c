#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

#define MAX_LINE 80

void getString(char *s) {
  int i =0;
  char c; 

  while(c != '\n') {
    scanf("%c", &c);
    s[i] = c; 
    s = (char *)realloc(s, (i + 1) * sizeof(char));
    i++;
  }

  s[i - 1] = '\0'; 
}

void initArgs(char* args[]) {
  int i = 0;
  while(i < MAX_LINE/2) {
    args[i] = NULL;
    i++;
  }
}

void printArgs(char* args[]){
  int i = 0;
  while(args[i] != NULL) {
    printf("arg %d: %s ", i, args[i]);
    i++;
  }

  printf("\n");
}


int main() {
  while(1) {
    pid_t pid = fork();

    if (pid < 0) {
      printf("FORK ERROR!!");
    }
    else if (pid == 0){
      printf("CHILD PROCESS: \n");

      char* s;
      char* args[MAX_LINE/2 +1];
      int i, j, k;

      i = j = k = 0;

      s = (char*)malloc(sizeof(char));
      args[j] = (char*)malloc(MAX_LINE * sizeof(char));

      printf("osh>");

      getString(s);

      while(s[i] != '\0') {
        if(s[i] == ' '){
          args[j][k] = '\0';
          ++j;
          k = 0;
          args[j] = (char*)malloc(MAX_LINE * sizeof(char));
        }
        else if(s[i] != ' ') {
          args[j][k] = s[i];
          k++;
        }

        i++;
      }

      args[j][k] = '\0';
      args[j + 1] = NULL;


      execvp(args[0], args);
      
      i = 0;
      while(args[i] != NULL){
        free(args[i]);
        i++;
      }
      free(s);
    }
    else {
      wait(NULL);
      printf("PARENT PROCESS\n");
    }
  }
  return 0;
}
