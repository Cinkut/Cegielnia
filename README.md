

# Symulacja Cegielni – Projekt IPC
Projekt stanowi symulację działania cegielni, zrealizowaną w języku C z wykorzystaniem mechanizmów komunikacji międzyprocesowej (IPC) systemu Linux. System modeluje współpracę między dyspozytorem, pracownikami produkującymi cegły oraz ciężarówkami odbierającymi towar.

# Architektura Systemu
System składa się z kilku kluczowych modułów zarządzających procesami:

Dyspozytor: Centralny proces zarządzający, który synchronizuje pracę całego zakładu, zarządza wjazdami ciężarówek i dba o poprawne zakończenie pracy wszystkich jednostek.

Pracownicy (P1, P2, P3): Procesy symulujące produkcję cegieł o różnych masach (1kg, 2kg, 3kg) i umieszczanie ich na taśmie transportowej.

Ciężarówki: Procesy odpowiedzialne za podjeżdżanie pod taśmę, załadunek cegieł do osiągnięcia pełnej ładowności i ich transport.

Menedżery (Pracownicy i Ciężarówki): Procesy nadrzędne (wrapper), które tworzą grupy procesów potomnych i monitorują ich status za pomocą dedykowanych wątków zbierających "zombie".

# Wykorzystane Mechanizmy IPC
W projekcie zaimplementowano pełen wachlarz mechanizmów Systemu V IPC:

Kolejki Komunikatów: Służą do synchronizacji stanów gotowości procesów oraz zarządzania logistyką ciężarówek (pozwolenia na wjazd, potwierdzenia odjazdu).

Pamięć Dzielona: Reprezentuje taśmę transportową o określonej pojemności (liczba cegieł) i udźwigu.

Semafory: Zapewniają wzajemne wykluczanie przy dostępie do taśmy (pamięci dzielonej), chroniąc przed błędami zapisu przy jednoczesnej pracy wielu pracowników.

# Sygnały:

SIGUSR1: Wysyłany przez dyspozytora do ciężarówki w celu nakazania odjazdu z niepełnym ładunkiem.

SIGUSR2: Sygnał kończący pracę, rozsyłany do grup procesów w celu bezpiecznego zamknięcia symulacji.


# Konfiguracja i Parametry
Kluczowe parametry symulacji można modyfikować w pliku Funkcje/ZmienneSrodowiskowe.h:

N (4): Liczba dostępnych ciężarówek.

C (20): Maksymalna ładowność ciężarówki (liczba cegieł).

K (7): Maksymalna liczba cegieł na taśmie.

M (19): Maksymalny udźwig taśmy (suma mas cegieł).

T (5): Czas potrzebny na rozwożenie cegieł przez ciężarówkę.

T1 (1): Interwał czasowy produkcji cegieł przez pracowników.
