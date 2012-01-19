#ifndef LINEPARAMS_H
#define LINEPARAMS_H

#include "Goblin/DllExport.h"
#include "Goblin/splist.h"
#include "Goblin/spectrum.h"
#include "Goblin/voigt.h"

#include <vector>

namespace Goblin
{

typedef vector<double>::iterator DVI;
typedef vector<int>::iterator IVI;


class GOBLIN_EXPORT lineparams {
public:
  vector<string> names;
  double bad_par;
  static char cfix[];
  splist* spl;
  vector<int> *fixp_;
  size_t psize;
  vector<bool> default_fix;
  bool der_;
  bool error_;
  enum fit_type {DER,DUD,REG};
  fit_type fit_t_;
  vector<string> err_names;
  vector< vector<double>* > errs;
  lineparams():spl(0),psize(0),default_fix(50,false),der_(true),error_(false),fit_t_(DER){}
  lineparams(splist& s):spl(&s),psize(0),default_fix(50,false),der_(true),error_(false),fit_t_(DER)
     {fixp_ = &(s.getFieldNew<int>("i",cfix)->data);}
  virtual ~lineparams(){}
  bool fixed(size_t i,size_t j){return i<nol()?((*fixp_)[i] & (1<<j)):true;}
  void fix(size_t i,size_t j){if (i<nol()) (*fixp_)[i] |= (1<<j);}
  void unfix(size_t i,size_t j){if (i<nol()) (*fixp_)[i] ^= (1<<j);}
  void def_fix(string ff);
  int def_fix();
  size_t size(){return psize;}  // number of all parameters
  size_t nol(){return spl?spl->size():0;} // number of lines in linelist *spl
  bool der(){return der_;}
  void der(bool boo){der_=boo;}
  bool error(){return error_;} // true if fit errors can be calculated
  void error(bool boo);
  double& err(size_t i,size_t j);
  bool allowed(size_t i){return spl->allowed(i);}
  fit_type fit_t(){return fit_t_;}
  void fit_t(fit_type t){der(t==DER);fit_t_ = t;}
  virtual void init(spectrum* sp=0){}
  virtual void init_line(size_t i,spectrum* sp=0){}
  virtual double value(size_t i,double ww){return 0;}

  virtual double line(size_t i,spectrum *sp=0){return i*0.;};
  virtual double height(size_t i,spectrum *sp=0){return i*0.;};
  virtual void new_line(size_t i,double xx,double yy,spectrum *sp=0){}
  virtual void set_line(size_t i,double xx,spectrum *sp=0){}
  virtual void set_height(size_t i,double xx,spectrum *sp=0){}
  virtual string print(size_t i){return string("line");};
  virtual void set_default(string s,double val);
  virtual double& par(size_t i,size_t j){return bad_par;}
  virtual double dp(size_t i,size_t j){return 0.00001;}
  virtual void limit(size_t i){} // checks and if necessary changes params if they are aparently wrong
  virtual cmd_res cmd(string str){return false;}
  virtual double EW(size_t i,spectrum *sp=0){return 0.;}
  virtual void set(size_t i,string nam,double val){}
};

lineparams *new_lineparams(string s,splist& spl);

class GOBLIN_EXPORT lineparams_dmp:public lineparams{
public:
  static Voigt voigt;

  //static char cline[];
  //static char cheight[];
  //static char cwidth[];
  //static char cdamping[];

  vector<double> *w_,*hi_,*wdt_,*dmp_,*height_;
  double def_wdt,def_dmp;
  lineparams_dmp(splist& s);
  double& w(size_t i){return (*w_)[i];}
  double& hi(size_t i){return (*hi_)[i];}
  double& wdt(size_t i){return (*wdt_)[i];}
  double& dmp(size_t i){return (*dmp_)[i];}

//  virtual void init(spectrum* sp=0);
  virtual void init_line(size_t i,spectrum* sp=0);
  virtual double value(size_t i,double wx);

