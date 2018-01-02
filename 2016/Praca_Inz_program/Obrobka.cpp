#include <iostream>
#include <cmath>
#include <vector>
#include <string>

#include "PropertyFileConfiguration.h"
#include "FileUtils.h"
#include "vector_utility.h"

using namespace std;

//a.u.l. to nm
double nm_to_aul = 18.89725988579;
//eV to Hartree
double eV_to_Hartree = 0.0367502;

double ni_w,ni_b,ni_H;
double Gamma_w_00,Gamma_b_00,Gamma_H_el_00,Gamma_H_g_00;
double Gamma_w_10,Gamma_b_10,Gamma_H_el_10,Gamma_H_g_10;
double Gamma_w_11,Gamma_b_11,Gamma_H_el_11,Gamma_H_g_11;
double d_8,d_7,d_6,d_b8,d_b7,d_b6,del_g,del_b,del_w;
double f_m,P;
double E_p,E_g,E_w,E_b;
double L,L_w,L_d,L_b2,L_b3,L_b,w,dz;
double V_0,V_b,V_g;
unsigned zPoints;
double V_pocz,dV,V_konc;

vector<double> z,f0,f1;
vector<double> V,V_H_el,V_H_g_dop,V_offset,V_H;
vector<vector<double>> reader;

double a00,a10,a11;

int wlacz_program=0;
int czysc_plik=0;

double integrate(vector<double>& v1,vector<double>& v2,vector<double>& F);
void initializeStartVariableValues();
void readResults();
void readPropertiesFile(string file_pr,string file_obr);
void calculate_alfa();
void normalize(vector<double>& v);
int main();

int sgn(double i)
{
    if(i>=0.0)
        return 1;
    else
        return -1;
}

void readPropertiesFile(string file_pr,string file_obr)
{
	PropertyFileConfiguration pr = PropertyFileConfiguration(file_pr);
	L = pr.getDouble("nm_L");
	L_w = pr.getDouble("nm_L_w");
	L_d = pr.getDouble("nm_L_d");
	L_b = pr.getDouble("nm_L_b");
	L_b2 = pr.getDouble("nm_L_b2");
	L_b3 = pr.getDouble("nm_L_b3");
	w = pr.getDouble("nm_w");
	dz = pr.getDouble("nm_dz");
	d_6 = pr.getDouble("eV_V_0");
	d_b6 = pr.getDouble("eV_V_b");
	V_g = pr.getDouble("eV_V_g2");
	f_m = pr.getDouble("f_m");
	pr.printProperties();

	PropertyFileConfiguration obr = PropertyFileConfiguration(file_obr);
    E_w = obr.getDouble("eV_E_w");
    E_g = obr.getDouble("eV_E_g");
    E_b = obr.getDouble("eV_E_b");
    E_p = obr.getDouble("eV_E_p");
    del_b = obr.getDouble("eV_del_b");
    del_w = obr.getDouble("eV_del_w");
    del_g = obr.getDouble("eV_del_g");
    V_pocz = obr.getDouble("eV_V_pocz");
    dV = obr.getDouble("eV_dV");
    V_konc = obr.getDouble("eV_V_konc");
    wlacz_program = obr.getInt("wlacz_program");
    czysc_plik = obr.getInt("czysc_plik");
    obr.printProperties();

}

