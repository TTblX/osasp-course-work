CC = gcc

CFLAGS = -c -std=c11 -pedantic -W -Wall -Wextra 

.PHONY:all
all: start

start:

	sudo $(CC) main.c utils.c  $(CFLAGS)
	$(CC) main.o -o ~bin/ufs_si	

clean:
	rm -rf main.o utils.o