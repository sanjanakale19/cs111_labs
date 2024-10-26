#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

void close_helper(int fd) {
	if (fd >= 0) {
		if (close(fd) < 0) { 
			perror(NULL); 
			exit(1); 
		}
	}
}

int main(int argc, char *argv[])	// argc = numprocs + 1
{
	int num = argc - 1;

	if (num == 0) {
		// if no process names passed in, exit
		errno = EINVAL;
		exit(1);
	}

	int pipe0[2] = {-1, -1};
	int pipe1[2] = {-1, -1};
		
	int *pipe_prev = pipe0;
	int *pipe_next = pipe1;

	for (int i = 1; i <= num; i++) {
		// if i == num, does not need pipe_next
		// if i == 1, don't need to assign pipe_prev = pipe_next of last run
		if (i < num) {
			// make a pipe on the right side
			int val = pipe(pipe_next);
			if (val < 0) {
				perror(NULL);
				exit(1);
			}
		}
		
		// fork
		int rc = fork();

		if (rc < 0) {		// exit if invalid
			perror(NULL);
			exit(1);
		} else if (rc == 0) { 	// child code: context switch to child
			// do fclose on b's previous out and b's next in
			// dup2() to connect b stdin to in prev and b stdout to out next

			if (i > 1) {
				// prev/left
				close_helper(pipe_prev[1]);
				if (dup2(pipe_prev[0], STDIN_FILENO) < 0) { perror(NULL); exit(1); }
			}

			if (i < num) {
				// next/right
				close_helper(pipe_next[0]);
				if (dup2(pipe_next[1], STDOUT_FILENO) < 0) { perror(NULL); exit(1); }
			}

			if (execlp(argv[i], argv[i], NULL) < 0) { perror(NULL); exit(1); }

		} else { 				// parent code (pipe)
			// close pipes from the parent, only close whatever's assigned to prev
			close_helper(pipe_prev[0]);
			close_helper(pipe_prev[1]);

			pipe_prev[0] = -1;
			pipe_prev[1] = -1;

			// swap the two pointers - most importantly, next becomes the new prev
			int *temp = pipe_prev;
			pipe_prev = pipe_next;
			pipe_next = temp;
		}
	}

	// after the swap, close the previous pipe (the old )
	close_helper(pipe_prev[0]);
	close_helper(pipe_prev[1]);

	int ret_val = 0;

	for (int j = 0; j < num; j++) {
		int status = 0;
		wait(&status);

		if (status != 0 && ret_val == 0) {
			ret_val = 1;
		}
	}

	return ret_val;
}

/*

rough pseudocode/thought process:

in main()

pipe()
get stdin args as array of strings (each is a process name)
if argc < 2 {only pipe, exit with errno: EINVAL in stderr of pipe}
n = numprocs

int pipe0[2];
int pipe1[2];

int *pipe_l;
int *pipe_r;

for loop: start at i = 1 until argc - 1: argc - 1 total processes, not including pipe
	// check if current i has a following process, then make another pipe
	p a b c
		create pipe 1 for a to b
		create a
		keep pipe 1 open
			pipe 1 = a's next

		create pipe 2 for b to c
		create b
		reassign pipe 1 stuff
		close pipe 1
			pipe 1 = b's prev
			only close the prev pipe in all cases
			next pipe, make prev point to next
			make next point to prev


		create c
		reassign pipe 2
		close pipe 2
			pipe 2 = c's prev

	

	int rc = fork();

	if (rc < 0) {exit(1)}
	else if(rc == 0) { // child code

	}
	else { //parent code (pipe)
		close all pipes from this loop
	}	
}




if (argc == 2) {
		int rc = fork();
		case rc < 0:
		case rc == 0: // child
			close both pipes
			execlp()
		case rc > 0: // parent
			close both pipes
			wait()
	}

	if !(n = 1 or n = arc - 1) {pipe()}


*/


/*
printf("hello world (pid:%d)\n", (int) getpid());
	int rc = fork();
	if (rc < 0) { // fork failed; exit
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (rc == 0) { // child (new process)
		// printf("hello, I am child (pid:%d)\n", (int) getpid());
		execlp("wc", "wc", "pipe.c", NULL); // runs word count
		printf("this shouldn’t print out");
	} else { // parent goes down this path (main)
		int rc_wait = wait(NULL);
		// printf("hello, I am parent of %d (rc_wait:%d) (pid:%d)\n",
		// rc, rc_wait, (int) getpid());
	}
	return 0;
*/