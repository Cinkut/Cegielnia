#include "Funkcje/ZmienneSrodowiskowe.h"
#include "Funkcje/FunkcjeKolejkiKomunikatow.h"
#include <sys/wait.h>
#include <pthread.h>

struct message CiezarowkiGotowe = { .mtype = 2 };
struct message CiezarowkiSkonczylyPrace = { .mtype = 4 };

void* collectZombie()
{
	int zombieCounter = 0;
	while (zombieCounter != N)
	{
		while (wait(NULL) == -1);
		zombieCounter++;
	}
}

void ignore_signal(int sig) 
{
    // Nic nie rób
}


int main()
{
    pid_t parent_pid = getpid();
	if (setpgid(parent_pid, parent_pid) == -1) 
	{
    	perror("setpgid (parent) failed");
    	exit(1);
	}   

    pthread_t zombieCollector; 
	if (pthread_create(&zombieCollector, NULL, collectZombie, NULL) != 0)
	{
		perror("pthread_create error");
		exit(3);
	}

    for (int i = 0; i < N; i++)
    {
        switch(fork())
        {
            case -1:
                perror("Fork failed");
                exit(1);
            case 0:
                execl("./Ciezarowka", "Ciezarowka", NULL);
                perror("Execl Failed");
                exit(2);
        }
    }
    int kolejkaKomunikatow = create_message_queue(".", 'A', IPC_CREAT | 0600);
    printf("[%d] Ciężarówki ~ Ciężarówki Gotowe!\n", getpid());
    CiezarowkiGotowe.pidProcesu = (int)getpgrp();
    send_message(kolejkaKomunikatow, &CiezarowkiGotowe, 0);
    
    signal(SIGUSR2, ignore_signal);

    if (pthread_join(zombieCollector, NULL) != 0)
	{
		perror("pthread join error");
		exit(7);
	}

    printf("[%d] Ciezarowki ~ Ciezarowki Zakończyły Pracę!\n", getpid());
    send_message(kolejkaKomunikatow, &CiezarowkiSkonczylyPrace, 0);

    return 0;
}