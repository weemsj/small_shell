#define _GNU_SOURCE
#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 200112L
#define __STDC_LIB_EXT1__ 1
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdint.h>
#include <signal.h>

#define MAX_ARGS 512
#define MAX_CHARS 2048



/* struct for command line */

struct commandLine {
    char *command;
    char *arguments[MAX_ARGS];
    char *inFileName;
    char *outFileName;
    int *argCount;
    _Bool background;
    int *pid;
    };


struct commandLine* createCommand(char *command, char *arguments, char *inFileName, char *outFileName, int argCount, bool background){
      
     char* result;
     //char *empty = {NULL};
     // allocate dynamic memory for command line
     struct commandLine* c1 = malloc(sizeof(struct commandLine));

     // allocate memory for command and then copy into stucture
     c1->command = calloc(strlen(command) + 1, sizeof(char));
     c1->command = command;

     
     
     c1->command = arguments;
     
        
     if(inFileName != NULL){
     //allocate memory for input file name then copy the filename into structure
       c1->inFileName = calloc(strlen(inFileName) + 1, sizeof(char));
       c1->inFileName = inFileName;
     }else{
       c1->inFileName = NULL;
     }

     if(outFileName != NULL){
     //allocate memory for output file name then copy the filename into stucture
       c1->outFileName = calloc(strlen(outFileName) + 1, sizeof(char));
       c1->outFileName = outFileName;
     }else{
       c1->outFileName = NULL;
     }
     
     int *ptr = &argCount;
     if(ptr != NULL){
      //allocate memory for argument count, then copy into structure
      c1->argCount = malloc(argCount * sizeof(int));
      c1->argCount = &argCount;
     }else{
      c1->argCount = NULL;
     }
     

     c1->background = malloc(1);
     if(background == true){
       c1->background = 1;
     }else{
       c1->background = 0;
     }
     
     pid_t pid = getpid();
     pid_t *pidPtr = &pid;
     c1->pid = malloc(sizeof(pid));
     c1->pid = pidPtr;


     return c1;
};

void prompt(){
  char prompt[] = ":";
  printf("%s", prompt);
  fflush(stdout);
}

struct commandLine* commandPrompt(struct commandLine* cl){
  
 
  char userInput[MAX_CHARS];  // handle max characters
  char *command[1]; // one command and null pointer
  char *arguments[MAX_ARGS];  // handle max arguments
  char *holder[1] = {};  
  bool redirIn = false;
  bool redirOut = false;
  bool isCommand = false;
  bool background = false;
  char *inFileName[1] = {};
  char *outFileName[1] = {};
  int argCount = 0;

  /*ASSIGN FIRST ARGUMENT IN THE COMMANDLINE TO COMMAND */
  for(int i = 0; i < 1;){
   
    // prompt user with ":" to enter a command
    prompt();
    
    // get input from standard in, only read upto the max character from command line and store it in userInput variable
    fgets(userInput, MAX_CHARS, stdin);

    // store the value in holder to elvaluate it and parse command
    holder[0] = strtok(userInput, " ");

    // if the user enters a "#" or new line, they get a new prompt otherwise the first
    // value is assigned to command variable.
    if (strcmp(holder[0], "\n") == 0 || strcmp(holder[0], "#") == 0){
        i = 0;
    }
    //assign first value to command variavle
    else {
      command[0] = holder[0];
      //printf("%s",*command);
      i = 1;
      isCommand = true;

    }
  }


  // parse remander input into arguments array, check for redirections and background options
  for (int i=0; i < 512; i++){

     // user holder as a variable to compare strings, if the value in holder is NULL no more arguments
     // exist and we check to see if the last argument is "&" if so we run the 
     // the boolean variable background is true. Holder is also compared to "<" and ">" 
     // if the value in holder equals "<" it changes the boolean value of redirIn to true and on the next iteration
     // the value in holder will be the file name of the inpoutfile. After the file name is saved the 
     // boolean value of redirIn is changed back to false. The same logic is applied for ">". 
     // Otherwise the value in holder is considered
     // an arguent so it is counted and added to the array of arguments.
     holder[0] = strtok(NULL, " ");
     
     // reached the end of the string  
     if(holder[0] == NULL){
          // check if the last value read is a background command, if so set the background variable to true and
         // decrement the argCount, remove the ampersand sign.
        if(argCount > 0 && strcmp(arguments[argCount - 1], "&") == 0  ){
          
          arguments[i-1] = NULL;
          argCount -= 1;
          background = true;
          break;
        }
        
        else{
          arguments[i] = NULL;
        }
        break;
        
     }

     // check for file redirection
     else if(strcmp(holder[0], "<") == 0) {
        redirIn = true;
     } 
     else if (strcmp(holder[0], ">")== 0 ){
        redirOut = true;
     }
     else if (redirIn) {
        inFileName[0] = holder[0];
        redirIn = false;
     }
     else if (redirOut){
        outFileName[0] = holder[0];
        redirOut = false;
     }

     // if just a normal command continue through loop, incrementing the argCound value.
     else{
       arguments[i] = holder[0];
       argCount ++;
      }
  }