  double line(size_t i,spectrum *sp=0){return i<nol()?w(i):0.;}
  double height(size_t i,spectrum *sp=0){return i<nol()?(*height_)[i]:0.;}
  void set_line(size_t i,double xx,spectrum *sp=0){w(i) = xx;}
  void set_height(size_t i,double xx,spectrum *sp=0){(*height_)[i] = xx;}
  void new_line(size_t i,double xx,double yy,spectrum *sp=0);
  void set_default(string s,double val);
  string print(size_t i);
  double& par(size_t i,size_t j);
  void limit(size_t i);
  cmd_res cmd(string str);
};

class GOBLIN_EXPORT lineparams_ser:public lineparams{
  double cc;
public:
  static Voigt voigt;
//  static char cline[];
//  static char cheight[];
//  static char cwidth[];
//  static char cdamping[];
//  static char chi1[];
//  static char cwdt1[];
  vector<double> *w_,*hi_,*wdt_,*dmp_,*height_,*hi1_,*wdt1_;
  double def_wdt,def_dmp,def_wdt1,def_hi1;
  lineparams_ser(splist& s):lineparams(s){
    psize = 6;
    names.resize(psize);
    names[0] = "line";
    names[1] = "height";
    names[2] = "width";
    names[3] = "damping";
    names[4] = "hi1";
    names[5] = "wdt1";
      w_ = &(s.getFieldNew<double>("d",names[0])->data);
     hi_ = &(s.getFieldNew<double>("d",names[1])->data);
    wdt_ = &(s.getFieldNew<double>("d",names[2])->data);
    dmp_ = &(s.getFieldNew<double>("d",names[3])->data);
    hi1_ = &(s.getFieldNew<double>("d",names[4])->data);
    wdt1_= &(s.getFieldNew<double>("d",names[5])->data);
    height_ = hi_;
    set_default("wdt",0.04);
    set_default("dmp",0.01);
    set_default("wdt1",0.0);
    set_default("hi1",0.0);
  }
  double& w(size_t i){return (*w_)[i];}
  double& hi(size_t i){return (*hi_)[i];}
  double& wdt(size_t i){return (*wdt_)[i];}
  double& dmp(size_t i){return (*dmp_)[i];}
  double& hi1(size_t i){return (*hi1_)[i];}
  double& wdt1(size_t i){return (*wdt1_)[i];}

//  virtual void init(spectrum* sp=0);
  virtual void init_line(size_t i,spectrum* sp=0);
  virtual double value(size_t i,double wx);

  double line(size_t i,spectrum *sp=0){return i<nol()?w(i):0.;}
  double height(size_t i,spectrum *sp=0){return i<nol()?(*height_)[i]:0.;}
  void set_line(size_t i,double xx,spectrum *sp=0){w(i) = xx;}
  void set_height(size_t i,double xx,spectrum *sp=0){(*height_)[i] = xx;}
  void new_line(size_t i,double xx,double yy,spectrum *sp=0);
  void set_default(string s,double val);
  string print(size_t i);
  double& par(size_t i,size_t j);
  void limit(size_t i);
  cmd_res cmd(string str);
};

class GOBLIN_EXPORT lineparams_LD:public lineparams{
  double s1_,s2_,dl_,dd_,y_,sx_,h0_;
public:
//  static char cline[];
//  static char cheight[];
//  static char clorentz[];
//  static char cdoppler[];
  vector<double> *w_,*hi_,*lor_,*dop_,*height_;
  vector<VJKG> *q_p,*q0_p;
  Exp_cond *exp_cond;
  double def_lor,def_dop;
  lineparams_LD(splist& s);
  double& w(size_t i){return (*w_)[i];}
  double& hi(size_t i){return (*hi_)[i];}
  double& lor(size_t i){return (*lor_)[i];}  //  full lorentzian width
  double& dop(size_t i){return (*dop_)[i];}  //  full doppler width

//  virtual void init(spectrum* sp=0);
  virtual void init_line(size_t i,spectrum* sp=0);
  virtual double value(size_t i,double wx);

