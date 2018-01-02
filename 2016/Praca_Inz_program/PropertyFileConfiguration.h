#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <map>

/*
urzytkowanie:
-wystarczy dodac ten plik do folderu ze swoim kodem i w pliku .cpp zainkludowac:
#include "PropertyFileConfiguration.h"

-utworzyc plik z parametrami do symulacji, np o tresci:

nm_L = 20
nm_dx = 0.1

-w programie sczytuje wartosci z tego pliku w taki sposob:

PropertyFileConfiguration readProperties = PropertyFileConfiguration("nazwa_pliku.txt");
double L = readProperties.getDouble("nm_L");
double dx = readProperties.getDouble("nm_dx");

-sa jeszcze funkcjie getString() i getInt()
-plik txt nie zwraca uwagi na spacje i tabulatory ale kazda wartosc powinna byc w osobnej lini

-to oczywiście nie jest jakies cudo i wywali program jak popelni sie blad typu:
L=7.55555.4


*/

using namespace std;

class PropertyFileConfiguration
{
	private:
	void split(const string &s, char delim, vector<string> &elems);
	vector<string> split(const string &s, char delim);
	map<string,string> prop_map;
	string file_name;
	public:
	PropertyFileConfiguration(string file_name);
	string getString(string kay);
	int getInt(string kay);
	double getDouble(string kay);
	void printProperties();
	void changeProperty(string key,string val);
};


void PropertyFileConfiguration::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    elems.resize(0);
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

PropertyFileConfiguration::PropertyFileConfiguration(string file_name)
    :file_name(file_name)
{
	fstream plik;
	plik.open(file_name.c_str(),std::ifstream::in);
	string tmp = "";
	char split_char ='=';
	vector<string> str_vec = vector<string>(0);
	while(getline(plik,tmp))
	{
		for(unsigned i = 0; i<tmp.size() ; ++i)
			if(tmp[i] == ' ' || tmp[i] == '\t')
				tmp.erase(i,1);
		if(tmp[0] == '#' || tmp[0] == '/')
			continue;
		split(tmp,split_char,str_vec);
		prop_map[str_vec[0]] = str_vec[1];
	}
	plik.close();
}

string PropertyFileConfiguration::getString(string kay)
{
	return prop_map[kay];
}

int PropertyFileConfiguration::getInt(string kay)
{
	return atoi(prop_map[kay].c_str());
}

double PropertyFileConfiguration::getDouble(string kay)
{
	return atof(prop_map[kay].c_str());
}


void PropertyFileConfiguration::printProperties()
{
    cout<<"Program properties:"<<endl;
    for(map<string, string>::const_iterator it = prop_map.begin();it != prop_map.end(); ++it)
    {
        std::cout << it->first <<"="<< it->second<<endl;
    }
	cout<<endl;
}

void PropertyFileConfiguration::changeProperty(string key,string val)
{
    prop_map[key]=val;
    fstream file;
    file.open(file_name.c_str(),std::ofstream::out |std::ofstream::trunc);
    for(map<string, string>::const_iterator it = prop_map.begin();it != prop_map.end(); ++it)
    {
        file << it->first <<"="<< it->second<<endl;
    }
	file<<endl;
    file.close();
}

