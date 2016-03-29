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

extern int errno;

#define MAX_CMD_LEN  128
#define HISTORY_COUNT 20


int setenv(const char *envname, const char *envval, int overwrite);
char *path,*home;
char *hist[HISTORY_COUNT];
int current=0;
pid_t pid1; //Process id

//function declaration
void exec_echo(char *);
int cmd_exec(char **);
void SIGCHLD_handler(int num);
void chld_SIGQUIT_handler(int num);

//history function
int history(char *hist[], int current)
{
        int i = current;
        int hist_num = 1;

        do {
                if (hist[i]) {
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

//read the profile and setting the path
char* set(char *read_file,char *param){
  char * read_line = NULL;
    size_t buf = 0;
    FILE *file_ptr;
    char *args;
    file_ptr = fopen(read_file,"r");
    if(file_ptr==NULL){
      perror("Error while opening the file.\n");
              exit(-1);
    }
     while (getline(&read_line, &buf, file_ptr) != -1) {
       if(strstr(read_line,param)!=NULL){
         args = read_line;
         break;
       }
     }
    fclose(file_ptr);
    if(read_line)
    {
      free(read_line);

        }
char *pos;
if((pos=strchr(args,'\n'))!=NULL)
  *pos = '\0';
//printf("This is test print %s",strstr(args,"/"));
return strstr(args,"/");
}

int
set_profile(void)
{
  //  printf("\nInside the profile function");
    path = set(".profile","PATH");
    setenv("PATH",path,true);//
  printf("\nPATH is %s",path);
    home=set(".profile","HOME");
    setenv("HOME",home,true);
    printf("\nHOME is%s\n",home);
    //To display the current directory
/*
    if (getcwd(cwd, sizeof(cwd)) != NULL){
    fprintf(stdout, "Current working dir: %s\n",cwd);}
    printf("Hi after reading the file");*/
    return 0;
}

//reading from the line
char *get_from_line()
{
  char *line = NULL;
  int bytes_read;

  size_t buffersize = 0; // Just a buffer

  bytes_read = getline(&line, &buffersize, stdin);
  //printf("%d",bytes_read);
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
  int status = 0;
  if(strcmp(args[0],"echo") == 0)
  {
    int pos = 1;
    printf("%s \n",args[pos++]);
  }
  else if(strcmp(args[0],"\r") == 0){
    printf("=>");
  }
  else if(strcmp(args[0],"h")==0){//histroy can be seen by pressing the h key
    history(hist,current);
  }
  else{
    return cmd_exec(args);
  }
  return status;
}


// MAIN FUNCTION
int main( void ){
  char *command_line;
  char **arguments;
  int status;
  int p;
  for (p = 0; p < HISTORY_COUNT; p++)//creating all the spaces for history buffer
      hist[p] = NULL;
  printf("Clling the profile function");

    if(set_profile() != 0) {
      printf("Error in reading file");
        return -1;
    }
  if(signal(SIGCHLD,SIGCHLD_handler) == SIG_ERR){
    printf("SIGNT install error\n");
    exit(1);
  }
  printf("\nIF you need the history of commands you have entered press the h key \n");
  while(1){
    printf("\n=>");//GIVEN PROMPT TO BE DISPLAYED
    command_line = get_from_line();//calling function to read from shell
    arguments = split_line(command_line);//separation of the line in to arguments
    //history_exec(command_line);//trying history storage
    status = parsin(arguments);
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
    if (execvp(args[0], args) == -1) {
      perror("sh: couldnot exec");
    }
    exit(0);
  } else if (pid1 < 0) {

    perror("sh: couldnot fork");
  } else {
  pid2 = fork();
  if (pid2 == 0)
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
  fflush(stdin);
  //printf("The value of d is %s",&d);
  if (d == 'Y' || d == 'y') {
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

//Signal handling functions
void SIGCHLD_handler(int num)
{
kill(pid1, SIGQUIT);
}
void chld_SIGQUIT_handler(int num)
{
exit(0);
}


