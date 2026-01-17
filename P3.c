#include "Funkcje/ZmienneSrodowiskowe.h"
#include "Funkcje/FunkcjePamieciDzielonej.h"
#include "Funkcje/FunkcjeKolejkiKomunikatow.h"
#include "Funkcje/FunkcjeSemafory.h"
#include "Funkcje/FunkcjeObslugiTasmy.h"

#define MasaCegly 3
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
            printf("\033[1;34m[%d] P3 ~ Wrzuciłem cegłe o masie %dkg na taśme.\033[0m\n", getpid(), MasaCegly);
            sleep(T1);
        }
        signal_semafor(semaforTasmy, 0, 0);
    }
    printf("\033[1;34m[%d] P3 ~ Kończę pracę na dziś.\033[0m\n", getpid());
    return 0;
}
