//
//  AiSD_wszystkie_sortowania.cpp
//  projekt
//
//  Created by Marek Wala on 12/05/2022.
//
// AiSD_wszyskie_srotowania.cpp : This file contains the 'main' function. Program execution begins and ends there.


// Added 3 algorithms to compare, shell as it can be modified to have other complexity, and heap sort, and library sort from cpp library

#include "pch.h"
//fast headers
#include <iostream>
#include <chrono>
//timer manipulation
#include <random>
//pseudo numbers
#include <limits>
#include <algorithm>
//compare to library "sort"
#include <string>
#include <iomanip>
//manpip for streams
#include <fstream>
#include <clocale>

using std::swap;

//generater
int losowa_liczba(int min, int max)
{
    
	static std::default_random_engine gen(std::random_device{}());
	static std::uniform_int_distribution<int> dist;
	return dist(gen, std::uniform_int_distribution<int>::param_type{ min, max });
}

void wypelnij(int* tablica, int rozmiar, int min = 0, int max = std::numeric_limits<int>::max())
{
	for (int i = 0; i < rozmiar; ++i)
		tablica[i] = losowa_liczba(min, max);
}

bool jest_posortowane(int* tablica, int rozmiar)
{
	return std::is_sorted(tablica, tablica + rozmiar);
}

double mierz_czas(int* tablica, int rozmiar, void(*funkcja_sortujaca)(int*, int))
{
	auto start = std::chrono::high_resolution_clock::now();
	funkcja_sortujaca(tablica, rozmiar);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	return duration.count();
}

/*

	parametry:
	 - funkcja_sortujaca - wskanik na funkcjącą sortowanie, musi przyjmować dwa parametry: adres początku tablicy (int*) oraz jej rozmiar (int)
	 - nazwa - nazwa testowanej funkcji, tylko w celach wypisania
	 - output - strumien do ktorego beda zapisane wyniki, domyslnie std::cerr, przy ostatnim uruchomieniu warto nadpisac otwartym strumieniem plikowym, aby sobie zebrac wyniki do pliku
	 - dodatkowe_miejsce - liczba dodatkowych elementow tablicy zaalokowanych PRZED poczatkiem tablicy, przykladowo gdy =1, pierwszym indeksem tablicy jest -1, ale dane rozpoczynaja sie od indeksu 0, moze sie przydac do sortowania przez wstawianie z wartownikiem
*/
void eksperyment(void(*funkcja_sortujaca)(int*, int), const std::string& nazwa, std::ostream& output = std::cerr, int dodatkowe_miejsce = 0)
{
	//ustawienia
	const double limit_czasu = 30.0; //sekund
	const int powtorzen = 5;
	const int rozmiar_poczatkowy = 1 << 10;
	/////////////////////////////////////////
	const int szerokosc = 100;
	int gwiazdek = szerokosc - nazwa.length() - 2;
	if (gwiazdek < 0)
		gwiazdek = 0;
	int i = 0;
	output << " ";
	for (; i < gwiazdek / 2; ++i)
		output << '*';
	output << " " << nazwa << " ";
	for (; i < gwiazdek; ++i)
		output << '*';
	output << "\n\n";
	output.flush();

	output << std::setw(9) << "N";
	output << std::setw(1) << "";
	for (int nr = 0; nr < powtorzen; ++nr)
		output << std::setw(9) << nr + 1 << " ";
	output << std::setw(12) << "średna" << " ";
	output << "\n";
	for (int rozmiar = rozmiar_poczatkowy; ; rozmiar *= 2)
	{
		output << std::setw(9) << rozmiar << ": ";
		output.flush();
		int* pamiec = new int[dodatkowe_miejsce + rozmiar];
		int* tablica = pamiec + dodatkowe_miejsce;
		double czas = 0.0;

		int* pattern = new int[rozmiar];


		for (int nr = 0; nr < powtorzen; ++nr)
		{
			wypelnij(tablica, rozmiar);
			for (int i = 0; i < rozmiar; ++i)
				pattern[i] = tablica[i];
			std::sort(pattern, pattern + rozmiar);
			double c = mierz_czas(tablica, rozmiar, funkcja_sortujaca);
			if (!jest_posortowane(tablica, rozmiar))
			{
				output << "Tablica nieposortowana!!\n";
				if (&output != &std::cerr)
					std::cerr << "Tablica nieposortowana!!\n";
				return;
			}
			if (!std::equal(pattern, pattern + rozmiar, tablica, tablica + rozmiar))
			{
				output << "Tablica zawiera inne wartosci niz powinna!!\n";
				if (&output != &std::cerr)
					std::cerr << "Tablica zawiera inne wartosci niz powinna!!\n";
				return;
			}
			czas += c;
			output.precision(6);
			output << std::fixed << c << "  ";
			output.flush();
		}
		czas /= powtorzen;
		output << std::setw(12) << std::fixed << czas << "\n";
		output.flush();
		delete[] pamiec;
		delete[] pattern;
		if (czas > limit_czasu)
			break;
	}
	output << "\n";
	output.flush();
}




