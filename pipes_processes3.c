#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//Modified version of pipes_processes2.c that takes one argument
//from the command line that is passed to the grep program. It 
//then creates three processes P1 - Parent that executes cat scores,
//P2 - Child that executes grep <argument>, and P3 - Child's Child
//that executes sort.

int main(int argc, char **argv){
  if (argc != 2){
    fprintf(stderr, "Usage: %s <grep_argument>\n", argv[0]);
    return 1;
  }

  int pipefd1[2], pipefd2[2];
  int pid1, pid2;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
  char *sort_args[] = {"sort", NULL};

  //make pipes
  pipe(pipefd1);
  pipe(pipefd2);

  pid1 = fork();

  if (pid1 == 0){
    //P2 - Child
    pid2 = fork();

    if (pid2 == 0){
      //P3 - Child's Child
      //execute sort

      //replace standard input with input part of pipe
      dup2(pipefd2[0], 0);

      //close unused half of the pipes
      close(pipefd1[0]);
      close(pipefd1[1]);
      close(pipefd2[1]);

      //execute sort
      execvp("sort", sort_args);
    }
    else{
      //P2 - Child
      //execute grep

      //replace standard input with input part of pipe
      dup2(pipefd1[0], 0);

      //replace standard output with output part of the second pipe
      dup2(pipefd2[1], 1);

      //close unused half of pipes
      close(pipefd1[1]);
      close(pipefd2[0]);

      //execute grep
      execvp("grep", grep_args);
    }

  }
  else{
    //P1 - Parent
    //execute cat

    //replace standard output with output part of the first pipe
    dup2(pipefd1[1], 1);

    //close unused half of the pipes
    close(pipefd1[0]);
    close(pipefd2[0]);
    close(pipefd2[1]);

    //execute cat
    execvp("cat", cat_args);    
  }

  return 0;

}