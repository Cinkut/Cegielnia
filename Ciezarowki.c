#include "Funkcje/ZmienneSrodowiskowe.h"
#include "Funkcje/FunkcjeKolejkiKomunikatow.h"

struct message CiezarowkiGotowe = { .mtype = 2 };
struct message CiezarowkiSkonczylyPrace = { .mtype = 4 };


int main()
{
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
    send_message(kolejkaKomunikatow, &CiezarowkiGotowe, 0);
    
    return 0;
}