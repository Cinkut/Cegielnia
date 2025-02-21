#include "FunkcjeObslugiTasmy.h"


int sprawdzCzyMoznaWrzucicCegle(int* tasmaPnt, int limitCegiel, int masaDorzucanejCegly, int masaKrytyczna)
{
    if (tasmaPnt[0] != 0)
        return 0;
    
    int obecnaMasaNaTasmie = 0;
    for (int i = 0; i < limitCegiel; i++)
        obecnaMasaNaTasmie += tasmaPnt[i];
    
    if (obecnaMasaNaTasmie + masaDorzucanejCegly > masaKrytyczna)
        return 0;
    return 1;
}


void wrzucCegleNaTasme(int *tasmaPnt, int limitCegiel, int masaDorzucanejCegly)
{
    int licznik = limitCegiel - 1;
    while (tasmaPnt[licznik] != 0) 
        licznik--;

    tasmaPnt[licznik] = masaDorzucanejCegly;
}


int czyJestCoSciagacZtasmy(int* tasmaPnt, int limitCegiel)
{
    if (tasmaPnt[limitCegiel - 1] != 0)
        return 1;
    return 0;
}


void przesunTasme(int* tasmaPnt, int limitCegiel)
{
    for (int i = limitCegiel - 1; i > 0; i--)
        tasmaPnt[i] = tasmaPnt[i - 1];
    tasmaPnt[0] = 0;
}


int sciagnijCegle(int *tasmaPnt, int limitCegiel)
{
    int sciagnietaCegla = tasmaPnt[limitCegiel - 1];
    tasmaPnt[limitCegiel - 1] = 0;
    przesunTasme(tasmaPnt, limitCegiel);
    return sciagnietaCegla;
}
