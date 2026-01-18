#include "Funkcje/ZmienneSrodowiskowe.h"
#include "Funkcje/FunkcjeKolejkiKomunikatow.h"
#include <sys/wait.h>
#include <pthread.h>

struct message PracownicyGotowi = { .mtype = 1 };
struct message PracownicySkonczyliPrace = { .mtype = 3 };


void* collectZombie(void *arg)
{
	int zombieCounter = 0;
	while (zombieCounter != LiczbaPracownikow)
	{
		while (wait(NULL) == -1);
		zombieCounter++;
	}
}

void ignore_signal(int sig) 
{
    // Nic nie rób
}


// Proces nadrzędny uruchamiający pracowników P1/P2/P3.
int main()
{
    char *komendy[] = { "./P1", "./P2", "./P3" };
    char *nazwyProcesow[] = { "P1", "P2", "P3" };

    pid_t parent_pid = getpid();
    // Wspólna grupa procesów do sterowania sygnałami.
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

    for (int i = 0; i < LiczbaPracownikow; i++)
    {
        switch (fork())
        {
            case -1:
                perror("Fork Failed");
                exit(1);
            case 0:
                execl(komendy[i], nazwyProcesow[i], NULL);
                perror("Execl Failed");
                exit(2);
        }
    }

    // Informuje dyspozytora o gotowości pracowników.
    int kolejkaKomunikatow = create_message_queue(".", 'A', IPC_CREAT | 0600);
    PracownicyGotowi.pidProcesu = (int)getpgrp();
    printf("\033[1;34m[%d] Pracownicy ~ Pracownicy Gotowi.\033[0m\n", getpid());
    send_message(kolejkaKomunikatow, &PracownicyGotowi, 0);

    signal(SIGUSR2, ignore_signal);
    
    if (pthread_join(zombieCollector, NULL) != 0)
	{
		perror("pthread join error");
		exit(7);
	}


    printf("\033[1;34m[%d] Pracownicy ~ Pracownicy Zakończyli Pracę!\033[0m\n", getpid());
    send_message(kolejkaKomunikatow, &PracownicySkonczyliPrace, 0);

    return 0;
}
