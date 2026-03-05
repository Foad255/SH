#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAXARGS 128
#define MAXLINE 4096
#define MAXPROMPT 8191

static void sig_int(int);

void parseline(const char*cmdline,char**argv);
char *get_prompt(char*pwd,char*home,char*prompt);

int 
main(void)
{
    char commandline[MAXLINE];
    char *args[MAXARGS];
    char prompt[MAXPROMPT];
    pid_t pid;
    int status;
   
    char * pwd  = getenv("PWD"); 
    char * home = getenv("HOME");

    get_prompt(pwd,home,prompt);
  

    printf("%s %c ",prompt,'%');
    
    while(fgets(commandline,MAXLINE,stdin) != NULL) {
       parseline(commandline,args);

       if (!strcmp(args[0], "quit")) {
            printf("Good bye\n");
            exit(0);
       }


       pid = fork();
     
       if (pid == 0) {
        execvp(args[0],args);
        exit(127);
       }

       if ((pid = waitpid(pid, &status, 0)) < 0)
           printf("Waitpid error"); 
       printf("%s %c ",prompt,'%');
    }
}



void
parseline(const char *cmdline, char **argv) 
{
    char array[MAXLINE];
    char *buf = array;
    char *delim;
    int argc;


    strcpy(buf,cmdline);
    buf[strlen(buf)-1] = ' ';
    while (*buf && (*buf == ' '))
        buf++;

    argc = 0;
    while((delim = strchr(buf, ' '))) {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' '))
            buf++;
    }
    argv[argc] = NULL;
  
    
    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n",i,argv[i]);
    }
}

char * get_prompt(char*pwd,char*home,char*prompt) {
 
    int atHome = 1;
    for(int i = 0; i < strlen(home); i++){
        if(pwd[i] != home[i]) {
           atHome = 0;
           break;
        }
    }


    if(atHome) {
        for(int i = 0; i < strlen(home); i++) {
            pwd+=1;
        }

        prompt[0] = '~';
        strcat(prompt,pwd);
    } else {
        for(int i = 0; i < strlen(pwd); i++) {
            prompt[i] = pwd[i];
        }
      }
}


