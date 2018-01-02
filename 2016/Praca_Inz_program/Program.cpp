#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <string>
#include <armadillo>

#include "PropertyFileConfiguration.h"

using namespace std;

	//a.u.l. to nm
	double nm_to_aul = 18.89725988579;
	//eV to Hartree
	double eV_to_Hartree = 0.0367502;

	double L,L_w,L_d,L_b,L_b2,L_b3,L_b4,L_b5,w,dz;

	double kT = 0.0257*eV_to_Hartree;

	double E_f,d_6,d_b6,V_g1,V_g2;

	double f_m;
	double eps_0=0.07957747154594767; //a.u.
	double eps_r;
	double dop_dense;
	double omega;//Poisson mix parameter
	double alpha;//potential new/old mix parameter
	double poisson_lim;
	double poisson_lim_start;

	unsigned N;
	unsigned iterations;
	unsigned zPoints;

    arma::Col<int> n;

	arma::mat Schr_mat_diag_E;
	arma::mat Schr_mat;
    arma::mat C_mat;
	arma::mat waveFunctions;

	arma::vec E;
	arma::vec z;

	arma::vec V;
	arma::vec V_old;
	arma::vec V_H_el;
	arma::vec V_H_el_old;
    arma::vec V_H_g_dop;
	arma::vec V_H_g_dop_old;
	arma::vec V_offset;

	arma::vec dense_el;
    arma::vec dense_dop;
    arma::vec dense;


	void initializeStartVariableValues();
	void doCalculations();
	void solveShroedingerEquation();
	void solvePoisonEquation();
	void solvePoisonEquationStart();
	void calculateWaveFunctinAmplitude();
	void calculateElectronsDense();
	double integrate(arma::vec& V, int m, int n);
	void readPropertiesFile(string file_name);
	void translateFromAtomicUnits();
	void normalize(arma::mat& M);
	void saveResults();
	int main();

void readPropertiesFile(string file_name)
{
	PropertyFileConfiguration c = PropertyFileConfiguration(file_name);
	L = c.getDouble("nm_L");
	L_w = c.getDouble("nm_L_w");
	L_d = c.getDouble("nm_L_d");
	L_b = c.getDouble("nm_L_b");
	L_b2 = c.getDouble("nm_L_b2");
	L_b3 = c.getDouble("nm_L_b3");
	L_b4 = c.getDouble("nm_L_b4");
	L_b5 = c.getDouble("nm_L_b5");
	w = c.getDouble("nm_w");
	dz = c.getDouble("nm_dz");
	d_6 = c.getDouble("eV_d_6");
	d_b6 = c.getDouble("eV_d_b6");
	V_g1 = c.getDouble("eV_V_g1");
	V_g2 = c.getDouble("eV_V_g2");
	N = c.getInt("N");
	E_f = c.getDouble("eV_E_f");
	iterations = c.getDouble("iterations");
	f_m = c.getDouble("f_m");
	eps_r = c.getDouble("eps_r");
	dop_dense = c.getDouble("cm^-3_dop_dense");
	alpha = c.getDouble("alpha");
	omega = c.getDouble("omega");
	poisson_lim = c.getDouble("poisson_lim");
	poisson_lim_start = c.getDouble("p_lim_start");
	c.printProperties();
}

void initializeStartVariableValues()
{
    zPoints = unsigned((2*L/dz))+1;
    cout<<zPoints<<endl;
	L *= nm_to_aul;
	L_w *= nm_to_aul;
	L_d *= nm_to_aul;
	L_b *= nm_to_aul;
	L_b2 *= nm_to_aul;
	L_b3 *= nm_to_aul;
	L_b4 *= nm_to_aul;
	L_b5 *= nm_to_aul;
	w *= nm_to_aul;
	dz *= nm_to_aul;
	E_f *= eV_to_Hartree;
	d_6 *= eV_to_Hartree;
	d_b6 *= eV_to_Hartree;
	V_g1 *= eV_to_Hartree;
	V_g2 *= eV_to_Hartree;
	dop_dense *= pow(1e7 * nm_to_aul,-3.0);//from cm to nm, from nm to a.u.l.


	z = arma::vec(zPoints);
	for(unsigned i = 0; i<zPoints ;++i)
	{
		z[i] = i*dz-L;
		cout<<i<<'\t'<<z(i)/nm_to_aul<<endl;
	}

	n = arma::Col<int>(N);
	E = arma::vec(N);
	C_mat = arma::mat(N,N);

	Schr_mat_diag_E = arma::mat(N,N);
	Schr_mat = arma::mat(N,N);

    Schr_mat_diag_E.fill(0.0);
	for(unsigned i = 0; i<N ;++i)
	{
		n[i] = i+1;
		Schr_mat_diag_E(i,i) = pow(n[i]*M_PI/(2.0*L),2.0)/2.0/f_m;
	}

	waveFunctions = arma::mat(zPoints,N);

	V = arma::vec(zPoints);
	V_old = arma::vec(zPoints);
    V_H_el = arma::vec(zPoints);
    V_H_el_old = arma::vec(zPoints);
    V_H_g_dop = arma::vec(zPoints);
    V_H_g_dop_old = arma::vec(zPoints);
	V_offset = arma::vec(zPoints);

	dense_el = arma::vec(zPoints);
    dense_dop = arma::vec(zPoints);
    dense = arma::vec(zPoints);

	for(unsigned i = 0; i<zPoints ;++i)
	{
	    V_H_el[i] = 0.0;
		if(abs(double(i)-double(zPoints/2))<L_d/L*zPoints*0.5&&abs(double(i)-double(zPoints/2))>(L_d-w)/L*zPoints*0.5)
            dense_dop[i] = dop_dense;
        if(abs(double(i)-double(zPoints/2))<L_w/2.0/L*zPoints*0.5)
            V_offset[i] = 0.0;
        else
            V_offset[i] = d_6;

        if(abs(double(i)-double(zPoints/2))>L_b/2.0/L*zPoints*0.5&&abs(double(i)-double(zPoints/2))<L_b2/2.0/L*zPoints*0.5)
            V_offset[i] = d_b6;
        if(abs(double(i)-double(zPoints/2))>L_b3/2.0/L*zPoints*0.5&&abs(double(i)-double(zPoints/2))<L_b4/2.0/L*zPoints*0.5)
            V_offset[i] = d_b6;
	}
}

