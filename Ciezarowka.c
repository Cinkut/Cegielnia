#include "Funkcje/ZmienneSrodowiskowe.h"
#include "Funkcje/FunkcjePamieciDzielonej.h"
#include "Funkcje/FunkcjeKolejkiKomunikatow.h"
#include "Funkcje/FunkcjeSemafory.h"


int ZaladunekTrwa = 0;
int PracaTrwa = 1;
struct message CiezarowkaMozeWjechac = { .mtype = 5 };
struct message CiezarowkaWjechala = { .mtype = 6 };
struct message CiezarowkaOdjechala = { .mtype = 7 };


void sygnalDyspozytoraJeden_handler(int singal)
{
    printf("[%d] Ciężarówka ~ Odebrałem sygnał SIGUSR1.\n", getpid());
    ZaladunekTrwa = 0;
}

int main()
{
    int kolejkaKomunikatow = create_message_queue(".", 'A', IPC_CREAT | 0600);
    CiezarowkaWjechala.pidProcesu = getpid();

    signal(SIGUSR1, sygnalDyspozytoraJeden_handler);

    while (PracaTrwa)
    {
        recive_message(kolejkaKomunikatow, &CiezarowkaMozeWjechac, 5, 0);
        printf("[%d] Ciężarówka ~ Wjeżdżam.\n", getpid());
        ZaladunekTrwa = 1;
        send_message(kolejkaKomunikatow, &CiezarowkaWjechala, 0);

        while (ZaladunekTrwa)
        {
            pause();
        }
        printf("[%d] Ciężarówka ~ Odjeżdżam.\n", getpid());
        send_message(kolejkaKomunikatow, &CiezarowkaOdjechala, 0);
    }
    return 0;
}