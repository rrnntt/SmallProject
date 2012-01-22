#ifndef SPLIST_H
#define SPLIST_H

#include "Goblin/DllExport.h"

//#include "../Troll2/def.h"
#include "Goblin/spbase.h"
#include "Goblin/d_container.h"
#include "Goblin/comdiff.h"
#include "Goblin/exp_cond.h"
#include "Goblin/spectrum.h"
#include "Goblin/spline.h"

namespace Goblin
{

using namespace std;

class lineparams;
class enlist;

struct set4efit_data{
vector<int> *ind, *eind, *iind;
vector<double> *lin, *hi, *e0, *intf, *cint;
};

struct blend_struct{
  size_t i1;
  size_t i2;
  size_t im;
  blend_struct():i1(0),i2(0),im(0){}
  blend_struct(size_t j1,size_t j2,size_t jm):i1(j1),i2(j2),im(jm){}
};

class GOBLIN_EXPORT splist : public spbase  {
  vector<string> names;
  vector<double*> p; // pointers to fitting parameters
  vector<double> p0; // parameter values before fit
  vector<double> dp;
  vector<int> ip;    //  line index: ip[i] gives the line to which p[i] belongs; -1 means a global parameter
  vector<double*> err; // pointers to fit errors
  data_container<spectrum> calc;
  vector<spline*> bl;
  vector<double> y;  // experimental data vector
  vector<double> f;  // current calc data vector
  vector<double> weig;
  int fit_i1,fit_i2;
  size_t ny,np,ns,npe,npi; // size of data vector
  bool noise_;
public:
  VJKG bad_q;
  vector<VJKG> *q_p;
  vector<VJKG> *q0_p;
#ifndef GRAPHICS
  vector<double> *line_, *height_;
#endif
  //--- connect ----//
  vector<int> *cnnct;
  double find_dw,find_eh;
  size_t find_max;
  bool follow;
  //----------------//
  Exp_cond exp_cond;
  comdiff cd;
  lineparams *lp;
  //rgb cur_color;
  typedef data_container<spectrum>::iterator s_iter;
  data_container<spectrum> spectra;
  enum STYLE {FROM_ZERO, FROM_TOP, WHOLE, NONE};
  STYLE style;
  bool fit_vis;
  bool mouse_add_line;
  splist();
  ~splist();
  bool load(string fn);
  bool save(string fn);
  bool save();
//  void loadFromFile(const string fn);
  string print(size_t i);
  string print(){return print(curr());}
  void apply_params();
  void erase(size_t i1,size_t i2);
  void renew(size_t i1,size_t i2);
  void sort();
  VJKG& q(size_t i){return (q_p&&i<size())?(*q_p)[i]:bad_q; }
  VJKG& q0(size_t i){return (q0_p&&i<size())?(*q0_p)[i]:bad_q; }
  VJKG& q(){return q(curr());}
  VJKG& q0(){return q0(curr());}
  void make_ener(enlist& en,const string e0f);
  void make_ener_tag(enlist& en,const string e0f);
  bool assigned(){return q_p && q0_p;}
  void make_quanta();
  cmd_res cmd(string str);
  fun_res fun(string str);
  cmd_res message(object* s,string m);
//  cmd_res set_lines(string str);
  void copy(splist& ll);
  void connect(splist& ll,string height,string height1="");
  void reconnect(splist& ll);
  void connect_q(splist& ll);
  void connect_q0(splist& ll,string ll_hei);
  double QS(double w, double hh,double w1, double hh1);
  void disconnect();
  bool connected(){return cnnct != nullptr;}
  bool connectedTo(splist& ll);
  splist* getConnected();
  int con(size_t i){return i<size()?(*cnnct)[i]:-1;}
  int con(){return con(curr());}
  void set_conn(size_t i,int j){(*cnnct)[i] = j;}
  double line(size_t i);
  double height(size_t i);
  void height(size_t i,double h);
  double line(){return line(curr());}
  double height(){return height(curr());}
  bool assignment();
  void assign(size_t i,string str);
  bool isblend(size_t i,double h=1.,blend_struct *b=0);
  void line_error(string e0f,double h=1.);
#ifdef GRAPHICS
  int addLine(double w,double h,spectrum *sp=0);
#else
  int addLine(double w,double h);
#endif
  void deleteLine(size_t i);
#ifdef GRAPHICS
  bool draw(canvas& c);
  cmd_res mouseClick(canvas* c,int x,int y,int shft=0);
  cmd_res mouseDoubleClick(canvas* c,int x,int y,int shft=0);
#endif
  lineparams& lparams(){return *lp;}
  void setParamType(string);
  bool setSpectrum(string sn);
  bool delSpectrum(string sn);
  bool setSpectrum();
  void renew();
  void residual(size_t j);
  double fit(size_t i1,size_t i2,bool out=true);
  double fit_dud(size_t i1,size_t i2,bool out=true);
  double fit_der(size_t i1,size_t i2,bool out=true);
  double fit_reg(size_t i1,size_t i2,bool out=true);
  void unfit();
  bool noise_f(){return noise_;}
  void noise_f(bool nf){noise_ = nf;}
  double fitq(double h1,double h2,enlist* pen=0);
  void efit(enlist& en,vector<int>& ind,bool int_fit=false,bool out=true);
  void set4efit(int jp,set4efit_data& dt);
  void prep4efit();
  void prep_q4efit(enlist& en,VJKG& qq,bool info=false);
};

} // Goblin

#endif
