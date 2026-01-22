#include "Funkcje/ZmienneSrodowiskowe.h"
#include "Funkcje/FunkcjePamieciDzielonej.h"
#include "Funkcje/FunkcjeKolejkiKomunikatow.h"
#include "Funkcje/FunkcjeSemafory.h"
#include "Funkcje/FunkcjeObslugiTasmy.h"

// Flaga czy trwa załadunek
static volatile sig_atomic_t ZaladunekTrwa = 0;
// Flaga czy ciężarówka pracuje
static volatile sig_atomic_t PracaTrwa = 1;
struct message CiezarowkaMozeWjechac = { .mtype = 5 };
struct message CiezarowkaWjechala = { .mtype = 6 };
struct message CiezarowkaOdjechala = { .mtype = 7 };

// Obsługa SIGUSR1 - odjedź z niepełnym ładunkiem
void sygnalDyspozytoraJeden_handler(int singal)
{
    ZaladunekTrwa = 0;
}

// Obsługa SIGUSR2 - zakończ pracę
void sygnalDyspozytoraDwa_handler(int signal)
{
    PracaTrwa = 0;
    ZaladunekTrwa = 0;
}

int main()
{
    // Łączenie się z kolejką komunikatów
    int kolejkaKomunikatow = create_message_queue(".", 'A', IPC_CREAT | 0600);
    CiezarowkaWjechala.pidProcesu = getpid();
    // Podłączenie do pamięci dzielonej (taśma)
    int sharedMemoryID = create_shared_memory(".", 'B', K * sizeof(int), IPC_CREAT | 0600);
    int* tasma = (int*)attach_shared_memory(sharedMemoryID, NULL, 0);
    // Podłączenie do semafora taśmy
    int semaforTasmy = create_semafor(".", 'C', 1, IPC_CREAT | 0600);
    int kontenerCiezarowki[C]; // Tablica przechowująca załadowane cegły

    signal(SIGUSR1, sygnalDyspozytoraJeden_handler);

    while (PracaTrwa)
    {
        // Czekamy na zezwolenie wjazdu od dyspozytora
        while (recive_message(kolejkaKomunikatow, &CiezarowkaMozeWjechac, 5, 0))
            if (!PracaTrwa)
                break;

        printf("\033[1;32m[%d] Ciężarówka ~ Wjeżdżam.\033[0m\n", getpid());
        ZaladunekTrwa = 1;
        int miejsceKolejnejCeglyWkontenerze = 0;
        send_message(kolejkaKomunikatow, &CiezarowkaWjechala, 0); // Potwierdzamy wjazd

        // Proces załadunku cegieł z taśmy
        while (ZaladunekTrwa && PracaTrwa)
        {
            // Próbujemy zdobyć semafor (nieblokująco z IPC_NOWAIT)
            if (wait_semafor(semaforTasmy, 0, SEM_UNDO | IPC_NOWAIT) == 0)
            {
                // Mamy semafor - ściągamy cegły z taśmy
                while (czyJestCoSciagacZtasmy(tasma, K))
                {
                    // Sprawdzamy czy ciężarówka pełna
                    if (miejsceKolejnejCeglyWkontenerze >= C) {
                        ZaladunekTrwa = 0;
                        break;
                    }

                    kontenerCiezarowki[miejsceKolejnejCeglyWkontenerze] = sciagnijCegle(tasma, K);
                    printf("\033[1;32m[%d] Ciezarowka ~ Załadowano cegłe o masie %d.\033[0m\n", getpid(), kontenerCiezarowki[miejsceKolejnejCeglyWkontenerze]);
                    miejsceKolejnejCeglyWkontenerze++;
                }
                // Zwalniamy semafor taśmy
                signal_semafor(semaforTasmy, 0, SEM_UNDO);
            }
            
            // Krótka przerwa przed ponowną próbą (unikamy busy-waiting)
            if (ZaladunekTrwa && PracaTrwa)
                usleep(10000); // 10ms
        }
        printf("\033[1;32m[%d] Ciężarówka ~ Odjeżdżam.\033[0m\n", getpid());
        send_message(kolejkaKomunikatow, &CiezarowkaOdjechala, 0);

        sleep(T); // Rozwozenie cegieł
        printf("\033[1;32m[%d] Ciężarówka ~ Rozwiozłam cegły.\033[0m\n", getpid());
        
    }
    return 0;
}
