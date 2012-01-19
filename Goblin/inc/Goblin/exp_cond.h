#ifndef EXP_COND_H
#define EXP_COND_H

#include <vector>

namespace Goblin
{

using namespace std;

class mixture{
  vector<double> Z_;
  double temper_;
public:
  vector<int> Iso;
  double press;
  //-----------
  mixture(){}
  mixture(double prs):press(prs){}
  size_t niso(){return Iso.size();}
  void add(int IIso);  //  add gas to mixture
  double P(){return press;}
  void P(double prs){press = prs;}
  double T(){return temper_;}
  void T(double tt);
  double Z(size_t i){return (i<niso())?Z_[i]:1.;}
  void Z(size_t i,double tt){if (i<niso()) Z_[i]=tt>0?tt:1.;}
  void copy(mixture& mix);
};

class Exp_cond {
  double temper_;
  double length_;
  vector<mixture> mix;
  double p_tot;
public:
	Exp_cond();
	~Exp_cond();
	size_t nmix(){return mix.size();}
	size_t niso(size_t i){return i<nmix()?mix[i].Iso.size():0;}
	int Iso(size_t i,size_t j){return i<nmix()?mix[i].Iso[j]:0;}
	double T(){return temper_;}
	void T(double tmp);
	double L(){return length_;}
	void L(double lng){length_ = lng;}
	double P(int Iso);
	double P(){return p_tot;}
	void addMix(double prs){mix.push_back(mixture(prs));p_tot+=prs;}
	void addIso(size_t im,int Iso);
	void addIso(int Iso){if (nmix()) addIso(nmix()-1,Iso);} // add Iso to the last added mixture
	void Z(double zz); // set z-sum for last iso in last mixture
	void Z(int Iso,double zz);
	double Z(int Iso);
	double Z(size_t i,size_t j){return i<nmix()?mix[i].Z(j):1.;}
	void copy(Exp_cond& e);
};

} // Goblin

#endif