void solveShroedingerEquation()
{

	Schr_mat.fill(0.0);
	Schr_mat += Schr_mat_diag_E;
	for(unsigned i = 0; i<N ;++i)
	{
		for(unsigned j = 0; j<i ; ++j)
		{
			double integ_Pot_ij=integrate(V,n[i],n[j]);
			Schr_mat(i,j)=integ_Pot_ij;
			Schr_mat(j,i)=integ_Pot_ij;
		}
	}

	for(unsigned i = 0; i<N ;++i)
	{
		Schr_mat(i,i) += integrate(V,n[i],n[i]);
	}

	eig_sym(E, C_mat, Schr_mat);
}

void solvePoisonEquationStart()
{
    double C=-dz*dz/eps_0/eps_r;

    dense = dense_dop;//dense_el <= 0.0

    V_H_g_dop[0]=V_g1;
    V_H_g_dop[zPoints-1]=V_g2;

    double sum=100000000;

    while(sum>poisson_lim_start)
    {
        V_H_g_dop_old = V_H_g_dop;

        for(unsigned i=1; i<zPoints-1 ;++i)
        {
            V_H_g_dop[i]=(1.0-omega)*V_H_g_dop_old[i]+omega*0.5*(V_H_g_dop_old[i-1]+V_H_g_dop_old[i+1]+C*dense[i]);
        }

        sum=0.0;
        for(unsigned i=1; i<zPoints-1 ;++i)
            sum+=abs(V_H_g_dop[i]-V_H_g_dop_old[i]);
        sum/=zPoints;
        cout<<"g_dop"<<sum<<endl;
    }

    V_old = V;
    V = V_offset + V_H_g_dop;
}

void solvePoisonEquation()
{
    double C=-dz*dz/eps_0/eps_r;

    arma::vec V_H_el_old_omega = arma::vec(zPoints);
    V_H_el_old = V_H_el;

    double sum=100000000;
    int l=0;
    while(sum>poisson_lim)
    {
        V_H_el_old_omega = V_H_el;

        for(unsigned i=1; i<zPoints-1 ;++i)
        {
            V_H_el[i]=(1.0-omega)*V_H_el_old_omega[i]+omega*0.5*(V_H_el_old_omega[i-1]+V_H_el_old_omega[i+1]+C*dense_el[i]);
        }

        sum=0.0;
        for(unsigned i=1; i<zPoints-1 ;++i)
            sum+=abs(V_H_el[i]-V_H_el_old_omega[i]);
        sum/=zPoints;
        cout<<"el"<<sum<<endl;
        cout<<"el"<<poisson_lim<<endl;
        cout<<"el l="<<l<<endl;
        cout<<"el"<<(sum<=poisson_lim)<<endl;

        if(l<100000 && sum<=poisson_lim)
        {
            poisson_lim*=0.75;
        }

        ++l;

    }

    V_H_el = (1.0-alpha)*V_H_el_old + alpha*V_H_el;
    dense = dense_dop + dense_el;//dense_el <= 0.0
    V_old = V;

    V = V_offset + V_H_g_dop + V_H_el;

}


void calculateWaveFunctinAmplitude()
{
	waveFunctions.fill(0.0);
	double C = sqrt(2.0/(L*2.0));
	//i - Z Axis point
	//j - energy/eigen vector index
	//k - n harmonic index
	for(unsigned i = 0; i < zPoints ; ++i)
	{
		for(unsigned j = 0; j < N ; ++j)
		{
			for(unsigned k = 0; k < N ; ++k)
			{
				waveFunctions(i,j)+=C*C_mat(k,j)*sin(n[k]*i*dz*M_PI/(L*2.0));
			}
		}
	}
}