void initializeStartVariableValues()
{
    L *= nm_to_aul;
    L_w *= nm_to_aul;
    L_d *= nm_to_aul;
    L_b *= nm_to_aul;
    L_b2 *= nm_to_aul;
    L_b3 *= nm_to_aul;
    w *= nm_to_aul;
    dz *= nm_to_aul;
    V_0 *= eV_to_Hartree;
    V_g *= eV_to_Hartree;
    del_b *= eV_to_Hartree;
    del_w *= eV_to_Hartree;
    del_g *= eV_to_Hartree;
    d_6 *= eV_to_Hartree;
    d_b6 *= eV_to_Hartree;
    E_g *= eV_to_Hartree;
    E_w *= eV_to_Hartree;
    E_b *= eV_to_Hartree;
    E_p *= eV_to_Hartree;

    d_8 = E_w-E_g-d_6;
    d_b8 = E_b-E_g-d_b6;
    d_7 = d_8+del_w-del_g;
    d_b7 = d_b8+del_b-del_g;

    zPoints = unsigned(round(2*L/dz)+1);

    P = sqrt(E_p*0.5);
    ni_w = P*P/3.0*(d_8/pow(E_g,2)-d_7/pow(E_g+del_g,2));
    ni_b = P*P/3.0*(d_b8/pow(E_g,2)-d_b7/pow(E_g+del_g,2));
    ni_H = P*P/3.0*(1.0/pow(E_g,2)-1.0/pow(E_g+del_g,2));

    z=vector<double>(zPoints);
    for(unsigned i = 0; i<zPoints ;++i)
    {
        z[i] = i*dz-L;
    }
}

void readResults()
{
    string naz_pli="wyniki/V";
	naz_pli.append(to_string(V_g/eV_to_Hartree));
	naz_pli.append(".txt");
    vector<vector<double>> v = read_data_in_columns(naz_pli);
    V=v[1];
    cout<<V[zPoints-1]<<endl;
    V_H_el=v[2];
    V_H_g_dop=v[3];
    V_offset=v[4];
    V*=eV_to_Hartree;
    V_H_el*=eV_to_Hartree;
    V_H_g_dop*=eV_to_Hartree;
    V_offset*=eV_to_Hartree;
    V_H=V_H_el+V_H_g_dop;


    naz_pli="wyniki/fun_fal";
	naz_pli.append(to_string(V_g/eV_to_Hartree));
	naz_pli.append(".txt");
    vector<vector<double>> f = read_data_in_columns(naz_pli);
    f0=f[1];
    f1=f[2];
    normalize(f0);
    normalize(f1);
}

double integrate(vector<double>& v1,vector<double>& v2,vector<double>& F)
{
    double sum = 0.0;
    for(unsigned i = 1;i<zPoints-1;++i)
    {
        sum+=v1[i]*v2[i]*(F[i+1]-F[i-1])*0.5;
    }
    return sum;
}

void normalize(vector<double>& v)
{
    double sum =0.0;
    for(unsigned i = 0;i < v.size();++i)
    {
        sum+=v[i]*v[i]*dz;
    }
    v/=sqrt(sum);
}

