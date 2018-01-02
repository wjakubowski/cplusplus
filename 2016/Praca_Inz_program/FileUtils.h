#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <map>

using namespace std;

void clean_file(string file_name);
void append_file(string file_name, vector<double> v);
vector<vector<double>> read_data_in_rows(string file_name);
vector<vector<double>> read_data_in_columns(string file_name);

void clean_file(string file_name)
{
    fstream file;
    file.open(file_name.c_str(),std::ofstream::out |std::ofstream::trunc);
    file.close();
}

void append_file(string file_name, vector<double> v)
{
    fstream file;
    file.open(file_name.c_str(),std::ofstream::out |std::ofstream::app);
    for(unsigned i = 0;i<v.size();++i)
        file<<v[i]<<'\t';
    file<<endl;
    file.close();
}

vector<vector<double>> read_data_in_rows(string file_name)
{
    vector<vector<double>> v;
    fstream file;
    string tmp="",tmp2="";
	file.open(file_name.c_str(),std::ifstream::in);

    while(getline(file,tmp))
	{
	    v.push_back(vector<double>());
        std::stringstream ss;
        ss.str(tmp);
        while(ss.good())
        {
            ss>>tmp2;
            v[v.size()-1].push_back(atof(tmp2.c_str()));
        }

	}
	return v;
}

vector<vector<double>> read_data_in_columns(string file_name)
{
    vector<vector<double>> v = read_data_in_rows(file_name);
    vector<vector<double>> w(v[0].size());

    for(unsigned i = 0;i<v.size();++i)
    {
        for(unsigned j = 0;j<v[i].size();++j)
        {
            w[j].push_back(v[i][j]);
        }
    }
	return w;
}



