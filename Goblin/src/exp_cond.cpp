#include "exp_cond.h"
#include "vjkg.h"
using namespace std;

void mixture::add(int IIso){
  vector<int>::iterator i;
  for(i=Iso.begin();i!=Iso.end();i++){
    if (IIso == *i) return;
  };
  Iso.push_back(IIso);
  Z_.push_back(1.);
}

void mixture::T(double tt){
  temper_ = tt;
}

void mixture::copy(mixture& mix){
  T(mix.T());
  P(mix.P());
  Z_.resize(mix.Z_.size());
  Z_ = mix.Z_;
  Iso.resize(mix.Iso.size());
  Iso = mix.Iso;
}

//----------------------------------------------------------------------

Exp_cond::Exp_cond():temper_(),p_tot(){
}
Exp_cond::~Exp_cond(){
}

double Exp_cond::P(int Iso){
  vector<mixture>::iterator im;
  vector<int>::iterator is;
  for(im=mix.begin();im!=mix.end();im++)
  for(is=im->Iso.begin();is!=im->Iso.end();is++)
    if (*is == Iso) return im->P();
  return 0;
}

double Exp_cond::Z(int Iso){
  vector<mixture>::iterator im;
  vector<int>::iterator is;
  size_t i;
  for(im=mix.begin();im!=mix.end();im++)
  for(is=im->Iso.begin(),i=0;is!=im->Iso.end();is++,i++)
    if (*is == Iso) return im->Z(i);
  return 1.;
}

void Exp_cond::Z(int Iso,double zz){
  vector<mixture>::iterator im;
  vector<int>::iterator is;
  size_t i;
  for(im=mix.begin();im!=mix.end();im++)
  for(is=im->Iso.begin(),i=0;is!=im->Iso.end();is++,i++)
    if (*is == Iso) im->Z(i,zz);
}

void Exp_cond::Z(double zz){
  size_t nm = nmix();
  if (!nm) return;
  size_t ni = niso(nm-1);
  if (!ni) return;
  mix[nm-1].Z(ni-1,zz);
  //cerr<<"OK "<<ni<<' '<<nm<<' '<<zz<<endl;
}
void Exp_cond::addIso(size_t im,int Iso){
  if (im < mix.size()){
    mix[im].add(Iso);
  };
}

void Exp_cond::T(double tt){
  temper_ = tt;
  vector<mixture>::iterator im;
  for(im=mix.begin();im!=mix.end();im++)
     im->T(tt);
}

void Exp_cond::copy(Exp_cond& e){
  temper_ = e.temper_;
  length_ = e.length_;
  mix.resize(e.nmix());
  for(size_t i=0;i<mix.size();i++)
    mix[i].copy(e.mix[i]);
  p_tot = e.p_tot;
}
