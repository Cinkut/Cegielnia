#include "Funkcje/ZmienneSrodowiskowe.h"
#include "Funkcje/FunkcjePamieciDzielonej.h"
#include "Funkcje/FunkcjeKolejkiKomunikatow.h"
#include "Funkcje/FunkcjeSemafory.h"

int PracaTrwa = 1;
int pidZaladowywanejCiezarowki;
struct message PracownicyGotowi = { .mtype = 1 };
struct message CiezarowkiGotowe = { .mtype = 2 };
struct message PracownicySkonczyliPrace = { .mtype = 3 };
struct message CiezarowkiSkonczylyPrace = { .mtype = 4 };
struct message CiezarowkaMozeWjechac = { .mtype = 5 };
struct message CiezarowkaWjechala = { .mtype = 6 };
struct message CiezarowkaOdjechala = { .mtype = 7 };

void sygnalDyspozytoraJeden_handler(int singal)
{
    printf("\033[1;31m[%d] Dyspozytor ~ Szybszy odjazd ciężarówki.\033[0m\n", getpid());
    printf("%d\n", pidZaladowywanejCiezarowki);
    kill(pidZaladowywanejCiezarowki, SIGUSR1);
}

void sygnalDyspozytoraDwa_handler(int signal)
{
    PracaTrwa = 0;
}


int main()
{
    int kolejkaKomunikatow = create_message_queue(".", 'A', IPC_CREAT | 0600);

    recive_message(kolejkaKomunikatow, &PracownicyGotowi, 1, 0);
    recive_message(kolejkaKomunikatow, &CiezarowkiGotowe, 2, 0);
    
    int sharedMemoryID = create_shared_memory(".", 'B', K, IPC_CREAT | 0600);
    int* tasma = (int*)attach_shared_memory(sharedMemoryID, NULL, 0);

    int semaforTasmy = create_semafor(".", 'C', 1, IPC_CREAT | 0600);
    initialize_semafor(semaforTasmy, 0, 1);

    signal(SIGUSR1, sygnalDyspozytoraJeden_handler);
    signal(SIGUSR2, sygnalDyspozytoraDwa_handler);

    printf("\033[1;31m[%d] Dyspozytor ~ Cegielnia rozpoczyna pracę.\033[0m\n", getpid());
    while (PracaTrwa)
    {
        printf("\033[1;31m[%d] Dyspozytor ~ Ciężarówka może wjeżdżać.\033[0m\n", getpid());
        send_message(kolejkaKomunikatow, &CiezarowkaMozeWjechac, 0);
        recive_message(kolejkaKomunikatow, &CiezarowkaWjechala, 6, 0);
        pidZaladowywanejCiezarowki = CiezarowkaWjechala.pidProcesu;

        while (recive_message(kolejkaKomunikatow, &CiezarowkaOdjechala, 7, 0))
        {
            if (!PracaTrwa)
                break;
            continue;
        }
    }
    kill(-(PracownicyGotowi.pidProcesu), SIGUSR2);
    kill(-(CiezarowkiGotowe.pidProcesu), SIGUSR2);
    recive_message(kolejkaKomunikatow, &PracownicySkonczyliPrace, 3, 0);
    recive_message(kolejkaKomunikatow, &CiezarowkiSkonczylyPrace, 4, 9);

    delete_message_queue(kolejkaKomunikatow);
    detach_shared_memory(tasma, sharedMemoryID);
    free_semafor(semaforTasmy);
    free_shared_memory(sharedMemoryID);

    printf("\033[1;31m[%d] Dyspozytor ~ Cegielnia zakończyła pracę.\033[0m\n", getpid());

    return 0;
}