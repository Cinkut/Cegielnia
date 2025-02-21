#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define N 4              // Liczba Ciężarówek
#define C 20            // Ładowność każdej z Ciężarówek wyrażaone w liczbie Cegieł
#define T 0           // Czas pod którym ciężarówce udaje się rozwieść cegły i wrócić do cegielni

#define K 7             // Maksymalna ilość cegieł jaką może przetransportować naraz taśma
#define M 19            // Maksymalna łączna masa cegieł jaką może przetransportować naraz taśma

#define T1 0           // Czas co ktory wrzucaja cegly

#define LiczbaPracownikow 3