  double line(size_t i,spectrum *sp=0){return i<nol()?w(i):0.;}
  double height(size_t i,spectrum *sp=0){return i<nol()?(*height_)[i]:0.;}
  void set_line(size_t i,double xx,spectrum *sp=0){w(i) = xx;}
  void set_height(size_t i,double xx,spectrum *sp=0){(*height_)[i] = xx;}
  void new_line(size_t i,double xx,double yy,spectrum *sp=0);
  void set_default(string s,double val);
  double& par(size_t i,size_t j);
  VJKG& q(size_t i){return  (q_p && i<w_->size())? (*q_p)[i] :VJKG::bad_q; }
  VJKG& q0(size_t i){return  (q0_p && i<w_->size())? (*q0_p)[i] :VJKG::bad_q; }
  string print(size_t i);
  void limit(size_t i);
  cmd_res cmd(string str);
  bool assigned(){return q_p != 0;}
  double T(){return exp_cond->T();}
  double EW(size_t i,spectrum *sp=0);
};

class GOBLIN_EXPORT lineparams_iso:public lineparams{
protected:
  static string ciso;
  vector<VJKG> *q_p,*q0_p;
  vector<int> *iso_;
  int def_iso;
public:
  lineparams_iso(splist& s);
  VJKG& q(size_t i){return (*q_p)[i];}
  VJKG& q0(size_t i){return (*q0_p)[i];}
  int iso(size_t i){return (iso_)?((*iso_)[i]):q(i).Iso();}
  double mass(size_t i){return (iso_)?(molecule::mass((*iso_)[i])):q(i).mass();}
  bool assigned(){return q_p != 0;}
  void set_default(string s,double val);
  void set(size_t i,string nam,double val);
};


class GOBLIN_EXPORT lineparams_gal:public lineparams_iso{
  double s1_,s2_,dl_,dd_,y_,z_,sx_,h0_;
public:
//  static char cline[];
//  static char cheight[];
//  static char clorentz[];
//  static char cdoppler[];
//  static char cnarrow[];
  vector<double> *w_,*hi_,*lor_,*dop_,*height_,*nar_;
  Exp_cond *exp_cond;
  double def_lor,def_dop,def_nar;
  lineparams_gal(splist& s);
  double& w(size_t i){return (*w_)[i];}
  double& hi(size_t i){return (*hi_)[i];}
  double& lor(size_t i){return (*lor_)[i];}  //  full lorentzian width
  double& dop(size_t i){return (*dop_)[i];}  //  full doppler width
  double& nar(size_t i){return (*nar_)[i];}  //  narrowing parameter 

//  virtual void init(spectrum* sp=0);
  virtual void init_line(size_t i,spectrum* sp=0);
  virtual double value(size_t i,double wx);

  double line(size_t i,spectrum *sp=0){return i<nol()?w(i):0.;}
  double height(size_t i,spectrum *sp=0){return i<nol()?(*height_)[i]:0.;}
  void set_line(size_t i,double xx,spectrum *sp=0){w(i) = xx;}
  void set_height(size_t i,double xx,spectrum *sp=0){(*height_)[i] = xx;}
  void new_line(size_t i,double xx,double yy,spectrum *sp=0);
  void set_default(string s,double val);
  double& par(size_t i,size_t j);
  string print(size_t i);
  void limit(size_t i);
  cmd_res cmd(string str);
  double T(){return exp_cond->T();}
  void set(size_t i,string nam,double val);
};

class GOBLIN_EXPORT lineparams_galh:public lineparams_iso{
  double s1_,s2_,dl_,dd_,y_,z_,s_,sx_,h0_;
public:
//  static char cline[];
//  static char cheight[];
//  static char clorentz[];
//  static char cdoppler[];
//  static char cnarrow[];
//  static char ccshift[];
  vector<double> *w_,*hi_,*lor_,*dop_,*height_,*nar_,*cshift_;
  Exp_cond *exp_cond;
  double def_lor,def_dop,def_nar,def_csh;
  lineparams_galh(splist& s);
  double& w(size_t i){return (*w_)[i];}
  double& hi(size_t i){return (*hi_)[i];}
  double& lor(size_t i){return (*lor_)[i];}  //  full lorentzian width
  double& dop(size_t i){return (*dop_)[i];}  //  full doppler width
  double& nar(size_t i){return (*nar_)[i];}  //  narrowing parameter 
  double& csh(size_t i){return (*cshift_)[i];}  

//  virtual void init(spectrum* sp=0);
  virtual void init_line(size_t i,spectrum* sp=0);
  virtual double value(size_t i,double wx);

