CC = gcc

CFLAGS = -c -std=c11 -pedantic -W -Wall -Wextra 

.PHONY:all
all: start

start:

	sudo $(CC) listdir.c helper.h $(CFLAGS)
	$(CC) listdir.o -o ~bin/ufs_listdir	

	sudo $(CC) dirwalk.c  helper.h $(CFLAGS)
	$(CC) dirwalk.o -o ~bin/ufs_dirwalk	

	sudo $(CC) move.c helper.h $(CFLAGS)
	$(CC) move.o -o ~bin/ufs_move	

	sudo $(CC) delete.c  helper.h $(CFLAGS)
	$(CC) delete.o -o ~bin/ufs_delete	

	sudo $(CC) create.c  helper.h $(CFLAGS)
	$(CC) create.o -o ~bin/ufs_create	

	sudo $(CC) change_mode.c  helper.h $(CFLAGS)
	$(CC) change_mode.o -o ~bin/ufs_change_mode	

clean:
	rm -rf listdir.o dirwalk.o move.o delete.o create.o change_mode.o