   // initialize args 2-d array with MAX_ARGS x MAX_CHARS with empty values
   char *args[argCount];
   char* p;
  
  
   //printf("args[0] = %s", args[0]);
   //fflush(stdin);

   for(int i=0; i<argCount; i++){
     //check the array for variable expansion
     char *expand = strstr(arguments[i], "$$");
     // if no variable expansion inside of the string just copy the string into the args array
     if(expand == NULL){
       args[i]= arguments[i];
       
       }else{
       p = arguments[i];
       for(int k = 0; k < strlen(arguments[i])-1; k++){
          if(strncmp(&p[k], "$$", 2) == 0){
            //expand, get pid and cat it to the array;
            char *pid;
            sprintf(pid, "%i", getpid());
            strcat(args[i],pid);
            k++;
          }else{
            //store the remander of the array after the variable expansion into a temp array
            //then cat into the args array
            char temp[1];
            strncpy(temp,&p[k], 1);
            strcat(args[i], temp);
          }
        }
       }
     }



  cl = createCommand(command[0], *args, inFileName[0], outFileName[0], argCount, background);
  return cl;

}


int main(int argc, char *argv[]){
  // keeps track of background processes, the processess variable tells us how many processses and the pid array makes it easy to
  // kill a process.
  int processes = 0;
  int *pids[5] = {0,0,0,0,0};
  // reallocate pids
  int childStatus;
  bool leave = false;
  
  // initialize parent process signal hadlers
  struct sigaction ignore_SIGINT={}, SIGSTP_action={};

  ignore_SIGINT.sa_handler = SIG_IGN;

  sigaction(SIGINT, &ignore_SIGINT, NULL);

  // loop starts here

LOOP:for(;;) {
    
    if(processes > 0){
      // check background processes for termination at the start of the loop
      for(int i = 0; i < sizeof(pids); i++){ 
        if(pids[i] > 0){
          pid_t bg = waitpid(*pids[i], &childStatus, WNOHANG);
          if(bg > 0){
            if(WIFEXITED(childStatus)){
              printf("background pid %jd done: exit value %i\n", (intmax_t) bg, childStatus);
          
            }else{
              printf("background pid %jd done: terminated by signal %d", (intmax_t) bg, childStatus);
            }
            processes --;
          }
        }
      }
    }
    /* initialize command line struct variable as cl, cl will be a pointer to the command line structure that will 
     * hold the command, arguments, redirect files and background status. The commandPrompt(struct commandLine) function
     * takes a commandLine structure as a parameter and returns a commandLine structure with values assigned to the
     * memers. After cl is returned we can access the values of each member using "->" notation. */
    struct commandLine *cl;
    cl = commandPrompt(cl);

    int argCount = *cl->argCount;
    int count = argCount + 2;
    char *command = cl->command;
    char *argsv[count];
    int inFD;
    int outFD;
    
    for(int i=0; i< argCount; i++){
      for(int k = 0; k < count; k++){
        // set last element to NULL
        if(k == (count - 1)){
          argsv[count - 1] = NULL; 
        }
        // set first element to command
        if(k == 0){
          argsv[k] = command;
        }
        else{
          argsv[i] = cl->arguments[i];
        }
      }
    }
  
    /* built in funtions (function that takes cl-> command and checks command for exit, status, cd then sends it to exit, status, cd */
    
    // exit will check for background processes, if no background process, exit normally. If there are background processes, send them the
    // termination signal and exit. 
    if(strcmp(command, "exit") == 0){
       // check if child processes finish
       if(processes == 0){
         leave = true; 
         exit(0);
       }
       else{
         for(int i=0; i < processes; i++){
        //send exit signal to other processes
          if(*pids[i] != 0 || *pids[i] != -1){
             kill(*pids[i], 9);
          }
         }
         exit(0);
       }
     }
  
    // status built in command chavks the value of childStatus to see how it exited.
    if(strcmp(command, "status") == 0){
     
      if(WIFEXITED(childStatus)){
        printf("exit value %d\n", WEXITSTATUS(childStatus));
        fflush(stdin);
        continue;
      }
      else{
        printf("terminated by signal %d\n", WTERMSIG(childStatus));
        fflush(stdin);
        continue;
      }
    }


    // cd built in command check
    if(strcmp(command, "cd") == 0){
      if(*cl->argCount == 2){
        //change directory to path in argument
        if(chdir(*cl->arguments) != 0){
          perror("chdir() failed\n");
          fflush(stdin);
          continue;
        }
        else{
          chdir(*cl->arguments);
          continue;
        }
      }
      else if(*cl->argCount > 2){
        // too many args passed
        printf("To many arguments passed\n");
        fflush(stdin);
      
      }
      else{
        // get thome env and set that as the current directory
        char *home = "HOME";
        home = getenv(home);
        
        if(chdir(getenv(home)) == 0) {
           perror("chdir() failed\n");
           fflush(stdin);
           continue;
        }
        else{
          chdir(home);
          continue;
        }
      }
     }
    
    // fork child process
    else{   
      
      pid_t child = fork();
      if(child == -1){
        perror("fork() failed!");
        childStatus = 1;
        exit(childStatus);
      }else if(child == 0){
        
        // child executes here
        // redirect input only
        if(cl->inFileName != NULL){
            inFD = open(cl->inFileName, O_RDONLY);
            if(inFD == -1){
              perror("source open()");
              childStatus = 1;
              exit(childStatus);
            }
            int result = dup2(inFD, 0);
            if(result == -1){
              perror("souce dup2()");
              childStatus = 2;
              exit(childStatus);
            }
        }

        // redirect output only
        if(cl->outFileName != NULL){
            outFD = open(cl->outFileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if(outFD == -1){
              perror("target open()");
              childStatus = 1;
              exit(childStatus);
            }
            int result = dup2(outFD, 1);
            if(result == -1){
              perror("target dup2()");
              childStatus = 2;
              exit(childStatus);
            }
          }

          // handle background redirection here and redirects input to /dev/null if user didnt specify
         if(cl->background == 1 && cl->inFileName == NULL){
            inFD = open("/dev/null", O_RDONLY);
            if(inFD == -1){
              perror("source open()");
              childStatus = 1;
              exit(childStatus);
            }

            int result = dup2(inFD, 0);
            if(result == -1){
              perror("source dup2()");
              childStatus = 2;
              exit(childStatus);
            } 
        }// handle background output redirection here and redirects output to /dev/null if user didnt specify
        if(cl->background == 1 && cl->outFileName == NULL){
            outFD = open("/dev/null", O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if(outFD == -1){
              perror("target open()");
              childStatus = 1;
              exit(childStatus);
            }
            int result = dup2(outFD, 1);
            if(result == -1){
              perror("target open()");
              childStatus = 2;
              exit(childStatus);
            }
        }
        
        if(sigaction(SIGINT, NULL, &ignore_SIGINT) == -1){
            perror("sigaction()");
            fflush(stdin);
          }
        else{
            sigaction(SIGINT, NULL, &ignore_SIGINT);
        }
        if(argCount == 0){

          execvp(argsv[0],argsv );
          perror("execvp");
          fflush(stdin);
          childStatus = 1;
          exit(childStatus);

        }else{
                  
          execvp(argsv[0], argsv);
          perror("execvp");
          fflush(stdin);
          childStatus = 1;
          exit(childStatus);
        }
     
      
      }else{
        // parent process executes here
        if(cl->background == 1){
          pids[processes] =&child;
          processes ++;
          printf("background pid is %jd\n",(intmax_t) child);
          
        }else{
          child = waitpid(child, &childStatus, 0);      
        }  
      }  
    }
  

  // free cl structure memory allocation
  free(cl);

  fflush(stdin);
  
  }
  return 0;
}
