#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 100 // Max size of input string

void execute_command(char *, char **, int);
int get_number_of_spaces(char *);
void get_args(char *, char **);
int checkCD(char **args);
void writeIntoFile(char *str);

char *exit_str = "exit";
char *cd_str = "cd";

int main()
{
  char process_name = 1;
  char input[SIZE];
  while (1)
  {
    gets(input);

    int spaces = get_number_of_spaces(input); // Needed to get total number of args.
    writeIntoFile(input);
    char **args = (char **)calloc(spaces + 2, sizeof(char *)); // last character is null.
    args[0] = input;
    args[spaces + 1] = NULL; // Args array must terminate with a null pointer.
    get_args(input, args);   // Now we fill the args array.
    execute_command(input, args, spaces + 1);
  }
  return 0;
}

int get_number_of_spaces(char *str)
{
  int spaces = 0;
  for (int i = 0; i < SIZE; i++)
  {
    if (str[i] == '\0')
    {
      break;
    }
    if (str[i] == ' ')
    {
      spaces++;
    }
  }
  return spaces;
}

void get_args(char *str, char **args)
{
  char *ptr = str;
  int count = 1;
  char space_found = 0;
  while (*ptr != '\0')
  {
    if (*ptr == ' ')
    {
      *ptr = '\0'; // Separate args by null characters.
      space_found = 1;
    }
    else if (space_found)
    {
      args[count] = ptr;
      count++;
      space_found = 0;
    }
    ptr++;
  }
}

void execute_command(char *input, char **args, int length)
{
  char wait_arg = 1;
  //Multithreading
  for (int i = 1; i < length; i++)
  {
    // Don't wait for child if found.
    if (strchr(args[i], '&') != NULL)
    { 
      args[i] = NULL;
      printf("Don't Wait!\n");
      wait_arg = 0;
    }
  }
  //Exit
  if (strcmp(input, exit_str) == 0)
  { 
    exit(0);
  }
  int pid = fork();
  //Navigation
  if (!strcmp(input, cd_str))
  {
    checkCD(args);
  }
  //Child
  else if (!pid)
  {
    // Needed to be sure that parent waits.
    int done = execvp(input, args);
    if (done == -1)
      printf("Error!\n");
  }
  //Parent
  else
  {
    signal(SIGCHLD, SIG_IGN); // To avoid Zombie processes.
    if (wait_arg)
    {
      waitpid(pid, 0, WUNTRACED);
    }
  }
}

void writeIntoFile(char *str){
     FILE *fptr;
        fptr = fopen("logger.text","w");

        if(fptr == NULL)
   {
      printf("Error!");   
      exit(0);             
   }
      fprintf(fptr,"%s\n",str);


}

int checkCD(char **args)
{
  if (args[1] == NULL)
    fprintf(stderr, "Expected command cd...\n");
  else
  {
    if (chdir(args[1]) != 0)
      perror("Error in Changing Directory...\n");
  }

  return 1;
}