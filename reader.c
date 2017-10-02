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

#ifndef DEBUG
	#define DEBUG 0
#endif

#define FOO 4096

void sigHandler(int);

const int SIZE = 64;
int shmId;
char *shmPtr, *shmS;

int main () 
{ 
	
	struct sigaction sa;	
	sa.sa_handler = &sigHandler;
	sa.sa_flags = SA_RESTART;
	sigfillset(&sa.sa_mask);
	
	//initialize sigHandler
	if(sigaction(SIGINT, &sa, NULL) == -1) {
		perror("ERROR: SIGNAL Handler failed");
	}
	
	key_t key;
	if ((key = ftok("key.txt", 42) ) == (key_t) -1) {
		perror("ftok error");
		exit(1);
	}
	
	if ((shmId = shmget (key, FOO, S_IRUSR|S_IWUSR)) < 0) { 
		perror ("i can't get no..\n"); 
		exit (1); 
	} 
	if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) { 
		perror ("can't attach\n"); 
		exit (1); 
	}
	
	shmS = shmPtr;
	#if (DEBUG == 1)
		printf("%s\n", shmS);
		fflush(stdout);
	#endif
	
	while(1) {
		// Get Flags
		// read from shared memory i.e. printf("%s\n", shmS);
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
		
		printf(" Reader Shutting down...\n");
		
		exit(0);
	}
}	