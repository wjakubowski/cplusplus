#include "Uklad.h"


int main()
{
	//generuje uklad rownan NxN
	srand(time(NULL));
	int N=20;
	Rownanie tab[N];
	Uklad u;
	for(int i=0;i<N;++i)
	{
		tab[i].wspu.resize(N);
		tab[i].l_zmiennych=N;
	}
	for(int i=0;i<N;++i)
	{
		for(int j=0;j<N;++j)
		{
			tab[i].wspu[j]=rand()%10;
		}
		tab[i].wart=rand()%10;
		u.dodaj_rownanie(tab[i]);
	}
	
	//wspolczynniki rownan inicjalizuje liczbami losowymi 
	//zalanczam Rownania do Ukladu

	u.sortuj();
	cout<<"po sortowaniu"<<endl<<u<<endl;//sortuje i wyswietlam posortowane
	Uklad kopia_do_sprawdzenia(u);/////////////////////////////////////////////////////////////////////////////////////
	u.rozwiazanie();//Rozwiozuje uklad rownan
	cout<<"ROZWIAZANIE"<<endl<<u<<endl;
	cout<<"kopia pierwotnego ukladu"<<endl;
	cout<<kopia_do_sprawdzenia;
	kopia_do_sprawdzenia.sprawdzenie(u);//sprawdzam wyniki


};

