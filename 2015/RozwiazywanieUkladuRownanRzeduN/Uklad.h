#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ostream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <utility>
#include <ctime>
#include <stdio.h>
#include <functional>
#include <stdlib.h>


using namespace std;

#ifndef klasa_h
#define klasa_h





class Rownanie
{
public:
//zmienne
	int l_zmiennych;//liczba zmiennych w równaniu
	vector<double> wspu;//wektor wspu³czynników przy zmiennych równania
	double wart;//wartoœæ liczbowa równania



//konstruktory
	Rownanie():l_zmiennych(0),wart(0){}
	Rownanie(int l_zmiennych):l_zmiennych(l_zmiennych),wart(0){wspu.resize(l_zmiennych);}
	Rownanie(const Rownanie& x):l_zmiennych(x.l_zmiennych),wspu(x.wspu),wart(x.wart){}
//operatory
	Rownanie operator+ (const Rownanie& x);	
	Rownanie operator- (const Rownanie& x);
	Rownanie operator* (const double x);
	Rownanie operator/ (const double x);
	Rownanie& operator= (const Rownanie& x); //operator przypisania
	friend bool operator== (const Rownanie& a,const Rownanie& b); //operator porównania
	friend ostream& operator<< (ostream& o,const Rownanie& r); //wypisywanie równania

//funkcje potrzebne do rozwi¹zywania uk³adu równañ
	int pierwszy_niezerowy_wspulczynnik_rownania();//urzywane tylko po sprawdzeniu ¿e równanie ma niezerowy wspulczynnik,
													//ta metoda sprawdza przy ktorej zmiennej jest pierwszy niezerwy wspolczynnik
													//jak wszystkie s¹ zero to wypluwa cout informacje 
													//lepiej urzywac z ponizsza funkcja -> rozumiec co dzieje sie w programie
													//przy wielu zmiennych ta fumkcja czesto pluje, ale jest to spowodowane
													//zerowaniem równania przez zbyr duze ró¿nice w wartosciach wspolczynnikow
	bool czy_ma_nie_zerowy_wspulczynnik();//sprawdza czy jest jakiœnie zerowy wspolczynnik

	

};


class Uklad
{
public:
	int l_rownan;//liczba rownan
	vector<Rownanie*> wsk_rown;//wektor wskaznikow do rownan ->w konstruktorach przy pomocy new twozone s¹ obiekty na
								//ktore wskazoja wskazniki ->wyatek to uzycie metody dodaj_rownaie
//konstruktory
	Uklad();
	Uklad(const Uklad& kopia);//twarda kopia-> nie kopia wskaznika ale obiektu
	void dodaj_rownanie(Rownanie& nowe);//dodaje rownanie, tzn wskaznik wsk_rown[ilestam] wskazuje na to rownanie
	void sortuj();//sortuje uzywa zewnetrznej funkcji sorter(nie umiem zroic tak by sorter byl metoda)
	
	friend ostream& operator<< (ostream& o,const Uklad& u);//wypoianie ukladu przy pomocy operatora << dla Rownania



//ROZWIOZANIE
	pair<bool,int> czy_ma_rownanie_i_z_jakim_numerem_o_pierwszym_niezerowym_wspulczynniku(int nr_wsp);
	//NAZWA WIELE MÓWI ZWRACA PAIR


	int zeruj_kolumne(int nr_kolumny);
//funkcja zeruje wszystkie oprócz wyrazu z równania roboczego wyrazy w kolumnie
//przez dodawanie równania z pierwszym niezerowym wspó³czynnikiem na pozycji w tej kolumnie nie wczesniej nie puzniej. 
//jak kolumna cala zerowa to nic ie robi kozysta min. z funkcjipowyzej.
//Chyba najwazniejsza funkcja w programie
	void rozwiazanie();
//zeruje kolomny od zerowej do l_rownan-1
	void skroc_wyniki();
//poprawia czytelnosc wyniku normalizuje wspolczynniki roawiazania do jedynki
	
	
	void skroc_liczby();
//bardzo potrzebna funkcja zwlaszcza jak duzy uklad
//funkcja dzoeli przez okreslono wartosc cale townanie w ktorym jakis wspolczynnik przekroczyl inn¹ okreslona wartosc
//wartosci wspolczynnikow szybko rosna trzeba je skracaæ

	void sprawdzenie(Uklad Rozwiazanie);//sprawdzam uklad rownan

};

