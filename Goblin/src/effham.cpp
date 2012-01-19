#include <fstream>
#include <sstream>
#include <iomanip>
#include "effham.h"
#include "matrix.h"
#include "../Troll1/mio.h"
#include "../Troll1/utils.h"
using namespace std;

double med1(valarray<double>& p,VJKG& q){
  return p[0];
}

effham::effham(){
  med = 0; med = med1;
  mer = 0;
  ccalc_ = cen.getDoubleNew("calc");
  cexp_ = cen.getDoubleNew("exp");
  cweig_ = cen.getDoubleNew("weight");
  fname.name("ham.par");
}

effham::~effham(){
}

void effham::newCalc(int jj,enlist *expl,enlist *calcl){
  Jmax_ = jj;
  cen.clear();
  ccalc_ = cen.getDoubleNew("calc");
  cexp_ = cen.getDoubleNew("exp");
  cweig_ = cen.getDoubleNew("weight");
  vector<double> *eweig = expl->getDouble("weight");
  VJKG q;
  map<vibq,size_t> nvib;
  for(size_t i=0;i<expl->size();i++){
    q = expl->q(i);
    if (q.j > Jmax()) continue;
    nvib[q.v] += 1;
  };
  q.j = q.k = q.g = 0;
  q.iso(expl->q(0).iso());
  vibs.clear();
  map<vibq,size_t>::iterator inv;
  for(inv=nvib.begin();inv!=nvib.end();inv++){
    make_dpar(inv->first);
    q.v = inv->first;
    inv->second = q.symm();
    vibs.push_back(inv->first);
  };
  int k0[2][4]={{0,1,1,2},{1,2,0,1}},g0[4]={0,1,0,1}; // starting values for Ka & Kc
  size_t j=0,isym,je;
  for(int J=0;J<=Jmax();J++)
    for(size_t sym=0;sym<q.nsymm();sym++)
    for(inv=nvib.begin();inv!=nvib.end();inv++){
      isym = inv->second;
      if (isym > 0) isym = 1;
      q.v = inv->first;
      q.j = J;
      q.k = k0[isym][sym];
      q.g = J - g0[sym];
      for(;q.k<=J&&q.g>=0;q.k+=2,q.g-=2){
        cen.add(q,0.00);
        ccalc(j) = calcl?(calcl->ener(q)):0.;
        je = expl->index(q);
        if (je < expl->size()){
          cexp(j) = expl->ener(je);
          if (eweig) cweig(j) = (*eweig)[je];
          else
            cweig(j) = 1.;
        }else{
          cexp(j) = 0.;
          cweig(j) = 0.;
        };
        j++;
      };
    };
}

