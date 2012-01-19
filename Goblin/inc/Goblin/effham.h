#ifndef EFFHAM_H
#define EFFHAM_H

#include "Goblin/vjkg.h"
#include "Goblin/enlist.h"

#include <map>
#include <valarray>
#include <vector>

namespace Goblin
{

using namespace std;

class params{
public:
  double bad;
  valarray< double > p;
  vector< bool > f;
  size_t size(){return p.size();}
  double& operator[](size_t i){return (i<size())?p[i]:bad;}
  bool fixed(size_t i){return (i<size())?f[i]:true;}
  void fix(size_t i){if (i<size()) f[i] = true;}
  void free(size_t i){if (i<size()) f[i] = false;}
  void resize(size_t n){if (n<3000) {p.resize(n);f.resize(n);}}
  valarray< double >& data(){return p;}
};

class effham {
  int Jmax_;
  vector<double>* ccalc_,*cexp_,*cweig_;
  double (*med)(valarray<double>&,VJKG&);
  double (*mer)(valarray<double>&,VJKG&,VJKG&);
  vector<vibq> vibs;
  vector<double> p0;
public:
  file_name fname;
  enlist cen;
  map< vibq,params > dpar;
	effham();
	~effham();
	int Jmax(){return Jmax_;}
	double& ccalc(size_t i){return (*ccalc_)[i];}
	double& cexp(size_t i){return (*cexp_)[i];}
	double& cweig(size_t i){return (*cweig_)[i];}
	void newCalc(int j,enlist *expl,enlist *calcl);
	void make_dpar(vibq v);
	void calc(bool fit=false);
	double D(VJKG& q);
	double R(VJKG& q1,VJKG& q2);
	void loadp(string fn);
	void savep(string fn);
	void get_pars(vector< double* > &p);
};

} // Goblin

#endif
