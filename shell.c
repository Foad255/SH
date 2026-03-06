#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAXARGS 128
#define MAXLINE 4096
#define MAXPROMPT 8191


#define CLEAR_ARRAY(arr) do { \
    for (size_t i = 0; i < sizeof(arr); i++) \
        (arr)[i] = 0; \
    } while(0)

static void sig_int(int);

void parseline(const char*cmdline,char**argv); 
void set_prompt(char*pwd,char*home,char*prompt);
int builtIn_command(char *arg);


void cd(char *arg,char *pwd,char *home,char *prompt);

/*
cd dir (dir that is inside the working dir)
cd /dir (absolute dir)
cd ..   (one step back in in the path, if not root already)
*/

int 
main(void)
{
    char commandline[MAXLINE];
    char *args[MAXARGS];
    char prompt[MAXPROMPT];
    char pwd[MAXPROMPT];
    
    pid_t pid;
    int status;
 

    getcwd(pwd, MAXPROMPT);
    char * home = getenv("HOME");

    set_prompt(pwd,home,prompt);
  

    printf("%s %c ",prompt,'%');
    
    while(fgets(commandline,MAXLINE,stdin) != NULL) {
       parseline(commandline,args);


       int r = builtIn_command(args[0]);
       switch(r) {
        case 0:
           exit(0);
           break;
        case 1:
            CLEAR_ARRAY(pwd);
            CLEAR_ARRAY(prompt);
            cd(args[1],pwd,home,prompt);  
            break;
       } 

       if((pid = fork()) < 0)
           printf("Fork Error\n");
       else if (pid == 0) {
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
  
    // 
    // for (int i = 0; i < argc; i++) {
    //     printf("argv[%d] = %s\n",i,argv[i]);
    // }
}

void
set_prompt(char*pwd,char*home,char*prompt) {
 
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



int 
builtIn_command(char *arg) {
    if (!strcmp(arg,"cd")) {
        return 1;
    }

       
   if (!strcmp(arg, "quit")) {
        printf("Good bye from shell\n");
        return 0;
       }
   return -1;
}

void
cd(char *arg,char *pwd,char *home,char *prompt) {
    chdir(arg);
    getcwd(pwd,MAXPROMPT);
    set_prompt(pwd,home,prompt);  
}


