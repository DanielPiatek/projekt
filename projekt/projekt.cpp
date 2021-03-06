
#include "pch.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <map>
#include <list>
#include <math.h>
#include <iomanip>
#include <algorithm>

using namespace std;

struct macierzStruktura
{
	string nazwa;
	int wiersze, kolumny;
	float** macierz;
};

void wczytajMacierz();
void zapiszMacierz(string nazwa, int wiersze, int kolumny, float** macierz);
void wyswietlWczytaneMacierze();
void wypiszMacierz(macierzStruktura macierz);
float** tworzenieDynamicznejTablicy2D(float** macierzTab2D, int wiersze, int kolumny);
macierzStruktura wybieranieMacierzyDoDzialan(bool pierwszy);
void dodawanieOdejmowanie(macierzStruktura macierz, bool dodawanie);
void mnozenie(macierzStruktura macierz);
macierzStruktura dopelnienieAlgebraiczne(macierzStruktura macierz, bool pierwotna);
float wyznacznikMacierzyDopelnien(int wiersze, int kolumny, macierzStruktura macierz);
float wyznaczWyznacznikNowy(macierzStruktura macierz);
float wyznacznikWyzszegoStopnia(macierzStruktura m);
float wyliczCalyWyznacznik(float wyznacznik);
macierzStruktura transponuj(macierzStruktura macierz, bool pierwotna);
void macierzOdwrotna(macierzStruktura macierz);
void wybierzMacierz(string operacja);

list<macierzStruktura> zaladowaneMacierze;
map<string, macierzStruktura> mapaMacierzy;
list <float>listaCyfrNaczelnych;

int main()
{
	int wybor;
	cout << "MENU GLOWNE KALKULATORA MACIERZY:" << endl;
	cout << "------------------------------------------" << endl;
	cout << "1. Wczytaj macierz z pliku" << endl;
	cout << "2. Wyswietl wczytane macierze" << endl;
	cout << "3. Dodaj macierz" << endl;
	cout << "4. Odejmij macierz" << endl;
	cout << "5. Pomnoz macierz" << endl;
	cout << "6. Wyznacznik macierzy" << endl;
	cout << "7. Dopelnienie algebraiczne" << endl;
	cout << "8. Macierz transponowana" << endl;
	cout << "9. Macierz odwrotna" << endl;
	cout << "10. Koniec programu " << endl;
	cout << "------------------------------------------" << endl;

	do
	{
		cout << "Wybor z menu: ";
		cin >> wybor;

		switch (wybor)
		{
		case 1:
			wczytajMacierz();
			break;

		case 2:
			wyswietlWczytaneMacierze();
			break;

		case 3:
			wybierzMacierz("dodawanie");
			break;

		case 4:
			wybierzMacierz("odejmowanie");
			break;

		case 5:
			wybierzMacierz("mnozenie");
			break;

		case 6:
			wybierzMacierz("wyznacznik");
			break;

		case 7:
			wybierzMacierz("dopelnienieAlgebraiczne");
			break;

		case 8:
			wybierzMacierz("transponuj");
			break;

		case 9:
			wybierzMacierz("odwrotna");
			break;

		case 10:
			return 0;
			break;

		default:
			return 0;
			break;

		}

	} while (true);

	system("pause");
}


void wczytajMacierz()
{
	cout << endl;
	string wczytajWiersze, wczytajKolumny, przerwa;
	fstream plik;
	string nazwaMacierzy;

	do
	{ 
		cout << "Podaj nazwe macierzy do wczytania z pliku (np. macierz_1): ";
		cin >> nazwaMacierzy;
		transform(nazwaMacierzy.begin(), nazwaMacierzy.end(), nazwaMacierzy.begin(), tolower);
		plik.open("macierze/" + nazwaMacierzy + ".txt", ios::in);

		if (plik.good() == false)
			cout << endl << "Plik nie istnieje" << endl << endl;

	} while (plik.good() == false);

	getline(plik, wczytajWiersze);
	getline(plik, wczytajKolumny);
	getline(plik, przerwa);

	int const wiersze = atoi(wczytajWiersze.c_str());
	int const kolumny = atoi(wczytajKolumny.c_str());

	float** macierzTab2D = nullptr;
	macierzTab2D = tworzenieDynamicznejTablicy2D(macierzTab2D, wiersze, kolumny);

	for (int i = 0; i < wiersze; i++)
		for (int j = 0; j < kolumny; j++)
			plik >> macierzTab2D[i][j];

	zapiszMacierz(nazwaMacierzy, wiersze, kolumny, macierzTab2D);
	wypiszMacierz(mapaMacierzy[nazwaMacierzy]);

	plik.close();
}