void sortowanie_babelkowe(int* tablica, int rozmiar)
{
	for (int i = 0; i < rozmiar; i++)
	{
		for (int j = 1; j < rozmiar; j++)
		{
			if (tablica[j - 1] > tablica[j])
			{
				swap(tablica[j - 1], tablica[j]);
			}
		}
	}
}

void sortowanie_proste_wstawianie1(int* tablica, int rozmiar)
{
	for (int i = 1; i < rozmiar; i++)
	{
		int x;
		int j;
		x = tablica[i];
		//for (j = i - 1; j >= 0 && tablica[j] > x; j--)
		j = i - 1;
		while(j>=0 && tablica[j]>x)
		{
			tablica[j + 1] = tablica[j];
			j -= 1;
		}
		tablica[j + 1] = x;
	}
}

void sortowanie_proste_wybieranie(int* tablica, int rozmiar)
{
	for (int i = 0; i < rozmiar; i++)
	{
		int k = i;
		for (int j = i + 1; j < rozmiar; j++)
		{
			if (tablica[j] < tablica[k])
			{
				k = j;
			}
		}
		swap(tablica[k], tablica[i]);
	}
}

void sortowanie_shella(int* tablica, int rozmiar)
{
	int x, j;
	int h = 1;
	while (h < rozmiar / 9)
	{
		//wyszukiwanie maks h
		h = (3 * h) + 1;
	}
	while (h > 0) //wykonuj h - sortowania tak długo, aż h zmaleje do 0
	{
		for (int i = h; i < rozmiar; i++)
		{
			x = tablica[i];
			j = i;

			while ((j >= h) && (x < tablica[j - h]))
			{
				tablica[j] = tablica[j - h];
				j = j - h;
			}
			tablica[j] = x;
		}
		h = h / 3; //zmniejszenie wartości h
	}
}

void sortowanie_szybkie(int* tab, int d, int g)
{
	int s, t = 0;
	if (d < g)
	{
		t = tab[d];
		s = d;

		for (int i = d + 1; i < g; i++)
		{
			if (tab[i] < t)
			{
				s = s + 1;
				swap(tab[s], tab[i]);
			}
		}
		swap(tab[d], tab[s]);
		sortowanie_szybkie(tab, d, s);
		sortowanie_szybkie(tab, s + 1, g);
	}
}
void szybka(int* tablica, int rozmiar)
{
	sortowanie_szybkie(tablica, 0, rozmiar);
}

int heap_size;
void kopiec_w_dol(int *tablica, int i)
{

	int largest;
	int  l = 2 * i;
	int r = 2 * i + 1;

	if (l < heap_size && tablica[l] > tablica[i])
	{
		largest = l;
	}
	else {
		largest = i;
	}

	if (r < heap_size && tablica[r] > tablica[largest])
	{
		largest = r;
	}

	if (largest != i)
	{
		swap(tablica[i], tablica[largest]);
		kopiec_w_dol(tablica, largest);
	}
}

void kopiec_buduj(int *tablica, int n)
{
	heap_size = n;
	for (int i = n / 2; i >= 0; i--)
	{
		kopiec_w_dol(tablica, i);
	}
}



void sortowanie_przez_kopcowanie(int *tablica, int n)
{
	kopiec_buduj(tablica, n);

	for (int i = n - 1; i >= 1; i--)
	{
		swap(tablica[0], tablica[i]);
		heap_size = heap_size - 1;
		kopiec_w_dol(tablica, 0);

	}
}

void standard_sort(int *tablica, int n)
{
	std::sort(tablica, tablica + n);
}


int main()
{
	setlocale(LC_ALL, "");
	std::ofstream wyniki("wyniki.txt");
    
	std::ostream& output = std::cerr; //zmienic na = wyniki aby zapisywalo do pliku
	eksperyment(sortowanie_babelkowe, "Sortowanie babelkowe", output);
	eksperyment(sortowanie_proste_wstawianie1, "Sortowanie przez proste wstawianie", output);
	eksperyment(sortowanie_proste_wybieranie, "Sortowanie przez proste wybieranie", output);
	eksperyment(sortowanie_shella, "Sortowanie Shella", output);
	eksperyment(szybka, "Sortowanie szybkie", output);
	eksperyment(sortowanie_przez_kopcowanie, "Sortowanie przez kopcowanie", output);
	eksperyment(standard_sort, "Sortowanie z biblioteki", output);
	return 0;
}
