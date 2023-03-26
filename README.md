# sort_all
Implemenacja algorymów:
 sortowanie_babelkowe
,  sortowanie_proste_wstawianie1
,  sortowanie_proste_wybieranie
,  sortowanie_shella
,  sortowanie_szybkie
,  sortowanie_przez_kopcowanie
,  standard_sort  - biblioteczna dla porównania

eksperyment funkcja testująca algorytmy - złożonosc czasowa

	parametry:
	 - funkcja_sortujaca - wskanik na funkcjącą sortowanie, musi przyjmować dwa parametry: adres początku tablicy (int*) oraz jej rozmiar (int)
	 - nazwa - nazwa testowanej funkcji, tylko w celach wypisania
	 - output - strumien do ktorego beda zapisane wyniki, domyslnie std::cerr, przy ostatnim uruchomieniu warto nadpisac otwartym strumieniem plikowym, aby sobie zebrac wyniki do pliku
	 - dodatkowe_miejsce - liczba dodatkowych elementow tablicy zaalokowanych PRZED poczatkiem tablicy, przykladowo gdy =1, pierwszym indeksem tablicy jest -1, ale dane rozpoczynaja sie od indeksu 0, moze sie przydac do sortowania przez wstawianie z wartownikiem

