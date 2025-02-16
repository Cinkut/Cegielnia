#include "Funkcje/ZmienneSrodowiskowe.h"
#include "Funkcje/FunkcjePamieciDzielonej.h"
#include "Funkcje/FunkcjeKolejkiKomunikatow.h"
#include "Funkcje/FunkcjeSemafory.h"

int PracaTrwa = 1;

struct message PracownicyGotowi = { .mtype = 1 };
struct message CiezarowkiGotowe = { .mtype = 2 };
struct message PracownicySkonczyliPrace = { .mtype = 3 };
struct message CiezarowkiSkonczylyPrace = { .mtype = 4 };
struct message CiezarowkaMozeWjechac = { .mtype = 5 };
struct message CiezarowkaWjechala = { .mtype = 6 };

void sygnalDyspozytoraJeden_handler(int singal)
{

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

    signal(SIGUSR1, sygnalDyspozytoraJeden_handler);
    signal(SIGUSR2, sygnalDyspozytoraDwa_handler);

    printf("[%d] Dyspozytor ~ Cegielnia rozpoczyna pracę.\n", getpid());
    while (PracaTrwa)
    {

    }

    delete_message_queue(kolejkaKomunikatow);
    detach_shared_memory(tasma, sharedMemoryID);
    free_shared_memory(sharedMemoryID);

    printf("[%d] Dyspozytor ~ Cegielnia zakończyła pracę.\n", getpid());

    return 0;
}