void calculate_alfa()
{
    unsigned z_wl,z_wr;
    z_wl=round(zPoints*(L-L_w*0.5)/2.0/L);
    z_wr=round(zPoints*(L+L_w*0.5)/2.0/L)-1;
    unsigned z_bl,z_br,z_bl2,z_br2;
    z_bl=round(zPoints*(L-L_b*0.5)/2.0/L);
    z_br=round(zPoints*(L+L_b*0.5)/2.0/L)-1;
    z_bl2=round(zPoints*(L-L_b2*0.5)/2.0/L);
    z_br2=round(zPoints*(L+L_b2*0.5)/2.0/L)-1;

    Gamma_w_00 = ni_w*(-f0[z_wl]*f0[z_wl]+f0[z_wr]*f0[z_wr]);
    Gamma_b_00 = 0.0;
    Gamma_b_00 += ni_b*(f0[z_bl]*f0[z_bl]-f0[z_br]*f0[z_br]);
    Gamma_b_00 += ni_b*(f0[z_bl2]*f0[z_bl2]-f0[z_br2]*f0[z_br2]);
    Gamma_H_el_00 = -ni_H*integrate(f0,f0,V_H_el);
    Gamma_H_g_00 = -ni_H*integrate(f0,f0,V_H_g_dop);
    a00 = Gamma_w_00+Gamma_b_00+Gamma_H_el_00+Gamma_H_g_00;

    Gamma_w_10 = ni_w*(-f0[z_wl]*f1[z_wl]+f0[z_wr]*f1[z_wr]);
    Gamma_b_10 = 0.0;
    Gamma_b_10 += ni_b*(f1[z_bl]*f0[z_bl]-f1[z_br]*f0[z_br]);
    Gamma_b_10 += ni_b*(f1[z_bl2]*f0[z_bl2]-f1[z_br2]*f0[z_br2]);
    Gamma_H_el_10 = -ni_H*integrate(f0,f1,V_H_el);
    Gamma_H_g_10 = -ni_H*integrate(f0,f1,V_H_g_dop);
    a10 = Gamma_w_10+Gamma_b_10+Gamma_H_el_10+Gamma_H_g_10;

    Gamma_w_11 = ni_w*(-f1[z_wl]*f1[z_wl]+f1[z_wr]*f1[z_wr]);
    Gamma_b_11 = 0.0;
    Gamma_b_11 += ni_b*(f1[z_bl]*f1[z_bl]-f1[z_br]*f1[z_br]);
    Gamma_b_11 += ni_b*(f1[z_bl2]*f1[z_bl2]-f1[z_br2]*f1[z_br2]);
    Gamma_H_el_11 = -ni_H*integrate(f1,f1,V_H_el);
    Gamma_H_g_11 = -ni_H*integrate(f1,f1,V_H_g_dop);
    a11 = Gamma_w_11+Gamma_b_11+Gamma_H_el_11+Gamma_H_g_11;

    a00=a00/eV_to_Hartree/nm_to_aul*1000;
    a10=a10/eV_to_Hartree/nm_to_aul*1000*sgn(V_g);
    a11=a11/eV_to_Hartree/nm_to_aul*1000;

    Gamma_w_00/=eV_to_Hartree*nm_to_aul/1000;
    Gamma_b_00/=eV_to_Hartree*nm_to_aul/1000;
    Gamma_H_el_00/=eV_to_Hartree*nm_to_aul/1000;
    Gamma_H_g_00/=eV_to_Hartree*nm_to_aul/1000;
    Gamma_w_10/=eV_to_Hartree*nm_to_aul/1000*sgn(V_g);
    Gamma_b_10/=eV_to_Hartree*nm_to_aul/1000*sgn(V_g);
    Gamma_H_el_10/=eV_to_Hartree*nm_to_aul/1000*sgn(V_g);
    Gamma_H_g_10/=eV_to_Hartree*nm_to_aul/1000*sgn(V_g);
    Gamma_w_11/=eV_to_Hartree*nm_to_aul/1000;
    Gamma_b_11/=eV_to_Hartree*nm_to_aul/1000;
    Gamma_H_el_11/=eV_to_Hartree*nm_to_aul/1000;
    Gamma_H_g_11/=eV_to_Hartree*nm_to_aul/1000;

    vector<double> tmp =
        { V_g/eV_to_Hartree , a00, a10 , a11 ,
        Gamma_w_00 , Gamma_b_00 , Gamma_H_el_00 ,Gamma_H_g_00 ,
        Gamma_w_10 , Gamma_b_10 , Gamma_H_el_10 ,Gamma_H_g_10 ,
        Gamma_w_11 , Gamma_b_11 , Gamma_H_el_11 ,Gamma_H_g_11 };
    cout<<endl<<tmp.size()<<endl;
    append_file("wyniki/wyniki_alfa.txt",tmp);


}

int main()
{

    readPropertiesFile("properties.txt","properties_obr.txt");
    initializeStartVariableValues();
    //cout<<V_pocz<<V_konc<<endl;
    if(czysc_plik==1)
        clean_file("wyniki/wyniki_alfa.txt");

    for(double v=V_pocz;v<=V_konc;v+=dV)
    {
        PropertyFileConfiguration p("properties.txt");
        p.changeProperty("eV_V_g2",to_string(v));
        V_g = v*eV_to_Hartree;
        if(wlacz_program == 1)
        {
                system("./program");
        }
        readResults();
        calculate_alfa();
    }

    system("gnuplot -e \"V='0.500000'\" drow.plt");


}
