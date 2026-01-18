#include "FunkcjeObslugiTasmy.h"

// Sprawdza czy można wrzucić cegłę (czy jest miejsce i udźwig)
int sprawdzCzyMoznaWrzucicCegle(int* tasmaPnt, int limitCegiel, int masaDorzucanejCegly, int masaKrytyczna)
{
    // Sprawdzamy czy początek taśmy jest wolny
    if (tasmaPnt[0] != 0)
        return 0;
    
    // Liczymy obecną masę na taśmie
    int obecnaMasaNaTasmie = 0;
    for (int i = 0; i < limitCegiel; i++)
        obecnaMasaNaTasmie += tasmaPnt[i];
    
    // Sprawdzamy czy dodanie nowej cegły nie przekroczy udźwigu
    if (obecnaMasaNaTasmie + masaDorzucanejCegly > masaKrytyczna)
        return 0;
    return 1;
}

// Wrzuca cegłę na taśmę (szuka pierwszego wolnego miejsca od końca)
void wrzucCegleNaTasme(int *tasmaPnt, int limitCegiel, int masaDorzucanejCegly)
{
    int licznik = limitCegiel - 1;
    while (tasmaPnt[licznik] != 0) 
        licznik--;

    tasmaPnt[licznik] = masaDorzucanejCegly;
}

// Sprawdza czy na końcu taśmy jest cegła do ściągnięcia
int czyJestCoSciagacZtasmy(int* tasmaPnt, int limitCegiel)
{
    if (tasmaPnt[limitCegiel - 1] != 0)
        return 1;
    return 0;
}

// Przesuwa taśmę w prawo (symuluje ruch taśmy)
void przesunTasme(int* tasmaPnt, int limitCegiel)
{
    for (int i = limitCegiel - 1; i > 0; i--)
        tasmaPnt[i] = tasmaPnt[i - 1];
    tasmaPnt[0] = 0;
}

// Ściąga cegłę z końca taśmy i zwraca jej masę
int sciagnijCegle(int *tasmaPnt, int limitCegiel)
{
    int sciagnietaCegla = tasmaPnt[limitCegiel - 1];
    tasmaPnt[limitCegiel - 1] = 0;
    przesunTasme(tasmaPnt, limitCegiel); // Przesuwamy taśmę
    return sciagnietaCegla;
}
