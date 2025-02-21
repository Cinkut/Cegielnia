#include "Funkcje/ZmienneSrodowiskowe.h"
#include "Funkcje/FunkcjePamieciDzielonej.h"
#include "Funkcje/FunkcjeKolejkiKomunikatow.h"
#include "Funkcje/FunkcjeSemafory.h"

#define MasaCegly 3
int PracaTrwa = 1;

void przesunTasme(int* tasmaPnt)
{
    for (int i = K - 1; i > 0; i--)
        tasmaPnt[i] = tasmaPnt[i - 1];
    tasmaPnt[0] = 0;
}

int czyTasmaWolna(int *tasmaPnt)
{
    if (tasmaPnt[K - 1] == 0 || tasmaPnt[0] == 0)
        return 1;
    return 0;
}

int czyTasmaNiePrzeciazona(int *tasmaPnt, int dorzucanaWaga)
{
    int wagaTasmy;
    for (int i = 0; i < K; i++)
        wagaTasmy += tasmaPnt[i];

    if (wagaTasmy + dorzucanaWaga > M)
        return 1;
    return 0;
}


int main()
{
    int sharedMemoryID = create_shared_memory(".", 'B', K, IPC_CREAT | 0600);
    int* tasma = (int*)attach_shared_memory(sharedMemoryID, NULL, 0);

    while (PracaTrwa)
    {

    }

    return 0;
}