void calculateElectronsDense()
{
	dense_el.fill(0.0);
	for(unsigned i = 0; i < zPoints ; ++i)
	{
		for(unsigned j = 0; j<N ; ++j)
		{
			dense_el[i] +=
			pow(waveFunctions(i,j),2.0)*
			f_m*kT/M_PI*log(1.0+exp((E_f-E[j])/kT));
		}
	}
	dense_el*=dz;
	dense_el*=(-1);
}

void normalize(arma::mat& M)
{
	double sum =0.0;
	for(unsigned j = 0;j < M.n_cols;++j)
	{
		for(unsigned i = 0;i < M.n_rows;++i)
		{
			sum+=M(i,j)*M(i,j)*dz;
		}
		sum = sqrt(sum);
		//cout<<sum<<endl;
		if(M(round(L/dz+1.0),j)<0)
			sum=sum;
		else
			sum=-sum;
		for(unsigned i = 0;i < M.n_rows;++i)
		{
				M(i,j)/=sum;
		}
		sum=0.0;
	}
}

void doCalculations()
{
	while(poisson_lim>1e-10)
	{
		/*cout<<int(100*i/iterations)<<" %"<<endl;
		if(int(100*i/iterations)>70)
            poisson_lim=1e-10;*/
        solveShroedingerEquation();
        calculateWaveFunctinAmplitude();
        normalize(waveFunctions);
        calculateElectronsDense();
        solvePoisonEquation();
        saveResults();
	}
}

void translateFromAtomicUnits()
{
	E/=eV_to_Hartree;
	V/=eV_to_Hartree;
	V_H_el/=eV_to_Hartree;
	V_H_g_dop/=eV_to_Hartree;
	V_offset/=eV_to_Hartree;
	waveFunctions*=sqrt(nm_to_aul);
	z/=nm_to_aul;
	dz/=nm_to_aul;
	L/=nm_to_aul;
}

double integrate(arma::vec& V, int m, int n)
{
	double sum =0.0;
	const double constM = m*M_PI/(L*2.0),constN = n*M_PI/(L*2.0);
	for(unsigned i = 0;i < V.size();++i)
	{
		sum+=2.0/(L*2.0)*sin(constM*i*dz)*sin(constN*i*dz)*V[i]*dz;
	}
	return sum;
}

void saveResults()
{

	fstream plik;

	string naz_pli="wyniki/energie";
	naz_pli.append(to_string(V_g2/eV_to_Hartree));
	naz_pli.append(".txt");
    plik.open(naz_pli.c_str(),std::ofstream::out |std::ofstream::trunc);
	plik<<E;
	plik.close();

    naz_pli="wyniki/fun_fal";
	naz_pli.append(to_string(V_g2/eV_to_Hartree));
	naz_pli.append(".txt");
    plik.open(naz_pli.c_str(),std::ofstream::out |std::ofstream::trunc);
	for(unsigned i = 0; i<zPoints; ++i)
	{
		plik<<z[i]<<'\t';
		for(unsigned j = 0; j<5; ++j)
		{
			plik<<'\t'<<waveFunctions(i,j);
		}
		plik<<endl;
	}
	plik.close();

	naz_pli="wyniki/V";
	naz_pli.append(to_string(V_g2/eV_to_Hartree));
	naz_pli.append(".txt");
    plik.open(naz_pli.c_str(),std::ofstream::out |std::ofstream::trunc);
	for(unsigned i = 0; i<zPoints; ++i)
	{
		plik<<z[i]<<'\t'<<
            V[i]<<'\t'<<
            V_H_el[i]<<'\t'<<
            V_H_g_dop[i]<<'\t'<<
            V_offset[i]<<'\t';
		plik<<endl;
	}
	plik.close();

	naz_pli="wyniki/dens";
	naz_pli.append(to_string(V_g2/eV_to_Hartree));
	naz_pli.append(".txt");
    plik.open(naz_pli.c_str(),std::ofstream::out |std::ofstream::trunc);
	for(unsigned i = 0; i<zPoints; ++i)
	{
		plik<<z[i]<<'\t'<<
            dense[i]<<'\t'<<
            dense_el[i]<<'\t'<<
            dense_dop[i];
		plik<<endl;
	}
	plik.close();

    string gnuplot="gnuplot -e \"V='";
	gnuplot.append(to_string(V_g2/eV_to_Hartree));
	gnuplot.append("'\" drow.plt");
	cout<<system(gnuplot.c_str())<<endl;
}

int main()
{
    readPropertiesFile("properties.txt");
	initializeStartVariableValues();
	solvePoisonEquationStart();
	saveResults();
    doCalculations();
	translateFromAtomicUnits();
	normalize(waveFunctions);
	saveResults();



};
