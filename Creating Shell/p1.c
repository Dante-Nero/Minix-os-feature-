#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <dirent.h>
#include <curses.h>

#define HISTORY_COUNT 20

char *hist[HISTORY_COUNT];
int current=0;

extern int errno;
//function declaration
void exec_echo(char *);
int cmd_exec(char **);

pid_t pid1; //Process id

//Signal handling functions
void SIGCHLD_handler(int num)
{
kill(pid1, SIGQUIT);
}
void chld_SIGQUIT_handler(int num)
{
exit(0);
}
//history function
int history(char *hist[], int current, char cmd[])
{
        int i = current;
        int hist_num = 1;
        int ret;
        int cmd_len;


        do {
                if (hist[i]) {
                  ret = strspn(hist[i],cmd);
                  cmd_len = strlen(cmd);
                  //printf("Command_length    %d",cmd_len);
                  //printf("ret value    %d",ret);
                  if(ret == cmd_len-1)
                    printf("%s\n", hist[i]);
                  hist_num++;
                }

                i = (i + 1) % HISTORY_COUNT;

        } while (i != current);

        return 0;
}
//clearing the history
int clear_history(char *hist[])
{
        int p;

        for (p= 0; p < HISTORY_COUNT; p++) {
                free(hist[p]);
                hist[p] = NULL;
        }

        return 0;
}

char *get_from_line()
{
  char *line = NULL;
  int bytes_read;
  size_t buffersize = 0;
  char d;
  bytes_read = getline(&line, &buffersize,stdin);
  //printf("%c, the line entered",line[bytes_read-1]);
  //printf("%dbytes_read",bytes_read);
  d= line[bytes_read-1];

  if (bytes_read <1)//exception handled
    {
    puts("Error!!!");
     }
//making sure that by simply pressing the enter key does not create an error
  if(bytes_read >1){
     return line;//returning to main
  }
  else if(bytes_read == 1){
     return "=>";
  }
  return line;
}

//function to split the line in to arguments
char **split_line(char *line)
{
  int buffsize = 50, pos = 0;
  char *token;
  char **tokenpos = malloc(buffsize * sizeof(char*));

  if (!tokenpos) {
    fprintf(stderr, "sh: no memory to allocate, re-run ur shell\n");
    exit(0);
  }

  token = strtok(line, " \a\r\n\t");
  while (token != NULL) {
    tokenpos[pos] = token;
    pos++;
//just-in case the line is lengthy especially for the exception handling
    if (pos >= buffsize) {
      buffsize = buffsize + 50;
      tokenpos = realloc(tokenpos, buffsize * sizeof(char*));
      if (!tokenpos) {
        fprintf(stderr, "sh: no memory to allocate, re-run ur shell\n");
        exit(0);
      }
    }

    token = strtok(NULL, " \a\r\n\t");
  }
  tokenpos[pos] = NULL;
  return tokenpos;
 //printf("\n this is extened  %s",*tokenpos);
  free(tokenpos);//free the space
}

//function to parse the arguments
int parsin(char **args)
{

  //rintf("%s from parsing",*args);
  int status = 0;
  if(strcmp(args[0],"echo") == 0)
  {
    int pos = 1;
    printf("%s \n",args[pos++]);
  }
  else if(strcmp(args[0],"h")==0){//histroy can be cleared by pressing the h key
    clear_history(hist);
  }
  else{
    return cmd_exec(args);
  }
  return status;
}
// Main function
int main (void){
  char *command_line;
  char **arguments;
  char d;
  int status;

  int p;
  for (p = 0; p < HISTORY_COUNT; p++)//creating all the spaces for history buffer
    hist[p] = NULL;
  int len =0;
  if(signal(SIGCHLD,SIGCHLD_handler)==SIG_ERR){
    printf("SIGNT install error");
    exit(1);
  }
  while(1){
    printf("=>");
    command_line = get_from_line();
    len = strlen(command_line);
    //printf("%c, the line entered",command_line[len-1]);
    d= command_line[len-2];
  if (d == '\b')
      printf("HI from backspace");
    else if (d == '\t'){
      if(command_line[len-1] == '\n')
        command_line[len-1] = '\0';
      history(hist,current,command_line);
    }
    else{
      arguments = split_line(command_line);
      status = parsin(arguments);
      }
 free(hist[current]);
    hist[current] = strdup(command_line);
    current = (current +1) % HISTORY_COUNT;
   // printf("%s this was the test case",arguments[1]);
    if(arguments[0]==NULL){
      //nothing wait
    }
    else{
      if(strcmp(arguments[0],"q") == 0)
        break;//quit the shell
    }
  }
  printf("Thanks for using the cs551_shell\n");
  return 0;

}



//function for executing commmands

int cmd_exec(char **args)
{
  int status;
  char d;
  pid_t pidp, pid2, wpid;
  pidp = getpid();
  pid1 = fork();
  if (pid1 == 0) {
    pid1 = getpid();
    if (execvp(args[0], args) == -1) {

      perror("sh: couldnot exec");
    }
    exit(0);
  } else if (pid1 < 0) {

    perror("sh: couldnot fork");
  } else {
  pid2 = fork();
  if (pid2 == 0)
    pid2 = getpid();
    {
    if (signal(SIGQUIT, chld_SIGQUIT_handler) == SIG_ERR)
    {
      printf("SIGINT install error\n");
        exit(1);
        }
    while(1)
    {
    sleep(5);
  printf("Do you want to terminate the current command[Y/N]:");
  d=getchar();
  getchar();
  
  //printf("The value of d is %s",&d);
  if (d == 'y') {
    kill(pidp, SIGCHLD);
    exit(0);
    }
  }

  }
  do{
      wpid = waitpid(pid1, &status, WUNTRACED);

  }while(!WIFEXITED(status) && !WIFSIGNALED(status));

}
kill(pid2, SIGQUIT);
  do{
      wpid = waitpid(pid2, &status, WUNTRACED);

  }while(!WIFEXITED(status) && !WIFSIGNALED(status));

return 1;

}

// have to use strstr command and compare the line entered in the command line with that of the history it gives the list of commands then i have concatnate the
//try using string compare command
