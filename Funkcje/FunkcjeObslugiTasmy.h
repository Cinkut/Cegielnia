#include <stdio.h>
#include <stdlib.h>

// Sprawdza czy można wrzucić cegłę na taśmę
int sprawdzCzyMoznaWrzucicCegle(int* tasmaPnt, int limitCegiel, int masaDorzucanejCegly, int masaKrytyczna);
// Wrzuca cegłę na taśmę
void wrzucCegleNaTasme(int *tasmaPnt, int limitCegiel, int masaDorzucanejCegly);
// Przesuwa taśmę (symuluje ruch)
void przesunTasme(int* tasmaPnt, int limitCegiel);
// Sprawdza czy na końcu taśmy jest cegła
int czyJestCoSciagacZtasmy(int * tasmaPnt, int limitCegiel);
// Ściąga cegłę z końca taśmy
int sciagnijCegle(int *tasmaPnt, int limitCegiel);
