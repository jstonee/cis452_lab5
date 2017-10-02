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
char *shmPtr, *shmS, *i, *j, *turn;

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
	
		// setup flag pointers
	
	// First value, j, will be Flag Writer (T or F).
	// Next value, i, will be Flag Reader (T or F.
	// Third value, turn, will turn (W or R)
	j = shmS;
	i = shmS++;
	turn = shmS++;
	shmS++;
	
	while(1) {
		//Flag i = true
		*i = 'T';
		//Turn = Writer
		*turn = 'W';
		while(*j == 'T' && *turn == 'W');
		// read from shared memory (CS)
		printf("%s", shmS);
		// Change flags
		*i = 'F';
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