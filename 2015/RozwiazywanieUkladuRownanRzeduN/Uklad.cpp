#include "Uklad.h"



Rownanie Rownanie::operator+ (const Rownanie& x)	
{
	Rownanie nowe(*this);
	for(int i=0;i<l_zmiennych;++i)
		nowe.wspu[i]=wspu[i]+x.wspu[i];
	nowe.wart=wart+x.wart;
	return nowe;

}

Rownanie Rownanie::operator- (const Rownanie& x)
{
	Rownanie nowe(*this);
	for(int i=0;i<l_zmiennych;++i)
		nowe.wspu[i]=wspu[i]-x.wspu[i];
	nowe.wart=wart-x.wart;
	return nowe;
}

Rownanie Rownanie::operator* (const double x)
{
	Rownanie nowe(*this);
	for(int i=0;i<l_zmiennych;++i)
		nowe.wspu[i]=wspu[i]*x;
	nowe.wart=wart*x;
	return nowe;

}

Rownanie Rownanie::operator/ (const double x)
{
	Rownanie nowe(*this);
	for(int i=0;i<l_zmiennych;++i)
		nowe.wspu[i]=wspu[i]/x;
	nowe.wart=wart/x;
	return nowe;

}

Rownanie& Rownanie::operator= (const Rownanie& x)
{
	for(int i=0;i<l_zmiennych;++i)
		wspu[i]=x.wspu[i];
	wart=x.wart;
	return *this;

}

bool operator== (const Rownanie& a,const Rownanie& b)
{
	int flaga=0;
	for(int i=0;i<a.l_zmiennych;++i)
		if(a.wspu[i]==b.wspu[i])++flaga;
	if(a.wart==b.wart)++flaga;
	return !flaga;

}

ostream& operator<< (ostream& o,const Rownanie& r)
{
	for(int i=0;i<r.l_zmiennych;++i){o<<((r.wspu[i])?(r.wspu[i]):0)<<"  ";}//ZAPOBIEGAM WYPISYWANIU -0 +0 JAK TO CZASEM WYPISUJE DOUBLE
	o<<" | "<<r.wart;
	return o;

}

int Rownanie::pierwszy_niezerowy_wspulczynnik_rownania()//urzywane tylko po sprawdzeniu ¿e równanie ma niezerowy wspulczynnik
{
	if(!czy_ma_nie_zerowy_wspulczynnik()) 
	{	
		cout<<endl<<"urzyto funkcji \"pierwszy_niezerowy_wspulczynnik_rownania\" bez sprawdzenia czy ma niezerowe wspolczynniki"<<endl;
		return 0;
	}
	int i=0;
	while(wspu[i]==0)++i;
	return i;


}
bool Rownanie::czy_ma_nie_zerowy_wspulczynnik()
{
	double flaga=0;
	for(int i=0;i<l_zmiennych;++i)flaga+=abs(wspu[i]);
	return flaga>0;


}

//UKLAD

Uklad::Uklad():l_rownan(0){}  


Uklad::Uklad(const Uklad& kopia):l_rownan(kopia.l_rownan)
{
	for(int i=0;i<l_rownan;++i)
	{
		wsk_rown.push_back(new Rownanie(*(kopia.wsk_rown[i])));
	}
}
void Uklad::dodaj_rownanie(Rownanie& nowe)
	{
		wsk_rown.push_back(&nowe);++l_rownan;
	}

void Uklad::sortuj(){sort(wsk_rown.begin(),wsk_rown.end(),sorter);}

bool sorter(Rownanie* a,Rownanie* b)
{
	bool a_niezerowy=a->czy_ma_nie_zerowy_wspulczynnik(),b_niezerowy=b->czy_ma_nie_zerowy_wspulczynnik();
	if((b_niezerowy&&!a_niezerowy)||(!b_niezerowy&&!a_niezerowy&&(b->wart>a->wart)))
		return true;

	int a_nr=a->pierwszy_niezerowy_wspulczynnik_rownania(),b_nr=b->pierwszy_niezerowy_wspulczynnik_rownania();
	double a_wartosc=a->wspu[a_nr],b_wartosc=b->wspu[b_nr];
	if((a_nr<b_nr)||(a_nr==b_nr&&a_wartosc<b_wartosc))
		return true;
	else return false;
}

