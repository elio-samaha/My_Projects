CC = gcc
CFLAGS = -Wall -g

all: main myGit

%.o: %.c
	$(CC) $(CFLAGS) -c $<

main : main.o outils.o workfile_worktree.o commit.o arborescence.o
	$(CC) $(CFLAGS) -o main $^

myGit : myGit.o outils.o workfile_worktree.o commit.o arborescence.o fusion.o
	$(CC) $(CFLAGS) -o myGit $^

clean:
	rm -rf *.o $(all)

clean_o:
	rm -rf *.o

clean_ex:
	rm -rf $(all)