void zapiszMacierz(string nazwa, int wiersze, int kolumny, float** macierz)
{
	for (macierzStruktura n : zaladowaneMacierze)
	{
		if (nazwa == n.nazwa)
		{
			cout << endl << "Taka macierz jest juz zaladowana" << endl << endl;
			return;
		}
	}
	mapaMacierzy[nazwa].nazwa = nazwa;
	mapaMacierzy[nazwa].wiersze = wiersze;
	mapaMacierzy[nazwa].kolumny = kolumny;
	mapaMacierzy[nazwa].macierz = macierz;
	zaladowaneMacierze.push_back(mapaMacierzy[nazwa]);
}


void wyswietlWczytaneMacierze()
{
	cout << endl;
	for (macierzStruktura n : zaladowaneMacierze)
	{
		cout << n.nazwa << " | ";
	}
	cout << endl << endl;
}


void wypiszMacierz(macierzStruktura macierz)
{
	cout << endl;
	for (int i = 0; i < macierz.wiersze; i++)
	{
		for (int j = 0; j < macierz.kolumny; j++)
		{
			cout.width(9);
			cout << setprecision(4) << macierz.macierz[i][j];
		}
		cout << endl;
	}
	cout << endl;
}


float** tworzenieDynamicznejTablicy2D(float** macierzTab2D, int wiersze, int kolumny)
{
	macierzTab2D = new float*[wiersze];
	for (int i = 0; i < wiersze; ++i)
		macierzTab2D[i] = new float[kolumny];
	return macierzTab2D;
}


macierzStruktura wybieranieMacierzyDoDzialan(bool pierwszy) 
{
	string wyborMacierzy;
	macierzStruktura macierz;

	do
	{
		if (pierwszy) cout << endl << "Wpisz nazwe macierzy: ";
		if(!pierwszy) cout << endl << "Wpisz nazwe drugiej macierzy: ";
		cin >> wyborMacierzy;
		transform(wyborMacierzy.begin(), wyborMacierzy.end(), wyborMacierzy.begin(), tolower);

		for (macierzStruktura n : zaladowaneMacierze)
		{
			if (wyborMacierzy == n.nazwa)
			{
				macierz = n;
				continue;
			}
		}

	} while (macierz.nazwa != wyborMacierzy);

	return macierz;
}


void dodawanieOdejmowanie(macierzStruktura pierwszaMacierz, bool dodawanie)
{
	macierzStruktura drugaMacierz;
	macierzStruktura wynikMacierzy;

	do
	{
		drugaMacierz = wybieranieMacierzyDoDzialan(false);
		if ((drugaMacierz.wiersze != pierwszaMacierz.wiersze) || (drugaMacierz.kolumny != pierwszaMacierz.kolumny))
			cout << endl << "Druga macierz musi miec tyle samo kolumn i wierszy co pierwsza" << endl;

	} while ((drugaMacierz.wiersze != pierwszaMacierz.wiersze) || (drugaMacierz.kolumny != pierwszaMacierz.kolumny));
	
	float** wynikMacierzyTab = nullptr;
	wynikMacierzyTab = tworzenieDynamicznejTablicy2D(wynikMacierzyTab, pierwszaMacierz.wiersze, pierwszaMacierz.kolumny);

	for (int i = 0; i < pierwszaMacierz.wiersze; i++)
	{
		for (int j = 0; j < pierwszaMacierz.kolumny; j++)
		{
			if(dodawanie)
				wynikMacierzyTab[i][j] = pierwszaMacierz.macierz[i][j] + drugaMacierz.macierz[i][j];

			if(!dodawanie)
				wynikMacierzyTab[i][j] = pierwszaMacierz.macierz[i][j] - drugaMacierz.macierz[i][j];
		}
			
	}

	wynikMacierzy.kolumny = pierwszaMacierz.kolumny;
	wynikMacierzy.wiersze = pierwszaMacierz.wiersze;
	wynikMacierzy.macierz = wynikMacierzyTab;

	if (dodawanie)
		cout << endl << pierwszaMacierz.nazwa + " + " + drugaMacierz.nazwa + " = " << endl;

	if (!dodawanie)
		cout << endl << pierwszaMacierz.nazwa + " - " + drugaMacierz.nazwa + " = " << endl;

	wypiszMacierz(wynikMacierzy);
}


