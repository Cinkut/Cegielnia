#include "Funkcje/ZmienneSrodowiskowe.h"
#include "Funkcje/FunkcjePamieciDzielonej.h"
#include "Funkcje/FunkcjeKolejkiKomunikatow.h"
#include "Funkcje/FunkcjeSemafory.h"
#include "Funkcje/FunkcjeObslugiTasmy.h"

#define MasaCegly 3 // P3 produkuje cegły o masie 3
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
        int wrzuconoCegle = 0;
        
        // Próbujemy zdobyć semafor (nieblokująco z IPC_NOWAIT)
        if (wait_semafor(semaforTasmy, 0, SEM_UNDO | IPC_NOWAIT) == 0)
        {
            // Mamy semafor - sprawdzamy czy można wrzucić cegłę
            if (sprawdzCzyMoznaWrzucicCegle(tasma, K, MasaCegly, M))
            {
                wrzucCegleNaTasme(tasma, K, MasaCegly);
                printf("\033[1;34m[%d] P3 ~ Wrzuciłem cegłe o masie %dkg na taśme.\033[0m\n", getpid(), MasaCegly);
                wrzuconoCegle = 1;
            }
            // Zwalniamy semafor
            signal_semafor(semaforTasmy, 0, SEM_UNDO);
        }
        
        // Czas produkcji kolejnej cegły (poza sekcją krytyczną!)
        if (wrzuconoCegle)
            sleep(T1);
        else
            usleep(10000); // 10ms przerwy przed ponowną próbą
    }
    printf("\033[1;34m[%d] P3 ~ Kończę pracę na dziś.\033[0m\n", getpid());
    return 0;
}
