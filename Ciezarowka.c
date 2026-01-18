#include "Funkcje/ZmienneSrodowiskowe.h"
#include "Funkcje/FunkcjePamieciDzielonej.h"
#include "Funkcje/FunkcjeKolejkiKomunikatow.h"
#include "Funkcje/FunkcjeSemafory.h"
#include "Funkcje/FunkcjeObslugiTasmy.h"


static volatile sig_atomic_t ZaladunekTrwa = 0;
static volatile sig_atomic_t PracaTrwa = 1;
struct message CiezarowkaMozeWjechac = { .mtype = 5 };
struct message CiezarowkaWjechala = { .mtype = 6 };
struct message CiezarowkaOdjechala = { .mtype = 7 };


void sygnalDyspozytoraJeden_handler(int singal)
{
    ZaladunekTrwa = 0;
}

void sygnalDyspozytoraDwa_handler(int signal)
{
    PracaTrwa = 0;
    ZaladunekTrwa = 0;
}

// Proces ciężarówki: odbiera cegły z taśmy i odjeżdża po zapełnieniu kontenera.
int main()
{
    int kolejkaKomunikatow = create_message_queue(".", 'A', IPC_CREAT | 0600);
    CiezarowkaWjechala.pidProcesu = getpid();
    int sharedMemoryID = create_shared_memory(".", 'B', K * sizeof(int), IPC_CREAT | 0600);
    int* tasma = (int*)attach_shared_memory(sharedMemoryID, NULL, 0);
    int semaforTasmy = create_semafor(".", 'C', 1, IPC_CREAT | 0600);
    int kontenerCiezarowki[C];

    signal(SIGUSR1, sygnalDyspozytoraJeden_handler);

    while (PracaTrwa)
    {
        while (recive_message(kolejkaKomunikatow, &CiezarowkaMozeWjechac, 5, 0))
            if (!PracaTrwa)
                break;

        printf("\033[1;32m[%d] Ciężarówka ~ Wjeżdżam.\033[0m\n", getpid());
        ZaladunekTrwa = 1;
        int miejsceKolejnejCeglyWkontenerze = 0;
        send_message(kolejkaKomunikatow, &CiezarowkaWjechala, 0);

        while (ZaladunekTrwa)
        {
            // Sekcja krytyczna taśmy sterowana semaforem.
            while (wait_semafor(semaforTasmy, 0, 0))
            if (!PracaTrwa)
                break;
            
            while (czyJestCoSciagacZtasmy(tasma, K))
            {
		    if(miejsceKolejnejCeglyWkontenerze >= C){
			    ZaladunekTrwa=0;
			    break;
		    }

                kontenerCiezarowki[miejsceKolejnejCeglyWkontenerze] = sciagnijCegle(tasma, K);
                printf("\033[1;32m[%d] Ciezarowka ~ Załadowano cegłe o masie %d.\033[0m\n", getpid(), kontenerCiezarowki[miejsceKolejnejCeglyWkontenerze]);
                miejsceKolejnejCeglyWkontenerze++;
            }
            while(signal_semafor(semaforTasmy, 0, 0))
                if(!PracaTrwa)
                    break;
        }
        printf("\033[1;32m[%d] Ciężarówka ~ Odjeżdżam.\033[0m\n", getpid());
        send_message(kolejkaKomunikatow, &CiezarowkaOdjechala, 0);

        sleep(T);       // Rozwozenie Cegieł
        printf("\033[1;32m[%d] Ciężarówka ~ Rozwiozłam cegły.\033[0m\n", getpid());
        
    }
    return 0;
}
