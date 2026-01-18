#include "Funkcje/ZmienneSrodowiskowe.h"
#include "Funkcje/FunkcjeKolejkiKomunikatow.h"
#include <sys/wait.h>
#include <pthread.h>

struct message PracownicyGotowi = { .mtype = 1 };
struct message PracownicySkonczyliPrace = { .mtype = 3 };

// Wątek zbierający zakończone procesy potomne (zombie)
void* collectZombie(void *arg)
{
	int zombieCounter = 0;
	while (zombieCounter != LiczbaPracownikow)
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
    // Programy i nazwy procesów dla trzech pracowników
    char *komendy[] = { "./P1", "./P2", "./P3" };
    char *nazwyProcesow[] = { "P1", "P2", "P3" };

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

    // Tworzymy 3 procesy dla pracowników P1, P2, P3
    for (int i = 0; i < LiczbaPracownikow; i++)
    {
        switch (fork())
        {
            case -1:
                perror("Fork Failed");
                exit(1);
            case 0: // Proces potomny
                execl(komendy[i], nazwyProcesow[i], NULL);
                perror("Execl Failed");
                exit(2);
        }
    }

    // Łączymy się z kolejką i informujemy dyspozytor o gotowości
    int kolejkaKomunikatow = create_message_queue(".", 'A', IPC_CREAT | 0600);
    PracownicyGotowi.pidProcesu = (int)getpgrp(); // ID grupy procesów
    printf("\033[1;34m[%d] Pracownicy ~ Pracownicy Gotowi.\033[0m\n", getpid());
    send_message(kolejkaKomunikatow, &PracownicyGotowi, 0);

    // Ignorujemy SIGUSR2 (tylko procesy potomne kończą pracę)
    signal(SIGUSR2, ignore_signal);
    
    // Czekamy aż wszystkie procesy potomne się zakończą
    if (pthread_join(zombieCollector, NULL) != 0)
	{
		perror("pthread join error");
		exit(7);
	}

    // Informujemy dyspozytor o zakończeniu pracy
    printf("\033[1;34m[%d] Pracownicy ~ Pracownicy Zakończyli Pracę!\033[0m\n", getpid());
    send_message(kolejkaKomunikatow, &PracownicySkonczyliPrace, 0);

    return 0;
}