ostream& operator<< (ostream& o,const Uklad& u)
{
	for(int i=0;i<u.l_rownan;++i){o<<*u.wsk_rown[i]<<endl;}
	return o;

}




int Uklad::zeruj_kolumne(int nr_kolumny)
{
	

	bool czy_jest_odpowiednie_rownanie=czy_ma_rownanie_i_z_jakim_numerem_o_pierwszym_niezerowym_wspulczynniku(nr_kolumny).first;
	int nr_rownania_roboczego=czy_ma_rownanie_i_z_jakim_numerem_o_pierwszym_niezerowym_wspulczynniku(nr_kolumny).second;
	if(czy_jest_odpowiednie_rownanie==false)
		{return 0;cout<<"nie bylo odpowiedniego równania"<<endl;}
	for(int i=0;i<l_rownan;++i)
	{
		
		if(wsk_rown[i]->wspu[nr_kolumny]&&i!=nr_rownania_roboczego)
		{
			
			*wsk_rown[i]=(*wsk_rown[i])*(wsk_rown[nr_rownania_roboczego]->wspu[nr_kolumny])-
			(*wsk_rown[nr_rownania_roboczego])*(wsk_rown[i]->wspu[nr_kolumny]);
		}
		skroc_liczby();
	}

	return 1;
	
}

pair<bool,int> Uklad::czy_ma_rownanie_i_z_jakim_numerem_o_pierwszym_niezerowym_wspulczynniku(int nr_wsp)
{
	for(int i=0;i<l_rownan;++i)
	{
		if(!wsk_rown[i]->czy_ma_nie_zerowy_wspulczynnik())
			continue;
		if(wsk_rown[i]->pierwszy_niezerowy_wspulczynnik_rownania()==nr_wsp)
		{
			return make_pair(true,i);
		}
	
	}
	return make_pair(false,0);


};


void Uklad::rozwiazanie()
{
	for(int i=0;i<l_rownan;++i)
	{	
		zeruj_kolumne(i);
		skroc_liczby();
		sortuj();
	}


	skroc_wyniki();
	sortuj();

}

void Uklad::skroc_wyniki()
{
	for(int i=0;i<l_rownan;++i)
	{
		if(!wsk_rown[i]->czy_ma_nie_zerowy_wspulczynnik())
			continue;
		//cout<<wsk_rown[i]->pierwszy_niezerowy_wspulczynnik_rownania();
		*wsk_rown[i]=*wsk_rown[i]/wsk_rown[i]->wspu[wsk_rown[i]->pierwszy_niezerowy_wspulczynnik_rownania()];

	}
}

void Uklad::skroc_liczby()
{
	int flaga=0;
	for(int j=0;j<l_rownan;++j)
	{
		flaga=0;
		for(int i=0;i<wsk_rown[j]->l_zmiennych;++i)
		{
			if(wsk_rown[j]->wspu[i]>pow(2,3))
			{
				++flaga;
				
			}
		}
		if(flaga)
		{
			*wsk_rown[j]=*wsk_rown[j]/pow(2,2);
		
		}
	}

}


void Uklad::sprawdzenie(Uklad Rozwiazanie)
{
	cout<<"Sprawdzenie, odtwarzam wyrazy wolne pierwotnego uklasu rownan:"<<endl;
	for(int i=0;i<wsk_rown[0]->l_zmiennych;++i)
	{
		double suma=0;
		for(int j=0;j<wsk_rown[0]->l_zmiennych;++j)
		{
			suma+=wsk_rown[i]->wspu[j]*Rozwiazanie.wsk_rown[j]->wart;
		}
		cout<<suma<<endl;
	}
}


