void effham::calc(bool fitt){
  bool fit = fitt;
  size_t sym = 100,n,i0,j,sym1,ilast,ip,np=0,ne=0;
  int J = -1;
  VJKG q1,q2;
  vector<size_t> im;
  matrix h,M,F;
  vec x;
  vector<double*> p;
  double pold,de,qs,qs0;
  bool fitting = false;

  if (fit){
    get_pars(p);
    np = p.size();
    p0.resize(np);
    for(size_t i=0;i<np;i++) p0[i] = *p[i];
    M.resize(np,np);
    x.resize(np);
    mio<<"Fitting "<<np<<" parameters."<<'\n';
    if (np == 0) fit = false;
  };

  if (cen.size()){
    J = cen.q(0).j;
    sym = cen.q(0).symm();
  };
  ilast = cen.size()-1;
  qs = 0.;
  for(size_t i=0;i<cen.size();i++){
    sym1 = cen.q(i).symm();
    if (sym1 != sym || cen.q(i).j != J || i == ilast){
      if (i == ilast) im.push_back(i);
      n = im.size();
      h.make(n,n);
      fitting = false;
      begin_fit:
      for(size_t i1=0;i1<n;i1++)
      for(size_t i2=i1;i2<n;i2++){
        q1 = cen.q(im[i1]);
        q2 = cen.q(im[i2]);
        if (i1 == i2) h(i1,i1) = ccalc(im[i1]) + D(q1);
        else
          h(i1,i2) = R(q1,q2);
      };
      // h.diag();
      if (!fitting){
        mio<<"J="<<J<<" symm="<<sym<<'\n';
        for(size_t i1=0;i1<n;i1++) {
          j = im[i1];
          cen.ener(j) = h(i1,i1);
          de = cen.ener(j)-cexp(j);
          qs += de*de*cweig(j);
          mio<<cen.q(j)<<' '<<strDouble(cen.ener(j),6,12)<<' '
                            <<strDouble(cexp(j),6,12)<<' '
                            <<strDouble(de*cweig(j),6,12)<<'\n';
        };
      }else{
        for(size_t i1=0;i1<n;i1++) {
          j = im[i1];
          F(i1,ip) = (cen.ener(j) - h(i1,i1))/(*p[ip]-p0[ip]);
        };
      };
      if (fit ){
        if (!fitting){
          fitting = true;
          ip = 0;
          if (p0[0])  *p[0] = p0[0]*1.001;
          else
            *p[0] = 0.001;
          F.resize(n,np);
          goto begin_fit;
        }else{
          *p[ip] = p0[ip];
          if (ip == np - 1) goto stop_fit;
          ip++;
          if (p0[ip])  *p[ip] = p0[ip]*1.001;
          else
            *p[ip] = 0.001;
          goto begin_fit;
        };
      };
      //------------------//
      stop_fit:
      if (fit){
        double sum;
        size_t ii;
        for(size_t k=0;k<np;k++){
           sum = x(k);
           for(size_t i=0;i<n;i++) {
             ii = im[i];
             sum += F(i,k)*(cen.ener(ii) - cexp(ii))*cweig(ii);
           };
           x(k) = sum;
           for(size_t j=0;j<np;j++){
              sum = M(k,j);
              for(size_t i=0;i<n;i++) {
                sum += F(i,k)*F(i,j)*cweig(im[i]);
              };
              M(k,j) = sum;
           };
        };

      };
      im.clear();
      J = cen.q(i).j;
      sym = sym1;
    };
    im.push_back(i);
  };
  if (fit){
    if (!M.solve(x)){
      mio<<"Fit failed\n";
      return;
    };
    mio<<"---------------------------------------\n";
    for(ip=0;ip<np;ip++) {
      mio<<ip<<") "<<*p[ip];
      *p[ip] += x[ip];
      mio<<' '<<*p[ip]<<'\n';
    };
  };
  mio<<"qs="<<qs<<'\n';
}

void effham::make_dpar(vibq v){
//  size_t n = 6;
  size_t nn = 4;//n*(n-1)/2 + 1;
  params& par = dpar[v];
  if (par.size() != nn){
    par.resize(nn);
    par.data() = 0.;
    for(size_t i=0;i<nn;i++) par.fix(i);
  };
}

double effham::D(VJKG& q){
  if (med) return med(dpar[q.v].data(),q);
  else
    return 0.;
}

double effham::R(VJKG& q1,VJKG& q2){
  return 0.;
}

void effham::loadp(string fn){
  ifstream fil(fn.c_str());
  if (!fil) {
    mio<<"Cannot load parameters\n";
    return;
  };
  string str;
  istringstream istr(str);
  size_t nd,nr;
  getline(fil,str); istr.str(str);
  istr>>nd>>nr;
  if (nd > 1000 || nr > 1000){
    mio<<"Trying to load too many parameters\n";
    return;
  };
  vibq v;
  params *p;
  bool fix;
  double d;
  while(getline(fil,str)){
    v = str;
    mio<<v<<'\n';
    p = &dpar[v];
    p->resize(nd);
    for(size_t i=0;i<nd;i++){
      getline(fil,str); istr.str(str);
      istr>>d>>fix;
      (*p)[i] = d;
      if (fix) (*p).fix(i);
    };
  };
  fname.name(fn);
  mio<<dpar.size()<<'\n';
}

void effham::savep(string fn){
  ofstream fil(fn.c_str());
  if (dpar.size()==0) return;
  size_t nd =dpar.begin()->second.size();
  fil<<nd<<' '<<0<<'\n';
  map< vibq,params >::iterator p;
  for(p=dpar.begin();p!=dpar.end();p++){
    fil<<p->first<<'\n';
    for(size_t j=0;j<nd;j++)
      fil<<(*p).second[j]<<' '<<(*p).second.fixed(j)<<'\n';
  };
  fname.name(fn);
}


void effham::get_pars(vector< double* > &p){
  vector<vibq>::iterator v;
  params *pp;
  p.clear();
  for(v=vibs.begin();v!=vibs.end();v++){
    pp = &dpar[*v];
    for(size_t i=0;i<pp->size();i++)
      if (!pp->fixed(i)) p.push_back(&((*pp)[i]));
  };
}
