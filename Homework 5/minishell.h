//Kush Parmar
//I pledge my honor that I have abided  by the stevens honor system-Kush 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>

#define BLUE "\x1b[34;1m"
#define DEFAULT "\x1b[0m"
#define BufMax 65536 
//Bufmax is the total maximum size allowed in c for a char array

void our_sig_handler(int sig);