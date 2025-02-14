all: Dyspozytor Ciezarowki Ciezarowka Pracownicy P1 P2 P3

Dyspozytor:
	gcc Dyspozytor.c Funkcje/FunkcjeKolejkiKomunikatow.c Funkcje/FunkcjePamieciDzielonej.c -o Dyspozytor

Ciezarowki:
	gcc Ciezarowki.c Funkcje/FunkcjeKolejkiKomunikatow.c Funkcje/FunkcjePamieciDzielonej.c -o Ciezarowki

Ciezarowka:
	gcc Ciezarowka.c Funkcje/FunkcjeKolejkiKomunikatow.c Funkcje/FunkcjePamieciDzielonej.c -o Ciezarowka

Pracownicy:
	gcc Pracownicy.c Funkcje/FunkcjeKolejkiKomunikatow.c Funkcje/FunkcjePamieciDzielonej.c -o Pracownicy

P1:
	gcc P1.c Funkcje/FunkcjeKolejkiKomunikatow.c Funkcje/FunkcjePamieciDzielonej.c -o P1

P2:
	gcc P2.c Funkcje/FunkcjeKolejkiKomunikatow.c Funkcje/FunkcjePamieciDzielonej.c -o P2

P3:
	gcc P3.c Funkcje/FunkcjeKolejkiKomunikatow.c Funkcje/FunkcjePamieciDzielonej.c -o P3

clean:
	rm -r Dyspozytor Ciezarowki Ciezarowka Pracownicy P1 P2 P3