  double line(size_t i,spectrum *sp=0){return i<nol()?w(i):0.;}
  double height(size_t i,spectrum *sp=0){return i<nol()?(*height_)[i]:0.;}
  void set_line(size_t i,double xx,spectrum *sp=0){w(i) = xx;}
  void set_height(size_t i,double xx,spectrum *sp=0){(*height_)[i] = xx;}
  void new_line(size_t i,double xx,double yy,spectrum *sp=0);
  void set_default(string s,double val);
  double& par(size_t i,size_t j);
  string print(size_t i);
  void limit(size_t i);
  cmd_res cmd(string str);
  double T(){return exp_cond->T();}
  void set(size_t i,string nam,double val);
};

class GOBLIN_EXPORT multiparams_LD:public lineparams_iso{
  double s1,s2;
  double y,sx,ww,h0,hi,dd,dl;
  double T,Z,L,Ps,P,A;
  double c2,c2tt;
  double Sprim;
public:
//  static string cline;
//  static string cintens;
//  static string cshift;
//  static string cair;
//  static string cself;
//  static string cnt;
//  static string ce0;
  vector<double> *w_,*intens_,*shift_,*air_,*self_,*nt_,*e0_;
//  Exp_cond *exp_cond;
  double T0_;
  map<int,double> Z0_;
  double def_shift,def_self,def_nt,def_e0,def_air;
  multiparams_LD(splist& s);
  double T0(){return T0_;}
  void T0(double tt){T0_ = tt;}
  double Z0(int Iso){double zz=Z0_[Iso];return zz>0.?zz:1.;}
  void Z0(int Iso,double tt){Z0_[Iso] = tt;}
  double& w(size_t i){return (*w_)[i];}
  double& intens(size_t i){return (*intens_)[i];}
  double& shift(size_t i){return (*shift_)[i];}
  double& air(size_t i){return (*air_)[i];}
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

class GOBLIN_EXPORT lineparams_rau:public lineparams_iso{
  double s1_,s2_,dl_,dd_,y_,z_,sx_,h0_;
public:
//  static char cline[];
//  static char cheight[];
//  static char clorentz[];
//  static char cdoppler[];
//  static char cnarrow[];
  vector<double> *w_,*hi_,*lor_,*dop_,*height_,*nar_;
  Exp_cond *exp_cond;
  double def_lor,def_dop,def_nar;
  lineparams_rau(splist& s);
  double& w(size_t i){return (*w_)[i];}
  double& hi(size_t i){return (*hi_)[i];}
  double& lor(size_t i){return (*lor_)[i];}  //  full lorentzian width
  double& dop(size_t i){return (*dop_)[i];}  //  full doppler width
  double& nar(size_t i){return (*nar_)[i];}  //  narrowing parameter

//  virtual void init(spectrum* sp=0);
  virtual void init_line(size_t i,spectrum* sp=0);
  virtual double value(size_t i,double wx);

  double line(size_t i,spectrum *sp=0){return i<nol()?w(i):0.;}
  double height(size_t i,spectrum *sp=0){return i<nol()?(*height_)[i]:0.;}
  void set_line(size_t i,double xx,spectrum *sp=0){w(i) = xx;}
  void set_height(size_t i,double xx,spectrum *sp=0){(*height_)[i] = xx;}
  void new_line(size_t i,double xx,double yy,spectrum *sp=0);
  void set_default(string s,double val);
  double& par(size_t i,size_t j);
  string print(size_t i);
  void limit(size_t i);
  cmd_res cmd(string str);
  double T(){return exp_cond->T();}
  void set(size_t i,string nam,double val);
};

class GOBLIN_EXPORT lineparams_grau:public lineparams_iso{
  double s1_,s2_,dl_,dd_,y_,z_,c_,sx_,h0_;
public:
//  static char cline[];
// static char cheight[];
//  static char clorentz[];
//  static char cdoppler[];
//  static char cnarrow[];
//  static char cxi[];
  vector<double> *w_,*hi_,*lor_,*dop_,*height_,*nar_,*xi_;
  Exp_cond *exp_cond;
  double def_lor,def_dop,def_nar,def_xi;
  lineparams_grau(splist& s);
  double& w(size_t i){return (*w_)[i];}
  double& hi(size_t i){return (*hi_)[i];}
  double& lor(size_t i){return (*lor_)[i];}  //  full lorentzian width
  double& dop(size_t i){return (*dop_)[i];}  //  full doppler width
  double& nar(size_t i){return (*nar_)[i];}  //  narrowing parameter
  double& xi(size_t i){return (*xi_)[i];}  //  narrowing parameter

//  virtual void init(spectrum* sp=0);
  virtual void init_line(size_t i,spectrum* sp=0);
  virtual double value(size_t i,double wx);

  double line(size_t i,spectrum *sp=0){return i<nol()?w(i):0.;}
  double height(size_t i,spectrum *sp=0){return i<nol()?(*height_)[i]:0.;}
  void set_line(size_t i,double xx,spectrum *sp=0){w(i) = xx;}
  void set_height(size_t i,double xx,spectrum *sp=0){(*height_)[i] = xx;}
  void new_line(size_t i,double xx,double yy,spectrum *sp=0);
  void set_default(string s,double val);
  double& par(size_t i,size_t j);
  string print(size_t i);
  void limit(size_t i);
  cmd_res cmd(string str);
  double T(){return exp_cond->T();}
  void set(size_t i,string nam,double val);
};

} // Goblin

#endif