void mnozenie(macierzStruktura pierwszaMacierz)
{
	macierzStruktura drugaMacierz;
	macierzStruktura wynikMacierzy;
	float pojedynczyWynikKomorki{};

	do
	{
		drugaMacierz = wybieranieMacierzyDoDzialan(false);
		if (pierwszaMacierz.kolumny != drugaMacierz.wiersze) 
			cout << endl << "Ilosc kolumn z pierwszej macierzy musi byc rowna ilosci wierszy z drugiej macierzy" << endl; 

	} while (pierwszaMacierz.kolumny != drugaMacierz.wiersze); 

	float** wynikMacierzyTab = nullptr;
	wynikMacierzyTab = tworzenieDynamicznejTablicy2D(wynikMacierzyTab, pierwszaMacierz.wiersze, drugaMacierz.kolumny); 

	for (int i = 0; i < pierwszaMacierz.wiersze; i++)
	{
		for (int j = 0; j < drugaMacierz.kolumny; j++)
		{
			for (int k = 0; k < drugaMacierz.wiersze; k++) 
			{
				pojedynczyWynikKomorki += (pierwszaMacierz.macierz[i][k] * drugaMacierz.macierz[k][j]);
			}
			wynikMacierzyTab[i][j] = pojedynczyWynikKomorki;
			pojedynczyWynikKomorki = 0;
		}

	}

	wynikMacierzy.kolumny = drugaMacierz.kolumny;
	wynikMacierzy.wiersze = pierwszaMacierz.wiersze;
	wynikMacierzy.macierz = wynikMacierzyTab;

	cout << endl << pierwszaMacierz.nazwa + " * " + drugaMacierz.nazwa + " = " << endl;
	wypiszMacierz(wynikMacierzy);
}


macierzStruktura dopelnienieAlgebraiczne(macierzStruktura macierz, bool pierwotna)
{
	macierzStruktura macierzKopia;
	float potegowanieJedynki = 0;

	float** wynikMacierzyTab = nullptr;
	wynikMacierzyTab = tworzenieDynamicznejTablicy2D(wynikMacierzyTab, macierz.wiersze, macierz.kolumny);

	for (int i = 0; i < macierz.wiersze; i++)
		for (int j = 0; j < macierz.kolumny; j++)
			wynikMacierzyTab[i][j] = macierz.macierz[i][j];

	macierzKopia = macierz;
	macierzKopia.macierz = wynikMacierzyTab;

	for (int i = 0; i < macierz.wiersze; i++)
	{
		for (int j = 0; j < macierz.kolumny; j++)
		{
			macierzKopia.macierz[i][j] = wyznacznikMacierzyDopelnien(i, j, macierz);
			macierzKopia.macierz[i][j] = wyliczCalyWyznacznik(macierzKopia.macierz[i][j]);
		}
	}

	for (int i = 0; i < macierz.wiersze; i++)
	{
		for (int j = 0; j < macierz.kolumny; j++)
		{
			potegowanieJedynki = (float)(pow(-1, i + j + 2));
			macierzKopia.macierz[i][j] *= potegowanieJedynki;
			if (macierzKopia.macierz[i][j] == -0)
				macierzKopia.macierz[i][j] = 0;
		}
	}

	if (pierwotna)
	{
		cout << endl << "Dopelnienie algebraiczne: " << endl;
		wypiszMacierz(macierzKopia);
	}

	return macierzKopia;
}


float wyznacznikMacierzyDopelnien(int wiersze, int kolumny, macierzStruktura m)
{
	macierzStruktura mniejszaMacierz;
	mniejszaMacierz = m;
	mniejszaMacierz.wiersze -= 1;
	mniejszaMacierz.kolumny -= 1;

	float** mniejszaMacierzTab2D = nullptr; 
	mniejszaMacierzTab2D = tworzenieDynamicznejTablicy2D(mniejszaMacierzTab2D, mniejszaMacierz.wiersze, mniejszaMacierz.kolumny);

	int mniejszeWiersze = 0;
	int mniejszeKolumny = 0;
	for (int i = 0; i < mniejszaMacierz.wiersze; i++)
	{
		if (i == wiersze)
			mniejszeWiersze++;

		for (int j = 0; j < mniejszaMacierz.kolumny; j++)
		{
			if (j == kolumny)
				mniejszeKolumny++;

			mniejszaMacierzTab2D[i][j] = mniejszaMacierz.macierz[i + mniejszeWiersze][j + mniejszeKolumny];
		}
		mniejszeKolumny = 0;
	}

	mniejszaMacierz.macierz = mniejszaMacierzTab2D;

	return wyznaczWyznacznikNowy(mniejszaMacierz);
}


