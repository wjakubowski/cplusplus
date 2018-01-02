#include <vector>

using namespace std;

void operator+=(vector<double>& v, double d)
{
    for(unsigned i = 0;i<v.size();++i )
        v[i]+=d;
}

void operator-=(vector<double>& v, double d)
{
    for(unsigned i = 0;i<v.size();++i )
        v[i]-=d;
}

void operator*=(vector<double>& v, double d)
{
    for(unsigned i = 0;i<v.size();++i )
        v[i]*=d;
}

void operator/=(vector<double>& v, double d)
{
    for(unsigned i = 0;i<v.size();++i )
        v[i]/=d;
}

vector<double> operator+(vector<double>& v, double d)
{
    vector<double> result = vector<double>(v);
    result+=d;
    return result;
}

vector<double> operator-(vector<double>& v, double d)
{
    vector<double> result = vector<double>(v);
    result-=d;
    return result;
}

vector<double> operator*(vector<double>& v, double d)
{
    vector<double> result = vector<double>(v);
    result*=d;
    return result;
}

vector<double> operator/(vector<double>& v, double d)
{
    vector<double> result = vector<double>(v);
    result/=d;
    return result;
}

vector<double> operator+=(vector<double>& v1,const vector<double>& v2)
{
    for(unsigned i = 0;i<v1.size();++i )
        v1[i]+=v2[i];
    return v1;
}

vector<double> operator-=(vector<double>& v1,const vector<double>& v2)
{
    for(unsigned i = 0;i<v1.size();++i )
        v1[i]-=v2[i];
    return v1;
}

vector<double> operator+(const vector<double>& v1,const vector<double>& v2)
{
    vector<double> v=v1;
    v+=v2;
    return v;
}

vector<double> operator-(const vector<double>& v1,const vector<double>& v2)
{
    vector<double> v=v1;
    v-=v2;
    return v;
}

