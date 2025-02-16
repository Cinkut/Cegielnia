#include "Funkcje/ZmienneSrodowiskowe.h"
#include "Funkcje/FunkcjeKolejkiKomunikatow.h"

struct message PracownicyGotowi = { .mtype = 1 };
struct message PracownicySkonczyliPrace = { .mtype = 3 };


int main()
{
    char *komendy[] = { "./P1", "./P2", "./P3" };
    char *nazwyProcesow[] = { "P1", "P2", "P3" };

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

    int kolejkaKomunikatow = create_message_queue(".", 'A', IPC_CREAT | 0600);
    send_message(kolejkaKomunikatow, &PracownicyGotowi, 0);

    return 0;
}