float wyznaczWyznacznikNowy(macierzStruktura macierz)
{
	float wyznacznik = 0;

	if ((macierz.kolumny == 1) && (macierz.wiersze == 1))
		wyznacznik = macierz.macierz[0][0];

	if ((macierz.kolumny == 2) && (macierz.wiersze == 2))
		wyznacznik = ((macierz.macierz[0][0] * macierz.macierz[1][1]) - (macierz.macierz[0][1] * macierz.macierz[1][0]));

	if ((macierz.kolumny == 3) && (macierz.wiersze == 3))
		wyznacznik = ((macierz.macierz[0][0] * macierz.macierz[1][1] * macierz.macierz[2][2]) + (macierz.macierz[1][0] * macierz.macierz[2][1] * macierz.macierz[0][2]) + (macierz.macierz[2][0] * macierz.macierz[0][1] * macierz.macierz[1][2]) - (macierz.macierz[0][2] * macierz.macierz[1][1] * macierz.macierz[2][0]) - (macierz.macierz[1][2] * macierz.macierz[2][1] * macierz.macierz[0][0]) - (macierz.macierz[2][2] * macierz.macierz[0][1] * macierz.macierz[1][0]));

	if ((macierz.kolumny > 3) && (macierz.wiersze > 3))
		wyznacznik = wyznacznikWyzszegoStopnia(macierz);

	return wyznacznik;
}


float wyznacznikWyzszegoStopnia(macierzStruktura m)
{
	int wierszeToAdd = 0;
	int kolumnyToAdd = 0;
	float cyfraNaczelna = 0;
	float doZerowania = 0;
	float potegowanieJedynki = 0;
	macierzStruktura kopiaMacierzy = m;

	float** wynikMacierzyTab = nullptr;
	wynikMacierzyTab = tworzenieDynamicznejTablicy2D(wynikMacierzyTab, kopiaMacierzy.wiersze, kopiaMacierzy.kolumny);

	for (int i = 0; i < kopiaMacierzy.wiersze; i++)
		for (int j = 0; j < kopiaMacierzy.kolumny; j++)
			wynikMacierzyTab[i][j] = m.macierz[i][j];

	kopiaMacierzy.macierz = wynikMacierzyTab;

	for (int i = 0; i < kopiaMacierzy.wiersze; i++)
	{
		if (cyfraNaczelna != 0) break;
		for (int j = 0; j < kopiaMacierzy.kolumny; j++)
		{
			if (kopiaMacierzy.macierz[i][j] != 0)
			{
				cyfraNaczelna = kopiaMacierzy.macierz[i][j];
				wierszeToAdd = i;
				kolumnyToAdd = j;
				break;
			}
		}
	}

	for (int i = 0; i < kopiaMacierzy.wiersze; i++)
	{
		doZerowania = -1 * (kopiaMacierzy.macierz[i][kolumnyToAdd] / cyfraNaczelna);
		if (i == wierszeToAdd) doZerowania = 0;
		for (int j = 0; j < kopiaMacierzy.kolumny; j++)
		{
			kopiaMacierzy.macierz[i][j] = doZerowania * kopiaMacierzy.macierz[wierszeToAdd][j] + kopiaMacierzy.macierz[i][j];
		}
	}

	potegowanieJedynki = (float)pow(-1, wierszeToAdd + kolumnyToAdd + 2);
	cyfraNaczelna *= potegowanieJedynki;
	listaCyfrNaczelnych.push_back(cyfraNaczelna);

	float** mniejszaMacierzTab2D = nullptr;
	mniejszaMacierzTab2D = tworzenieDynamicznejTablicy2D(mniejszaMacierzTab2D, kopiaMacierzy.wiersze - 1, kopiaMacierzy.kolumny - 1);

	int mniejszeWiersze = 0;
	int mniejszeKolumny = 0;
	for (int i = 0; i < kopiaMacierzy.wiersze - 1; i++)
	{
		if (i == wierszeToAdd)
			mniejszeWiersze++;

		for (int j = 0; j < kopiaMacierzy.kolumny - 1; j++)
		{
			if (j == kolumnyToAdd)
				mniejszeKolumny++;

			mniejszaMacierzTab2D[i][j] = kopiaMacierzy.macierz[i + mniejszeWiersze][j + mniejszeKolumny];
		}
		mniejszeKolumny = 0;
	}

	kopiaMacierzy.wiersze -= 1;
	kopiaMacierzy.kolumny -= 1;
	kopiaMacierzy.macierz = mniejszaMacierzTab2D;

	if ((kopiaMacierzy.wiersze > 3) && (kopiaMacierzy.kolumny > 3))
		wyznacznikWyzszegoStopnia(kopiaMacierzy);
	
	else if ((kopiaMacierzy.wiersze == 3) && (kopiaMacierzy.kolumny == 3))
	{
		cyfraNaczelna = ((kopiaMacierzy.macierz[0][0] * kopiaMacierzy.macierz[1][1] * kopiaMacierzy.macierz[2][2]) + (kopiaMacierzy.macierz[1][0] * kopiaMacierzy.macierz[2][1] * kopiaMacierzy.macierz[0][2]) + (kopiaMacierzy.macierz[2][0] * kopiaMacierzy.macierz[0][1] * kopiaMacierzy.macierz[1][2]) - (kopiaMacierzy.macierz[0][2] * kopiaMacierzy.macierz[1][1] * kopiaMacierzy.macierz[2][0]) - (kopiaMacierzy.macierz[1][2] * kopiaMacierzy.macierz[2][1] * kopiaMacierzy.macierz[0][0]) - (kopiaMacierzy.macierz[2][2] * kopiaMacierzy.macierz[0][1] * kopiaMacierzy.macierz[1][0]));
		listaCyfrNaczelnych.push_back(cyfraNaczelna);
	}

	return cyfraNaczelna;
}


