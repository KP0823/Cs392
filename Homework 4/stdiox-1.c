//Kush Parmar
//I pledge my honor that I have abided by the Stevens Honor System-Kush Parmar
#include "stdiox.h"

//converts into to a string
void int_To_string(int con, char* convert,int size,int negative){
    int counter = size-1;
    if(con ==0){
        convert[counter]='0';
    }
    else{
        while(con != 0 ){
            int digit = con %10; 
            char s= digit+'0';
            convert[counter]=s;
            counter--;
            con/=10;
        }
    }
    if(negative==1){ //checks if its a negative
        convert[0]='-';
    }
    convert[size]='\0'; // null terminator :)
}

void float_To_String(float con, char**convert){
    int int_part= (int) con;
    int negtive;
    int size_int=0;
    int size_dec=0;
    if(con <0){ //negative float check 
        int_part*=-1;
        negtive=1;
        size_int++;
    }
    else {  //not negative float
        negtive=0;  
    }
    int useless= con; 
    if(useless==0){
        size_int++;
    }
    else{
        while(useless!=0){ //length of int part of float
        size_int++;
        useless/=10;
        }
    }
    char half [size_int];
    int_To_string(int_part,half,size_int,negtive); //convert the first half
    if(negtive==1){ // getting the postive value of float
        con= (con * -1)-int_part;
    }
    else{
        con= con- int_part;
    }
    int counter_zero=0; //counting leading zeros
    int int_dec= (int )con;
    while(con-((float)int_dec)>0 ){ //goes through multiples by 10 to get size
        if(size_dec==6){ //shudong said 6th precision
            break;
        }
        if(size_dec>=0 && (int)con==0){
            counter_zero++;
            size_dec++;
            con=con*10;
            int_dec=(int)con;
        }
        else{
            con=con*10;
            int_dec=(int)con;
            size_dec++;
        }
    }
    float rounding= con-int_dec;
    rounding *=10;
    if((int) rounding >4 && size_dec==6){
        int_dec+=1;
    }
    size_dec++;
    char sndhalf[size_dec];
    sndhalf[0]='.';
    for(int i =1; i <counter_zero;i++){ //adds leading zeros
        sndhalf[i]='0';
    }
    int_To_string(int_dec,sndhalf,size_dec,0); //converts numbeer
    int convert_size=size_int+size_dec+1; 
    (*convert)=malloc(convert_size);
    strcpy((*convert),half); //appends all to one string
    strcat((*convert),sndhalf);
    (*convert)[convert_size-1]='\0'; //null terminator
}

int is_open(char* path_fd ){
    struct stat info;
    struct stat t;
    struct dirent* dirp;
    char path[PATH_MAX]="/proc/self/fd/";
    stat(path_fd,&info);
    int inode= info.st_ino; //inode number of file       
    DIR* dp=opendir(path);
    if((dirp=readdir(dp))==NULL){
        closedir(dp);
        return -1;
    }
    while((dirp=readdir(dp))!=NULL){
        if(strcmp(dirp->d_name,".")!=0 &&strcmp(dirp->d_name,"..")!=0){ //avoid "." ".."" files
            char d [PATH_MAX]; 
            strcpy(d,path); 
            strcat(d,dirp->d_name); //get path of file in dirent
            char z [PATH_MAX];
            realpath(d,z);      //gets realpath of file in fd
            stat(z,&t);
            int each_i=t.st_ino;    //gets inode of fd file
            if(inode==each_i){  //eachs if they are equal
                int z= atoi(dirp->d_name);
                closedir(dp);                
                return z;
            }
        }
    }
    closedir(dp);
    return -1;  
}

