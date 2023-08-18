#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

//Kush Parmar 
//I pledge my honor that I have abided by the Stevens Honor System-Kush Parmar
int valid_string(const char* pstring){
    if(strlen(pstring)!=9) { return 1;}

    for(int i=0; i <9; i+=3){
        if(pstring[i]!='-' &&pstring[i]!='r'){
            return 1;
        }
        if(pstring[i+1]!='-' &&pstring[i+1]!='w'){
            return 1;
        }
        if(pstring[i+2]!='-' &&pstring[i+2]!='x'){
            return 1;
        }
    }
    return 0;
}

int main(int argc, char* argv[]){
    if(argc!=3){
        fprintf(stdout,"Usage: ./spfind <directory> <permissions string>\n");
        exit(0);
    }
    if(valid_string(argv[2])==1){
        fprintf(stderr,"Error: Permissions string '%s' is invalid.\n",argv[2]);
        exit(1);
    }
    int pfind_pipe[2]; //0 should be closed don't need it for the pfind, need it for sort, 1 should be dup2 to stdout, close for sort
    int sort_pipe[2];  //0 should be closed for pfind and sort, only at the end to print, 1 should dup2 and change stdout, closed in sort
    char * fun[4];
    fun[0]="./pfind";
    fun[1]=argv[1];
    fun[2]=argv[2];
    fun[3]=NULL;
    if(pipe(pfind_pipe)== -1){      ///creates pipe
        fprintf(stderr,"Error: pfind pipe failed");
        return EXIT_FAILURE;  
    } 
    if(pipe(sort_pipe)== -1){   //creates pipe
        fprintf(stderr,"Error: sort pipe failed");
        return EXIT_FAILURE;  
    }
    pid_t first_fork = fork();  //pfind fork process
    if(first_fork==-1){     //pfind fork error..
        wait(NULL);
        fprintf(stderr,"Error: pfind fork failed.\n");
        return EXIT_FAILURE;
    }
    if(first_fork==0){
        close(pfind_pipe[0]);
        if(dup2(pfind_pipe[1],STDOUT_FILENO)==-1){  //checks to see if dup2 can happen
            fprintf(stderr,"Error: pfind dup2 failed.\n");
            return EXIT_FAILURE;  
        }
        close(sort_pipe[0]);
        close(sort_pipe[1]);
        if(execvp("./pfind",fun)==-1){ //attempts to exec ./pfind
            fprintf(stderr,"Error: pfind exec failed.\n");
            return EXIT_FAILURE;
        }
        return 0;
    }
    pid_t second_fork= fork();  //sort fork
    if(second_fork==-1){
        wait(NULL);
        fprintf(stderr,"Error: sort fork failed.\n");   //sort fork error..
        return EXIT_FAILURE;
    }
    if(second_fork==0){
        if(dup2(pfind_pipe[0],STDIN_FILENO)==-1){    //checks to see if dup2 can happen
            fprintf(stderr,"Error: sort dup2 failed.\n");
            return EXIT_FAILURE;
        }
        close(pfind_pipe[1]);
        close(sort_pipe[0]);

        if(dup2(sort_pipe[1],STDOUT_FILENO)==-1){   //checks to see if dup2 can happen
            fprintf(stderr,"Error: sort dup2 failed.\n");
            return EXIT_FAILURE;  
        }
        if(execlp("sort","sort",NULL)==-1){     //attempts to run sort
            fprintf(stderr,"Error: sort failed.\n");
            return EXIT_FAILURE;
        }
        return 0;
    }
    //printing out from sort pipe
    close(pfind_pipe[0]);
    close(pfind_pipe[1]);
    close(sort_pipe[1]);
    wait(NULL);     //wait for pfind
    char reading;
    int r = 1;      
    int total_match=0;
    int check_newline=0;
    while(r==1){
        r=read(sort_pipe[0],&reading,1);
        if(check_newline==1 && reading=='\n'){ //reads one byte at at time
            continue;
        }
        if(reading=='\n'&& check_newline==0){   //prevents extra new line
            total_match++;
            check_newline=1;
        }
        else{
            check_newline=0;
        }
        printf("%c",reading);
    }
    printf("Total matches: %d\n",total_match);
    close(sort_pipe[0]);
    wait(NULL);     //wait for sort

    return EXIT_SUCCESS;
}