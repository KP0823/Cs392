//Kush Parmar 
//I pledege my honor that I have abided by the stevens honor system- Kush Parmar
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <limits.h> /* PATH_MAX love stackoverflow */

char* pstring;
int perms [9] = {S_IRUSR, S_IWUSR, S_IXUSR, 
                 S_IRGRP, S_IWGRP, S_IXGRP, 
                 S_IROTH, S_IWOTH, S_IXOTH}; // just makes life easier


//checks if inputed pstring is valid or not
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
//checks creates the permission string for a file
char* create_perm_string(struct stat* stat_info){
    char* perm_string= (char*)malloc(10);
    for(int i=0; i<9; i+=3){
        if(((stat_info->st_mode) & perms[i])>0){
            *(perm_string+i)='r';
        }
        else{
            *(perm_string+i)='-';        
        }

        if(((stat_info->st_mode) & perms[i+1])>0){
            *(perm_string+(i+1))='w';
        }
        else{
            *(perm_string+(i+1))='-';            
        }

        if(((stat_info->st_mode) & perms[i+2])>0){
            *(perm_string+(i+2))='x';
        }
        else{
            *(perm_string+(i+2))='-';            
        } 
    }
    *(perm_string+9)='\0';
    return perm_string;
}
void read_directory(char* direct_path){
    struct stat info;
    DIR *dp;
    struct dirent *dirp;
    /* Open directory, and exit if DIR object returned is NULL*/
    dp = opendir(direct_path);
    if (dp == NULL) {
        fprintf(stderr,"Error: Cannot open directory '%s'. Permission denied.\n", direct_path);
        exit(EXIT_FAILURE);
    }
    else{
        char filename[PATH_MAX];   //creates an char array that is allocated to max amount of character for a path
        strncpy(filename,direct_path,PATH_MAX); //copy the path over
        int len= strlen(filename); 
        filename[len]='/'; //adds the dash just in case there is a file, for a folder it won't really matter for a file
        len= strlen(filename);
        /* Use readdir in a loop until it returns NULL */
        while ((dirp = readdir(dp)) != NULL) {
            if(strcmp(dirp->d_name,".")==0||strcmp(dirp->d_name,"..")==0){
                continue;
            }
            strncpy(filename+len,dirp->d_name, PATH_MAX-len); //finishes path 
            stat(filename,&info);
            char* perm= create_perm_string(&info);
            if(dirp->d_type== DT_DIR && (strcmp(perm,"---------")==0)){
                fprintf(stderr,"Error: Cannot open directory '%s'. Permission denied.\n", filename);
                continue;
            }
            if(strcmp(perm,pstring)==0){    //compare pstring to made permission string
                printf("%s\n",filename);
            }
            if(dirp->d_type== DT_DIR){      //if its a folder then recurse
                read_directory(filename);
            }
            free(perm);
        } 
        closedir(dp);
    }
}
int main(int argc, char* argv[]) {
    if(valid_string(argv[2])==1){
        fprintf(stderr,"Error: Permissions string '%s' is invalid.\n",argv[2]);
        exit(1);
    }
    pstring=argv[2];
    read_directory(argv[1]);
	exit(EXIT_SUCCESS);

}