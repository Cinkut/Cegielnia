# Cegielnia

Symulacja cegielni realizowana w wielu procesach z użyciem IPC System V
(kolejki komunikatów, pamięć dzielona, semafory). Projekt pokazuje współpracę
pracowników wrzucających cegły na taśmę i ciężarówek, które je odbierają.

## Struktura procesów

- **Dyspozytor** – koordynuje pracę, wpuszcza ciężarówki i kończy pracę wszystkich procesów.
- **Pracownicy** – uruchamia procesy P1/P2/P3, które wrzucają cegły na taśmę.
- **Ciężarówki** – uruchamia N procesów `Ciezarowka`, które odbierają cegły z taśmy.

## Budowanie

```bash
make
```

Czyszczenie artefaktów:

```bash
make clean
```

## Uruchamianie

Uruchom procesy w osobnych terminalach (kolejność nie jest krytyczna):

```bash
./Pracownicy
./Ciezarowki
./Dyspozytor
```

Zakończenie pracy: wyślij sygnał `SIGUSR2` do procesu Dyspozytora, aby ten
zatrzymał resztę procesów.

```bash
kill -USR2 <pid_dyspozytora>
```

## Konfiguracja

Parametry symulacji znajdują się w `Funkcje/ZmienneSrodowiskowe.h`
(np. liczba ciężarówek, pojemność taśmy, masy cegieł).