int fprintfx(char* filename, char format, void* data){
    int fd;
    char newL='\n';
    if(data==NULL){ //checks if data is null
        errno=EIO;
        return -1;
    }
    //check if we are using stdout
    if (strcmp(filename,"")==0){    
        fd=1;
    }
    else{
        char buf [PATH_MAX];   
        realpath(filename,buf);
        fd = open(buf, O_WRONLY| O_APPEND |O_CREAT, 0640); //open file
    }
    if (format == 'd'){ //format d case
        int con = (*((int* ) data));
        int negtive;
        int size=0;
        if(con <0){ //negative check
            con= abs(con);
            negtive=1;
            size++;
        }
        else {  //not negative
            negtive=0;  
        }
        int useless= con;
        if(useless==0){
            size++;
        }
        else{
            while(useless!=0){
                size++;
                useless/=10;
            }
        }
        int wri= size +1;
        char written[wri];
        int_To_string(con,written,size,negtive);    //calls function
        write(fd, written, strlen(written));        //writes
        write(fd,&newL,1);
        return 0; 
        
    }
    if(format == 's'){  //string case
        write(fd,data,strlen(( (char*) data )));
        write(fd,&newL,1);
        return 0;
    }
    if(format =='f'){
        char* ftos;
        float send= *((float*) data);
        float_To_String(send,&ftos);
        write(fd,ftos,strlen(ftos));
        write(fd,&newL,1);
        free(ftos);
    } 
    else{       //any other format
        errno=EIO;
        return -1;
    }
    return 0;    
}
// we are going to have to mod for ints good luck 
int fscanfx(char* filename, char format, void* dst){
    int fd;
    int read_size= 128;
    char read_i;
    if (dst==NULL){
        return 0;
    }
    if (strcmp(filename,"")==0){    //stdin
        fd=0;
    }
    else{        
        char buf [PATH_MAX];
        realpath(filename,buf);
        int check_is_open = is_open(buf); //checks if it is open
        if(check_is_open!=-1){
            fd= check_is_open;
        }
        else{   //else just open the file
            fd = open(buf, O_RDONLY);
            if (fd==-1){
                errno=ENOENT;
                return -1;
            }
        }
    }
    if(fd != 1){
        int cur= lseek(fd,0,SEEK_CUR);
        int end= lseek(fd,0,SEEK_END);
        if(cur==end){   //checking if we at the EOF
            return -1;
        }
        lseek(fd,cur,SEEK_SET);
    }
    int index=0;  //index of buffer
    char* read_C = (char*) malloc(read_size);
    while(1){   //read byte by byte
        int r= read(fd,&read_i,1);
        if(r<0){        //check if get an error reading
            free(read_C);
            errno=EIO;
            return -1;
        }
        if(r==0){       //nothing read
            break;
        }
        if (index==read_size){
            read_size+=128;
            read_C= (char*)realloc(read_C,read_size);
        }
        if(read_i=='\n' || read_i=='\0'){   //stop and new line and null terminator
            read_C[index]='\0';
            index++;
            break;
        }
        read_C[index]=read_i;
        index++;
    }
    if (format == 'd'){     //d format
        int to_ret= atoi(read_C);
        *((int*)(dst))=to_ret;
    }
    else if(format == 's'){ //s format
        strcpy(dst,read_C);
    }
    else if(format== 'f'){
        float to_ret =atof(read_C);
        *((float*)(dst))=to_ret;
    }
    else{
        free(read_C);       // free buffer
        errno=EIO;
        return -1;
    }
    free(read_C);
    return 0;   
}
int clean(){
    struct dirent* dirp;
    char path[PATH_MAX]="/proc/self/fd/";  //fd dir            
    DIR* dp=opendir(path);
    if((dirp=readdir(dp))==NULL){ //check if there is anything there
        errno=EIO;
        return -1;
    }
    while((dirp=readdir(dp))!=NULL){        
        if(strcmp(dirp->d_name,".")!=0 && strcmp(dirp->d_name,"..")!=0){
            int at= atoi(dirp->d_name);
            if(at==0 || at==1 || at==2){
                continue;
            }
            else if(at>=3 && at<1024){
                if (close(at)==-1){ //checks if we closed file sucessfully
                      closedir(dp);
                      errno=EIO;
                      return -1;
                }
            }
        }
    }
    closedir(dp);
    return 0;
}
