#include "Funkcje/ZmienneSrodowiskowe.h"
#include "Funkcje/FunkcjeKolejkiKomunikatow.h"
#include <sys/wait.h>
#include <pthread.h>

struct message CiezarowkiGotowe = { .mtype = 2 };
struct message CiezarowkiSkonczylyPrace = { .mtype = 4 };

// Wątek zbierający zakończone procesy potomne (zombie)
void* collectZombie(void *arg)
{
	int zombieCounter = 0;
	while (zombieCounter != N)
	{
		while (wait(NULL) == -1);
		zombieCounter++;
	}
}

// Pusta obsługa sygnału SIGUSR2
void ignore_signal(int sig) 
{
    // Nic nie rób
}


int main()
{
    // Tworzymy nową grupę procesów
    pid_t parent_pid = getpid();
	if (setpgid(parent_pid, parent_pid) == -1) 
	{
    	perror("setpgid (parent) failed");
    	exit(1);
	}   

    // Tworzymy wątek do zbierania zombie
    pthread_t zombieCollector; 
	if (pthread_create(&zombieCollector, NULL, collectZombie, NULL) != 0)
	{
		perror("pthread_create error");
		exit(3);
	}

    // Tworzymy N procesów ciężarówek
    for (int i = 0; i < N; i++)
    {
        switch(fork())
        {
            case -1:
                perror("Fork failed");
                exit(1);
            case 0: // Proces potomny
                execl("./Ciezarowka", "Ciezarowka", NULL);
                perror("Execl Failed");
                exit(2);
        }
    }
    
    // Łączymy się z kolejką i informujemy dyspozytor o gotowości
    int kolejkaKomunikatow = create_message_queue(".", 'A', IPC_CREAT | 0600);
    printf("\033[1;32m[%d] Ciężarówki ~ Ciężarówki Gotowe!\033[0m\n", getpid());
    CiezarowkiGotowe.pidProcesu = (int)getpgrp(); // ID grupy procesów
    send_message(kolejkaKomunikatow, &CiezarowkiGotowe, 0);
    
    // Ignorujemy SIGUSR2 (tylko procesy potomne kończą pracę)
    signal(SIGUSR2, ignore_signal);

    // Czekamy aż wszystkie procesy potomne się zakończą
    if (pthread_join(zombieCollector, NULL) != 0)
	{
		perror("pthread join error");
		exit(7);
	}

    // Informujemy dyspozytor o zakończeniu pracy
    printf("\033[1;32m[%d] Ciezarowki ~ Ciezarowki Zakończyły Pracę!\033[0m\n", getpid());
    send_message(kolejkaKomunikatow, &CiezarowkiSkonczylyPrace, 0);

    return 0;
}
