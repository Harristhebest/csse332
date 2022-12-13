/* Copyright 2016 Rose-Hulman
   But based on idea from http://cnds.eecs.jacobs-university.de/courses/caoslab-2007/
   */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void handle_sigchld(int ignored)
{
  /* TODO: Insert your code here! */
    printf("child signal recieved\n");
    printf("Background command finished running\n");

}


int main() {
    char command[82];
    char *parsed_command[2];
    //takes at most two input arguments
    // infinite loop but ^C quits
    signal(SIGCHLD, handle_sigchld);

    while (1) {
        printf("SHELL%% ");
        fgets(command, 82, stdin);
        command[strlen(command) - 1] = '\0';//remove the \n
        int len_1;
        for(len_1 = 0;command[len_1] != '\0';len_1++){
            if(command[len_1] == ' ')
                break;
        }
        parsed_command[0] = command;
        if(len_1 == strlen(command)){
            printf("Command is '%s' with no arguments\n", parsed_command[0]); 
            parsed_command[1] = NULL;


            if(parsed_command[0][0]=='.'&&parsed_command[0][1]=='/'){
                printf("program: %s\n",parsed_command[0]);
                execlp(parsed_command[0],parsed_command[1],NULL);
            }
            else if (parsed_command[0][0]=='B'&&parsed_command[0][1]=='G'){
                int status;
                char *fi = parsed_command[0]+2;
                pid_t pid = fork();
                if (pid < 0) {
                    perror("Fork failed.\n");
                    exit(1);  // nonzero means a failure result in unix
                }
                else if(pid==0){
                    int fork2 = fork();
                    if(fork2 == 0){ 
                        execlp(fi,fi,parsed_command[1],NULL);   
                    }else {
                        wait(NULL);
                    }
                    
                } 
            }
        }else{
            command[len_1] = '\0';
            parsed_command[1] = command + len_1 + 1;
            printf("Command is '%s' with argument '%s'\n", parsed_command[0], parsed_command[1]); 
            command[0]='\0';
        }
        


    }
}
