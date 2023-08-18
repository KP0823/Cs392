//Kush Parmar
//I pledge my honor that I have abided by the Stevens honor System-Kush Parmar

#include "minishell.h"

volatile sig_atomic_t sig = 0; 

void our_sig_handler(int signal){
    sig=1;
}
int main(){
    // char input [BufMax];
    char path [PATH_MAX];
    char break_At[2]= " ";
    struct sigaction sa;
    sa.sa_handler=our_sig_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags=0;
    //this is our signal handeler and sets it to 0
    
    if(sigaction(SIGINT,&sa,NULL)<-1){
        fprintf(stderr, "Error: Cannot register signal handler. %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }
    while (1){
        sig=0;
        int size=0;
        if(getcwd(path,sizeof(path))==NULL){
            fprintf(stderr, "Error: Cannot get current working directory. %s.\n", strerror(errno));
            return EXIT_FAILURE;
        }
        else{
            printf("%s%s[%s]%s> ",DEFAULT,BLUE,path,DEFAULT);
        }
        //fflush(stdout);
        //while loop below reading characters one at at time
        //i did not use scanf or fscanf because the infinite loops of printing :(
        char* input = (char*) malloc(BufMax);
        char* copy= (char*) malloc(BufMax);
        char* change = (char*) malloc (PATH_MAX);
        char* go_to_path = (char*) malloc (PATH_MAX);
        while(1){
            char in = getchar();
            if(in=='\n'){
                input[size]='\0';
                size++;
                break;
            }
            input[size]=in;
            size++;
        }
        if(sig==1){
            free(go_to_path);
            free(input);
            free(copy);
            free(change);
            continue;
        }   
        memcpy(copy,input,size);
        char* token;
        //token will be used to break the string apart using strtok()
        token=strtok(copy,break_At);
        if(token==NULL){
            free(input);
            free(copy);
            free(change);
            free(go_to_path);
            continue;
        }
        //this give back the first word before the space   
        if(strcmp(token,"cd")==0){
            if((token=strtok(NULL,break_At))==NULL);
            else{
                memset(go_to_path,'\0',sizeof(strlen(go_to_path)));
                strcpy(go_to_path,token);
                //next part 
            }
            if((token==NULL ||strcmp(token,"~")==0) && (sig ==0)){
                //this is home directory
                uid_t uid= getuid();
                struct passwd* pwd=getpwuid(uid);
                if(pwd== NULL){
                    fprintf(stderr, "Error: Cannot get passwd entry. %s.\n", strerror(errno));
                }
                strcpy(change, pwd->pw_dir);
                change[strlen(change)]='\0';
                if(chdir(change)==-1){
                    fprintf(stderr, "Error: Cannot change directory to '%s'. %s.\n", token, strerror(errno));
                }
                free(go_to_path);
                strcpy(path,change);
                free(input);
                free(copy);
                free(change);
                token=NULL;
                // free(token);

            }
            else if(strlen(go_to_path)>0){
                //check to see too many parameters
                if( (token=strtok(NULL,break_At))!=NULL){
                    fprintf(stderr,"Error: Too many arguments to cd.\n");
                    free(go_to_path);
                    free(input);
                    free(copy);
                    free(change);
                    token=NULL;
                    continue;
                }
                else{
                    //all others 
                    uid_t uid= getuid();
                    struct passwd* pwd=getpwuid(uid);
                    if(pwd== NULL){
                        fprintf(stderr, "Error: Cannot get passwd entry. %s.\n", strerror(errno));
                    }
                    //checks if we are going through home
                    if(go_to_path[0]=='~'){
                        strcpy(change,pwd->pw_dir);
                        strcat(change,"/");
                        strcat(change,go_to_path+2);
                    }
                    else{
                        strcat(change,go_to_path);
                    }
                    change[strlen(change)]='\0';
                    if(chdir(change)==-1){
                        fprintf(stderr, "Error: Cannot change directory to '%s'. %s.\n", change, strerror(errno));
                    }
                    free(go_to_path);
                    strcpy(path,change);
                    free(input);
                    free(copy);
                    free(change);
                }
              }    
            }
            else if((strcmp(token,"exit"))==0){
                free(go_to_path);
                free(input);
                free(copy);
                free(change);
                break;
            }
            //need to ask him about the empty enter
            else{
                pid_t pid=fork();
                int stat;
                //checks if it can fork
                if(pid < 0){
                    fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
                    return EXIT_FAILURE;  
                 }
                //after forking check if can execute, it will if possible
                else if(pid==0 ){
                    char* args[BufMax];
                    int it=0;
                    while (token!=NULL){
                        args[it]=token;
                        it++;
                        token=strtok(NULL,break_At);
                    }
                    args[it]=NULL;
                    if((execvp(args[0],args))<0 && sig == 0){
                        fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
                        free(input);
                        free(copy);
                        free(go_to_path);
                        free(change);
                        token=NULL;
                        return EXIT_FAILURE;  
                    }
                }
                //waits if its not interupted 
                else{
                    if(waitpid(pid,&stat,0)<0 && sig!=1){ 
                            fprintf(stderr, "Error: wait() failed. %s.\n",strerror(errno));
                        }
                    }
                free(input);
                free(copy);
                free(go_to_path);
                free(change);
                token=NULL;
            }
            sig=0;
        }
        return (EXIT_SUCCESS);
    }