macierzStruktura transponuj(macierzStruktura macierz, bool pierwotna)
{
	macierzStruktura macierzKopia;
	float potegowanieJedynki = 0;

	float** wynikMacierzyTab = nullptr;
	wynikMacierzyTab = tworzenieDynamicznejTablicy2D(wynikMacierzyTab, macierz.kolumny, macierz.wiersze);

	for (int i = 0; i < macierz.wiersze; i++)
		for (int j = 0; j < macierz.kolumny; j++)
			wynikMacierzyTab[j][i] = macierz.macierz[i][j];

	macierzKopia.kolumny = macierz.wiersze;
	macierzKopia.wiersze = macierz.kolumny;
	macierzKopia.macierz = wynikMacierzyTab;

	if (pierwotna)
	{
		cout << endl<<  "Macierz Transponowana: " << endl;
		wypiszMacierz(macierzKopia);
	}

	return macierzKopia;
}


void macierzOdwrotna(macierzStruktura macierz)
{
	float wyznacznik;

	wyznacznik = wyznaczWyznacznikNowy(macierz);
	wyznacznik = wyliczCalyWyznacznik(wyznacznik);
	macierz = dopelnienieAlgebraiczne(macierz, false);
	macierz = transponuj(macierz, false);

	for (int i = 0; i < macierz.wiersze; i++)
		for (int j = 0; j < macierz.kolumny; j++)
			macierz.macierz[j][i] /= wyznacznik;

	cout << endl << "Macierz odwrotna: " << endl;
	wypiszMacierz(macierz);
}


float wyliczCalyWyznacznik(float wyznacznik)
{
	if (!listaCyfrNaczelnych.empty())
	{
		wyznacznik = listaCyfrNaczelnych.back();
		listaCyfrNaczelnych.pop_back();

		for (auto x : listaCyfrNaczelnych)
			wyznacznik *= x;

		listaCyfrNaczelnych.clear();
	}
	return wyznacznik;
}


void wybierzMacierz(string operacja)
{
	macierzStruktura macierz;

	if (zaladowaneMacierze.empty())
	{
		cout << endl << "Brak zaladowanych macierzy" << endl << endl;
		return;
	}

	macierz = wybieranieMacierzyDoDzialan(true);

	if ((macierz.kolumny != macierz.wiersze) && ((operacja == "odwrotna") || (operacja == "dopelnienieAlgebraiczne") || (operacja == "wyznacznik")))
	{
		cout << endl << "Musisz podac macierz kwadratowa (taka, ktora ma tyle samo wierszy, co kolumn)." << endl << endl;
		return;
	}

	if(operacja == "odwrotna")
		macierzOdwrotna(macierz);

	if (operacja == "transponuj")
		transponuj(macierz, true);

	if (operacja == "dopelnienieAlgebraiczne")
		dopelnienieAlgebraiczne(macierz, true);

	if (operacja == "wyznacznik")
	{
		float wyznacznik = wyznaczWyznacznikNowy(macierz);
		wyznacznik = wyliczCalyWyznacznik(wyznacznik);
		cout << endl << "Wyznacznik macierzy " << macierz.nazwa << " jest rowny: "; cout.precision(10);	cout << wyznacznik << endl << endl;
	}

	if (operacja == "mnozenie")
		mnozenie(macierz);

	if (operacja == "odejmowanie")
		dodawanieOdejmowanie(macierz, false);

	if (operacja == "dodawanie")
		dodawanieOdejmowanie(macierz, true);

}