//inne funkcje
bool sorter(Rownanie* a,Rownanie* b);
//urzywane przez metode Uklad::sortuj




#endif



//RZECZY NIE POTRZEBNE

	/*int pierwszy_niezerowy_wspulczynnik_w_kolumnie(int nr_kolumny);//funkcja zwraca numer pierwszego niezerowego wspulczynnika kolumny o zadanym numerze
																	//przed urzyciem funkcji nalerzy sprawdziæ czy istnieje niezerowy wspulczynnik w tej kolumnie(funkcja ponirzej)
	int ostatni_niezerowy_wspulczynnik_w_kolumnie(int nr_kolumny);//to co wyzej ale mniej potrzebne

	bool czy_ma_nie_zerowy_wspulczynnik(int nr_kolumny);*///mal

/*int Uklad::pierwszy_niezerowy_wspulczynnik_w_kolumnie(int nr_kolumny)
{
	sortuj();
	if(!czy_ma_nie_zerowy_wspulczynnik(nr_kolumny)) 
	{	
		cout<<"urzyto funkcji \"pierwszy_niezerowy_wspulczynnik_w_kolumnie\" bez sprawdzenia czy ma niezerowe wspolczynniki"<<endl;
		return 0;
	}
	int i=0;
	while(wsk_rown[i]->wspu[nr_kolumny]==0)++i;
	return i;


}

int Uklad::ostatni_niezerowy_wspulczynnik_w_kolumnie(int nr_kolumny)
{
	sortuj();
	if(!czy_ma_nie_zerowy_wspulczynnik(nr_kolumny)) 
	{	
		cout<<"urzyto funkcji \"ostatni_niezerowy_wspulczynnik_w_kolumnie\" bez sprawdzenia czy ma niezerowe wspolczynniki"<<endl;
		return 0;
	}
	int i=l_rownan-1;
	while(wsk_rown[i]->wspu[nr_kolumny]==0)--i;
	return i;


}*/
//funkcja zwraca numer pierwszego niezerowego wspulczynnika kolumny o zadanym numerze
//przed urzyciem funkcji nalerzy sprawdziæ czy istnieje niezerowy wspulczynnik w tej kolumnie(funkcja ponirzej)
/*bool Uklad::czy_ma_nie_zerowy_wspulczynnik(int nr_kolumny)
{
	int flaga=0;
	for(int i=0;i<l_rownan;++i)
		flaga+=abs(wsk_rown[i]->wspu[nr_kolumny]);
	return flaga;


}*/

	/*if(!czy_ma_nie_zerowy_wspulczynnik(nr_kolumny)) 
	{	
		cout<<"urzyto funkcji \"zeruj_kolumne\" bez sprawdzenia czy ma niezerowe wspolczynniki"<<endl;
		return 0;
	}
	int 
	pierwszy_niezerowy_w_kolumnie=pierwszy_niezerowy_wspulczynnik_w_kolumnie(nr_kolumny);
	
	
	for(int i=0;i<l_rownan;++i)
	{
		//cout<<wsk_rown[i]->wspu[nr_kolumny] <<'\t'<<(wsk_rown[pierwszy_niezerowy_w_kolumnie]->wspu[nr_kolumny])<<'\t'<<
		//	  wsk_rown[pierwszy_niezerowy_w_kolumnie]->wspu[nr_kolumny] <<'\t'<<(wsk_rown[i]->wspu[nr_kolumny])<<endl;
		if(wsk_rown[i]->wspu[nr_kolumny]&&i!=pierwszy_niezerowy_w_kolumnie)
		{*/	
			/*Rownanie r1(3),r2(3);
			r1=(*wsk_rown[i])*(wsk_rown[pierwszy_niezerowy_w_kolumnie]->wspu[nr_kolumny]);
			r2=(*wsk_rown[pierwszy_niezerowy_w_kolumnie])*(wsk_rown[i]->wspu[nr_kolumny]);
			cout<<"wypisuje r1 r2"<<endl<<r1<<endl<<r2<<endl;
			*wsk_rown[i]=r1-r2;*/
			/**wsk_rown[i]=(*wsk_rown[i])*(wsk_rown[pierwszy_niezerowy_w_kolumnie]->wspu[nr_kolumny])-
			(*wsk_rown[pierwszy_niezerowy_w_kolumnie])*(wsk_rown[i]->wspu[nr_kolumny]);
		}
	}

	return 1;*/

