// Reader Program
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define FOO 4096

void sigHandler(int);

const int SIZE = 64;
int shmId;
char *shmPtr;

int main () 
{ 
	int key = ftok("key.txt", 42); 
	
	
	if ((shmId = shmget (key, FOO, S_IRUSR|S_IWUSR)) < 0) { 
		perror ("i can't get no..\n"); 
		exit (1); 
	} 
	if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) { 
		perror ("can't attach\n"); 
		exit (1); 
	}
	
	while(1) {
		// Get Flags
		// read from shared memory
		// fprints 
		// Change flags
	}
	
	return 0;
}

void sigHandler(int sigNum)
{
	if(sigNum == SIGINT) {
		if (shmdt (shmPtr) < 0) { 
			perror ("just can't let go\n"); 
			exit (1); 
		} 
		
		if (shmctl (shmId, IPC_RMID, 0) < 0) { 
			perror ("can't deallocate\n"); 
			exit(1); 
		}
		
		printf("Shutting down...\n");
		
		exit(0);
	}
}