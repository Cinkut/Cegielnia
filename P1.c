#include "Funkcje/ZmienneSrodowiskowe.h"
#include "Funkcje/FunkcjePamieciDzielonej.h"
#include "Funkcje/FunkcjeKolejkiKomunikatow.h"
#include "Funkcje/FunkcjeSemafory.h"
#include "Funkcje/FunkcjeObslugiTasmy.h"

#define MasaCegly 1 // P1 produkuje cegły o masie 1
static volatile sig_atomic_t PracaTrwa = 1;

// Obsługa SIGUSR2 - zakończ pracę
void sygnalDyspozytoraDwa_handler(int signal)
{
    PracaTrwa = 0;
}


int main()
{
    // Podłączenie do pamięci dzielonej (taśma)
    int sharedMemoryID = create_shared_memory(".", 'B', K * sizeof(int), IPC_CREAT | 0600);
    int* tasma = (int*)attach_shared_memory(sharedMemoryID, NULL, 0);
    // Podłączenie do semafora taśmy
    int semaforTasmy = create_semafor(".", 'C', 1, IPC_CREAT | 0600);

    signal(SIGUSR2, sygnalDyspozytoraDwa_handler);

    while (PracaTrwa)
    {
        // Czekamy na dostęp do taśmy
        while (wait_semafor(semaforTasmy, 0, SEM_UNDO))
            if (!PracaTrwa)
                break;

        // Sprawdzamy czy można wrzucić cegłę (miejsce i udźwig)
        if (sprawdzCzyMoznaWrzucicCegle(tasma, K, MasaCegly, M))
        {
            wrzucCegleNaTasme(tasma, K, MasaCegly);
            printf("\033[1;34m[%d] P1 ~ Wrzuciłem cegłe o masie %dkg na taśme.\033[0m\n", getpid(), MasaCegly);
            sleep(T1); // Czas produkcji kolejnej cegły
        }
        // Zwalniamy semafor
        while(signal_semafor(semaforTasmy, 0, SEM_UNDO))
            if(!PracaTrwa)
                break;
    }
    printf("\033[1;34m[%d] P1 ~ Kończę pracę na dziś.\033[0m\n", getpid());
    return 0;
}
