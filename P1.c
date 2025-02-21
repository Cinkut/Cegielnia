#include "Funkcje/ZmienneSrodowiskowe.h"
#include "Funkcje/FunkcjePamieciDzielonej.h"
#include "Funkcje/FunkcjeKolejkiKomunikatow.h"
#include "Funkcje/FunkcjeSemafory.h"
#include "Funkcje/FunkcjeObslugiTasmy.h"

#define MasaCegly 1
int PracaTrwa = 1;


void sygnalDyspozytoraDwa_handler(int signal)
{
    PracaTrwa = 0;
}


int main()
{
    int sharedMemoryID = create_shared_memory(".", 'B', K, IPC_CREAT | 0600);
    int* tasma = (int*)attach_shared_memory(sharedMemoryID, NULL, 0);
    int semaforTasmy = create_semafor(".", 'C', 1, IPC_CREAT | 0600);

    signal(SIGUSR2, sygnalDyspozytoraDwa_handler);

    while (PracaTrwa)
    {
        while (wait_semafor(semaforTasmy, 0, 0))
            if (!PracaTrwa)
                break;

        if (sprawdzCzyMoznaWrzucicCegle(tasma, K, MasaCegly, M))
        {
            wrzucCegleNaTasme(tasma, K, MasaCegly);
            printf("[%d] P1 ~ Wrzuciłem cegłe o masie %dkg na taśme.\n", getpid(), MasaCegly);
            sleep(T1);
        }
        signal_semafor(semaforTasmy, 0, 0);
    }

    return 0;
}
