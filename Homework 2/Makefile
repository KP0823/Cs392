#Kush Parmar
#I pledge my honor that I have abided by the stevens honor system- Kush Parmar
CC = gcc
CFLAGS = -c -Wall

all: main.o insertion.o utils.o
	$(CC) main.o insertion.o utils.o -o a.out

main.o: main.c
	$(CC) main.c $(CFLAGS)

insertion.o: insertion.c
	gcc insertion.c -c 

utils.o: utils.c
	gcc utils.c -c

clean:
	rm *.o 