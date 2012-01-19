#ifndef MLDS_H
#define MLDS_H

#include "Goblin/DllExport.h"
//#include "../Troll1/cmd.h"
#include "Goblin/lineparams.h"
//#include "splist.h"

namespace Goblin
{

class GOBLIN_EXPORT multiparams_LDs:public lineparams_iso{
  double s1,s2;
  double y,sx,ww,h0,hi,dd,dl;
  double T,Z,L,Ps,P,A;
  double c2,c2tt;
  double Sprim;
public:
  vector<double> *w_,*intens_,*shift_,*self_,*nt_,*e0_;
//  Exp_cond *exp_cond;
  double T0_;
  map<int,double> Z0_;
  double def_shift,def_self,def_nt,def_e0;
  multiparams_LDs(splist& s);
  double T0(){return T0_;}
  void T0(double tt){T0_ = tt;}
  double Z0(int Iso){double zz=Z0_[Iso];return zz>0.?zz:1.;}
  void Z0(int Iso,double tt){Z0_[Iso] = tt;}
  double& w(size_t i){return (*w_)[i];}
  double& intens(size_t i){return (*intens_)[i];}
  double& shift(size_t i){return (*shift_)[i];}
  double& self(size_t i){return (*self_)[i];}
  double& nt(size_t i){return (*nt_)[i];}
  double& e0(size_t i){return (*e0_)[i];}

  virtual void init(spectrum* sp=0);
  virtual void init_line(size_t i,spectrum* sp=0);
  virtual double value(size_t i,double wx);

  virtual double line(size_t i,spectrum *sp=0){return i<nol()?(*w_)[i]:0.;}
  virtual double height(size_t i,spectrum *sp=0){return i<nol()?(*intens_)[i]:0.;}
  virtual void new_line(size_t i,double xx,double yy,spectrum *sp=0);
  virtual void set_line(size_t i,double xx,spectrum *sp=0);
  virtual void set_height(size_t i,double xx,spectrum *sp=0);
  virtual string print(size_t i);
  virtual void set_default(string s,double val);
  virtual double& par(size_t i,size_t j);
  virtual double dp(size_t i,size_t j);
  virtual void limit(size_t i); // checks and if necessary changes params if they are wrong
  virtual cmd_res cmd(string str);
  virtual double EW(size_t i,spectrum *sp=0);
  virtual void set(size_t i,string nam,double val);
};

} // Goblin

#endif
