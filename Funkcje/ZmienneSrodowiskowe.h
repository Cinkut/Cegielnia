#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define N 4              // Liczba Ciężarówek
#define C 20            // Ładowność każdej z Ciężarówek wyrażaone w liczbie Cegieł
#define T 10           // Czas pod którym ciężarówce udaje się rozwieść cegły i wrócić do cegielni

#define K 3             // Maksymalna ilość cegieł jaką może przetransportować naraz taśma
#define M 8             // Maksymalna łączna masa cegieł jaką może przetransportować naraz taśma

#define LiczbaPracownikow 3