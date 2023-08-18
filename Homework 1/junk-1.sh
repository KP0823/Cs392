#!/bin/bash 
#Kush parmar 
#I pledge my honor that I have abided by the stevens honor system


readonly JUNKPATH=~/.junk
mkdir -p $JUNKPATH

USED_H=0    #counts h
USED_L=0    #counts l
USED_P=0    #counts p
USED_FLAG=0 #its a useless variable Shudong scammed me

#the function screen is mean to show the heredoc, also makes my life easier when trying to display this message
function screen { 

cat << EOF
Usage: $(basename $0) [-hlp] [list of files]
   -h: Display help.
   -l: List junked files.
   -p: Purge all files.
   [list of files] with no other arguments to junk those files.
EOF
}

#show screen then finsh
function run_h {
     screen >&1 
     exit 0
}

#list of files in the junk folder then exit
function run_l {
     ls -lAF $JUNKPATH	
     exit 0  
}

#delete files in the junk folder then exit
function run_p {
     rm -r $JUNKPATH* && mkdir $JUNKPATH
     exit 0	
}

#checks if there is a parameter that is not a flag, if so then return an error

# if there no parameter then just show the screen
if [[ $# = 0 ]]; then {
     screen >&1
}

#the readability of this code is non existent, but at the same time text edit is really ugly to work with 
else {
    declare -a array #just so i can store the flags and and then check how many flags are inputed
    ARRAYINDEX=0     #increments the array index i really don't need this but 
    while getopts ":hlp" option; do
        case "${option}" in
            h)
            	if [[ ($USED_H -ge 0) && ($USED_L -eq 0) && ($USED_P -eq 0) ]]; then {  #counts h
            	    USED_H=$(( USED_H+1 ))
            	    USED_FLAG=$(( USED_FLAG+1 ))
            	    array[ARRAYINDEX]=h
                    ARRAYINDEX=$(( ARRAYINDEX+1 ))
                } 
                else {
                    echo "Error: Too many options enabled." >&2
                    screen >&1
                    exit 1 
                }
                fi
            ;;
            l)
                if [[ ($USED_H -eq 0) && ($USED_L -ge 0) && ($USED_P -eq 0) ]]; then { #counts l
            	    USED_L=$(( USED_L+1 ))	
            	    USED_FLAG=$(( USED_FLAG+1 ))
                    array[ARRAYINDEX]=l
                    ARRAYINDEX=$(( ARRAYINDEX++ ))
                }
                else {
                    echo "Error: Too many options enabled.">&2
                    screen >&1
                    exit 1 
                }
                fi

            ;;
            p)
                if [[ ($USED_H -eq 0) && ($USED_L -eq 0) && ($USED_P -ge 0) ]]; then { #counts p
                    USED_P=$(( USED_P+1 ))	
                    USED_FLAG=$(( USED_FLAG+1 ))
                    array[ARRAYINDEX]=p
                    ARRAYINDEX=$(( ARRAYINDEX+1 ))			
                } 
                else {
                    echo "Error: Too many options enabled.">&2 
                    screen >&1
                    exit 1 
                }
                fi
               
            ;;
            *)									      #everything else
                echo "Error: Unknown option '-${OPTARG}'." >&2
                screen >&1
                exit 1
            ;;
        esac
    done
    }
fi

#Identifies which output to produce, while also checking to make sure there is no file
if [[ (($USED_H -gt 0) && ($USED_L -eq 0) && ($USED_P -eq 0)) ]]; then {
	 for i in $@; do 
	  if [[ $i != -* ]]; then {
	       echo "Error: Too many options enabled.">&2 
	       screen >&1
	       exit 1
	  }
	  fi
          done
	 run_h
}
elif [[ (($USED_H -eq 0) && ($USED_L -gt 0) && ($USED_P -eq 0)) ]]; then {
	for i in $@; do 
	  if [[ $i != -* ]]; then {
	       echo "Error: Too many options enabled.">&2 
	       screen >&1
	       exit 1
	  }
	  fi
          done
	run_l
}
elif [[ (($USED_H -eq 0) && ($USED_L -eq 0) && ($USED_P -gt 0)) ]]; then {
	for i in $@; do 
	  if [[ $i != -* ]]; then {
	       echo "Error: Too many options enabled.">&2 
	       screen >&1
	       exit 1
	  }
	  fi
          done
	run_p
}
#moves files and folders
else {
    for i in $@; do 
         if [[ -e "$i" ]]; then {
	    mv $i $JUNKPATH
         }
         else {
	    echo "Warning: '$i' not found.">&2
         }  	
         fi
         done
         exit 0    
}
fi
exit 0


