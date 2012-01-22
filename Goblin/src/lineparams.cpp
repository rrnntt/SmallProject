
#include <math.h>
#include <cctype>
#include "lineparams.h"
#include "Goblin/humlicek.h"
#include "Goblin/mLDs.h"
#include "Goblin/mio.h"
//#include "ftlines.h"

namespace Goblin
{

lineparams *new_lineparams(string s,splist& spl){
  string ss;
  for(size_t i=0;i<s.size();i++)
    if (isalnum(s[i])) ss += s[i];
  if (ss == "dmp") return new lineparams_dmp(spl);
  if (ss == "LD") return new lineparams_LD(spl);
  if (ss == "gal") return new lineparams_gal(spl);
  if (ss == "galh") return new lineparams_galh(spl);
  if (ss == "mLD") return new multiparams_LD(spl);
  if (ss == "mLDs") return new multiparams_LDs(spl);
//  if (ss == "ft") return new ftlines(spl);
//  if (ss == "lor") return new lineparams_lor(spl);
  if (ss == "ser") return new lineparams_ser(spl);
  if (ss == "rau") return new lineparams_rau(spl);
  if (ss == "grau") return new lineparams_grau(spl);
  return 0;
}

char lineparams::cfix[] = "fix";

void lineparams::set_default(string s,double val){
}

void lineparams::def_fix(string s){
  for(size_t i=0;i<default_fix.size();i++)
    default_fix[i] = (s[i] != '0');
}

int lineparams::def_fix(){
  int fi = 0;
  for(size_t i=0;i<size();i++)
    if (default_fix[i]) fi += (1<<i);
  return fi;
}

void lineparams::error(bool boo){
  if (!boo){
    error_ = false;
    return;
  };
  error_ = false;
  if (!spl) return;
  err_names.resize(size());
  errs.resize(size());
  for(size_t i=0;i<size();i++){
     err_names[i] = names[i] + "_err";
     errs[i] = &(spl->getFieldNew<double>("d",err_names[i])->data);
     if (errs[i] == 0)  return;
  };
  error_ = true;
}

double& lineparams::err(size_t i,size_t j){
  if (!error()) return bad_par;
  if (i >= nol() || j >= size()) return bad_par;
  return (*errs[j])[i];
}

//-----------------------------------------------------------------------

//char lineparams_dmp::cline[] = "line";
//char lineparams_dmp::cheight[] = "height";
//char lineparams_dmp::cwidth[] = "width";
//char lineparams_dmp::cdamping[] = "damping";

Voigt lineparams_dmp::voigt;

lineparams_dmp::lineparams_dmp(splist& s):lineparams(s){
  psize = 4;
  names.resize(psize);
  names[0] = "line";
  names[1] = "height";
  names[2] = "width";
  names[3] = "damping";
    w_ = &(s.getFieldNew<double>("d",names[0])->data);
   hi_ = &(s.getFieldNew<double>("d",names[1])->data);
  wdt_ = &(s.getFieldNew<double>("d",names[2])->data);
  dmp_ = &(s.getFieldNew<double>("d",names[3])->data);
  height_ = hi_;
  set_default("wdt",0.04);
  set_default("dmp",0.01);
}
void lineparams_dmp::init_line(size_t i,spectrum* sp){
  double dmp1 = dmp(i);
  if (dmp1 < 0) dmp1 = 0.;
  if (dmp1 > 1.) dmp1 = 1.;
  voigt.vstart(dmp1);
}

double lineparams_dmp::value(size_t i,double wx){
  if (i<nol())  return hi(i)*voigt.voigt(wx-w(i),wdt(i));
  else
    return 0.;
}

string lineparams_dmp::print(size_t i){
  if (i>=nol()) return "";
  ostringstream ostr;
  ostr.precision(10);
  ostr << i<<" w:"<<w(i);
  if (fixed(i,0)) ostr<<'*';
  ostr <<" hi:"<<hi(i);
  if (fixed(i,1)) ostr<<'*';
  ostr <<" wdt:"<<wdt(i);
  if (fixed(i,2)) ostr<<'*';
  ostr <<" dmp:"<<dmp(i);
  if (fixed(i,3)) ostr<<'*';
  return ostr.str();
}

void lineparams_dmp::new_line(size_t i,double xx,double yy,spectrum *sp){
  set_line(i,xx);
  set_height(i,yy);
  wdt(i) = def_wdt;
  dmp(i) = def_dmp;
  for(size_t j=0;j<size();j++)
    if (default_fix[j]) fix(i,j);
}

void lineparams_dmp::set_default(string s,double val){
  if (s == "dmp") def_dmp = val;
  if (s == "wdt") def_wdt = val;
}

double& lineparams_dmp::par(size_t i,size_t j){
  switch (j){
    case 0:return w(i);
    case 1:return hi(i);
    case 2:return wdt(i);
    case 3:return dmp(i);
    default: return bad_par;
  };
}

void lineparams_dmp::limit(size_t i){
  if ( w(i) < 0 ) w(i) = 0;
  if ( hi(i) < 0 ) hi(i) = 0.000001;
  if ( wdt(i) < 0 ) wdt(i) = -wdt(i);
  if ( dmp(i) < 0 ) dmp(i) = 0;
  if ( dmp(i) > 1. ) dmp(i) = 0.9;
}

cmd_res lineparams_dmp::cmd(string str){
}

//-------------------------------------------------------------------------

//char lineparams_LD::cline[] = "line";
//char lineparams_LD::cheight[] = "height";
//char lineparams_LD::clorentz[] = "lorentz";
//char lineparams_LD::cdoppler[] = "doppler";

lineparams_LD::lineparams_LD(splist& s):lineparams(s){
  psize = 4;
  names.resize(psize);
  names[0] = "line";
  names[1] = "height";
  names[2] = "lorentz";
  names[3] = "doppler";
  w_ = &(s.getFieldNew<double>("d",names[0])->data);
  hi_ = &(s.getFieldNew<double>("d",names[1])->data);
  lor_ = &(s.getFieldNew<double>("d",names[2])->data);
  dop_ = &(s.getFieldNew<double>("d",names[3])->data);
  height_ = hi_;
  set_default("lor",0.01);
  set_default("dop",0.01);
  db_field<VJKG>* qf = s.getField<VJKG>("vjkg","q");
  q_p = qf?&qf->data:0;
  qf = s.getField<VJKG>("vjkg","q0");
  q0_p = qf?&qf->data:0;
  exp_cond = &s.exp_cond;
}


void lineparams_LD::init_line(size_t i,spectrum* sp){
  s1_ = sqrt(log(2.));
  s2_ = 2*s1_;
  dl_ = lor(i);
  dd_ = dop(i);
  if (dd_ < 0) dd_ *= -1;
  if (dl_ < 0) dl_ *= -1;

  dd_ = dd_>0?dd_:0.00001;
  y_ = s1_*dl_/dd_;
  sx_ = s2_/dd_;
  h0_ = humlicek(0,y_);
}

double lineparams_LD::value(size_t i,double wx){
  if (i<nol())  return hi(i)*humlicek(sx_*(wx-w(i)),y_)/h0_;
  else
    return 0.;
}


double lineparams_LD::EW(size_t i,spectrum *sp){
  init_line(i);
  double ew = hi(i)/h0_;
  return ew;
}

string lineparams_LD::print(size_t i){
  if (i>=nol()) return "";
  ostringstream ostr;
  ostr << i;
  ostr.precision(9);
  ostr<<" w:"<<w(i);
  if (fixed(i,0)) ostr<<'*';
  ostr<<" hi:"<<hi(i);
  if (fixed(i,1)) ostr<<'*';
  ostr<<" lor:"<<lor(i);
  if (fixed(i,2)) ostr<<'*';
  ostr<<" dop:"<<dop(i);
  if (fixed(i,3)) ostr<<'*';
  return ostr.str();
}

void lineparams_LD::new_line(size_t i,double xx,double yy,spectrum *sp){
  set_line(i,xx);
  set_height(i,yy);
  lor(i) = def_lor;
  if ( !assigned() || !T() ) dop(i) = def_dop;
  else{
     dop(i) = 7.162403e-7*sqrt(T()/q(i).mass())*xx;
//     fix(i,3);
  };
  for(size_t j=0;j<size();j++)
    if (default_fix[j]) fix(i,j);
}

void lineparams_LD::set_default(string s,double val){
  if (s == "dop") def_dop = val;
  if (s == "lor") def_lor = val;
}

double& lineparams_LD::par(size_t i,size_t j){
  switch (j){
    case 0:return w(i);
    case 1:return hi(i);
    case 2:return lor(i);
    case 3:return dop(i);
    default: return bad_par;
  };
}

void lineparams_LD::limit(size_t i){
  if ( w(i) < 0 ) w(i) = 0;
  if ( hi(i) < 0 ) hi(i) = 0.000001;
  if ( lor(i) < 0 ) lor(i) = -lor(i);
  if ( dop(i) < 0 ) dop(i) = -dop(i);
}

cmd_res lineparams_LD::cmd(string str){
}

//-------------------------------------------------------------------------

//char lineparams_gal::cline[] = "line";
//char lineparams_gal::cheight[] = "height";
//char lineparams_gal::clorentz[] = "lorentz";
//char lineparams_gal::cdoppler[] = "doppler";
//char lineparams_gal::cnarrow[] = "narrow";

lineparams_gal::lineparams_gal(splist& s):lineparams_iso(s){
  psize = 5;
  names.resize(psize);
  names[0] = "line";
  names[1] = "height";
  names[2] = "lorentz";
  names[3] = "doppler";
  names[4] = "narrow";
  w_ = &(s.getFieldNew<double>("d",names[0])->data);
  hi_ = &(s.getFieldNew<double>("d",names[1])->data);
  lor_ = &(s.getFieldNew<double>("d",names[2])->data);
  dop_ = &(s.getFieldNew<double>("d",names[3])->data);
  nar_ = &(s.getFieldNew<double>("d",names[4])->data);
  height_ = hi_;
  set_default("lor",0.01);
  set_default("dop",0.01);
  set_default("nar",0.005);
  exp_cond = &s.exp_cond;
}


void lineparams_gal::init_line(size_t i,spectrum* sp){
  s1_ = sqrt(log(2.));
  s2_ = 2*s1_;
  dl_ = lor(i);
  dd_ = dop(i);
  if (dd_ < 0) dd_ *= -1;
  if (dl_ < 0) dl_ *= -1;

  dd_ = dd_>0?dd_:0.00001;
  y_ = s1_*dl_/dd_;
  z_ = nar(i)/dd_;
  sx_ = s2_/dd_;
  h0_ = galatryG(0,y_,z_);
}

double lineparams_gal::value(size_t i,double wx){
  if (i<nol())  return hi(i)*galatryG(sx_*(wx-w(i)),y_,z_)/h0_;
  else
    return 0.;
}


string lineparams_gal::print(size_t i){
  if (i>=nol()) return "";
  ostringstream ostr;
  ostr << i;
  ostr<<" w:"<<w(i);
  if (fixed(i,0)) ostr<<'*';
  ostr<<" hi:"<<hi(i);
  if (fixed(i,1)) ostr<<'*';
  ostr<<" lor:"<<lor(i);
  if (fixed(i,2)) ostr<<'*';
  ostr<<" dop:"<<dop(i);
  if (fixed(i,3)) ostr<<'*';
  ostr<<" nar:"<<nar(i);
  if (fixed(i,4)) ostr<<'*';
  return ostr.str();
}

void lineparams_gal::new_line(size_t i,double xx,double yy,spectrum *sp){
  set(i,"iso",def_iso);
  set_line(i,xx);
  set_height(i,yy);
  lor(i) = def_lor;
  nar(i) = def_nar;
  mio<<"new line "<<assigned()<<' '<<T()<<' '<<exp_cond->L()<<'\n';
  if (default_fix[3] && assigned() && T() ) {
     dop(i) = 7.162403e-7*sqrt(T()/mass(i))*xx;
//     fix(i,3);
  }else
    dop(i) = def_dop;

  for(size_t j=0;j<size();j++)
    if (default_fix[j]) fix(i,j);
}

void lineparams_gal::set_default(string s,double val){
  if (s == "dop") def_dop = val;
  else if (s == "lor") def_lor = val;
  else if (s == "nar") def_nar = val;
  else lineparams_iso::set_default(s,val);
}

void lineparams_gal::set(size_t i,string nam,double val){
  if (i >= nol()) return;
  else if (nam == "line") w(i) = val;
  else if (nam == "height") hi(i) = val;
  else if (nam == "lorentz") lor(i) = val;
  else if (nam == "doppler") dop(i) = val;
  else if (nam == "narrow") nar(i) = val;
  else lineparams_iso::set(i,nam,val);
}

double& lineparams_gal::par(size_t i,size_t j){
  switch (j){
    case 0:return w(i);
    case 1:return hi(i);
    case 2:return lor(i);
    case 3:return dop(i);
    case 4:return nar(i);
    default: return bad_par;
  };
}

void lineparams_gal::limit(size_t i){
  if ( w(i) < 0 ) w(i) = 0;
  if ( hi(i) < 0 ) hi(i) = 0.000001;
  if ( lor(i) < 0 ) lor(i) = -lor(i);
  if ( dop(i) < 0 ) dop(i) = -dop(i);
  if ( nar(i) < 0 ) nar(i) = -nar(i);
}

cmd_res lineparams_gal::cmd(string str){
  return false;
}

//-------------------------------------------------------------------------

//char lineparams_galh::cline[] = "line";
//char lineparams_galh::cheight[] = "height";
//char lineparams_galh::clorentz[] = "lorentz";
//char lineparams_galh::cdoppler[] = "doppler";
//char lineparams_galh::cnarrow[] = "narrow";
//char lineparams_galh::ccshift[] = "cshift";

lineparams_galh::lineparams_galh(splist& s):lineparams_iso(s){
  psize = 6;
  names.resize(psize);
  names[0] = "line";
  names[1] = "height";
  names[2] = "lorentz";
  names[3] = "doppler";
  names[4] = "narrow";
  names[5] = "cshift";
  w_ = &(s.getFieldNew<double>("d",names[0])->data);
  hi_ = &(s.getFieldNew<double>("d",names[1])->data);
  lor_ = &(s.getFieldNew<double>("d",names[2])->data);
  dop_ = &(s.getFieldNew<double>("d",names[3])->data);
  nar_ = &(s.getFieldNew<double>("d",names[4])->data);
  cshift_ = &(s.getFieldNew<double>("d",names[5])->data);
  height_ = hi_;
  set_default("lor",0.01);
  set_default("dop",0.01);
  set_default("nar",0.005);
  set_default("csh",0.);
  exp_cond = &s.exp_cond;
}


void lineparams_galh::init_line(size_t i,spectrum* sp){
  s1_ = sqrt(log(2.));
  s2_ = 2*s1_;
  dl_ = lor(i);
  dd_ = dop(i);
  if (dd_ < 0) dd_ *= -1;
  if (dl_ < 0) dl_ *= -1;

  dd_ = dd_>0?dd_:0.00001;
  y_ = s1_*dl_/dd_;
  z_ = nar(i)/dd_;
  s_ = csh(i)/dd_;
  sx_ = s2_/dd_;
  h0_ = galatryH(0,y_,z_,s_);
}

double lineparams_galh::value(size_t i,double wx){
  if (i<nol())  return hi(i)*galatryH(sx_*(wx-w(i)),y_,z_,s_)/h0_;
  else
    return 0.;
}


string lineparams_galh::print(size_t i){
  if (i>=nol()) return "";
  ostringstream ostr;
  ostr << i;
  ostr<<" w:"<<w(i);
  if (fixed(i,0)) ostr<<'*';
  ostr<<" hi:"<<hi(i);
  if (fixed(i,1)) ostr<<'*';
  ostr<<" lor:"<<lor(i);
  if (fixed(i,2)) ostr<<'*';
  ostr<<" dop:"<<dop(i);
  if (fixed(i,3)) ostr<<'*';
  ostr<<" nar:"<<nar(i);
  if (fixed(i,4)) ostr<<'*';
  ostr<<" csh:"<<csh(i);
  if (fixed(i,5)) ostr<<'*';
  return ostr.str();
}

void lineparams_galh::new_line(size_t i,double xx,double yy,spectrum *sp){
  set(i,"iso",def_iso);
  set_line(i,xx);
  set_height(i,yy);
  lor(i) = def_lor;
  nar(i) = def_nar;
  csh(i) = def_csh;
  if ( !assigned() || !T() ) dop(i) = def_dop;
  else{
     dop(i) = 7.162403e-7*sqrt(T()/mass(i))*xx;
//     fix(i,3);
  };
  for(size_t j=0;j<size();j++)
    if (default_fix[j]) fix(i,j);
}

void lineparams_galh::set_default(string s,double val){
  if (s == "dop") def_dop = val;
  else if (s == "lor") def_lor = val;
  else if (s == "nar") def_nar = val;
  else if (s == "csh") def_csh = val;
  else lineparams_iso::set_default(s,val);
}

void lineparams_galh::set(size_t i,string nam,double val){
  if (i >= nol()) return;
  else if (nam == "line") w(i) = val;
  else if (nam == "height") hi(i) = val;
  else if (nam == "lorentz") lor(i) = val;
  else if (nam == "doppler") dop(i) = val;
  else if (nam == "narrow") nar(i) = val;
  else if (nam == "cshift") csh(i) = val;
  else lineparams_iso::set(i,nam,val);
}

double& lineparams_galh::par(size_t i,size_t j){
  switch (j){
    case 0:return w(i);
    case 1:return hi(i);
    case 2:return lor(i);
    case 3:return dop(i);
    case 4:return nar(i);
    case 5:return csh(i);
    default: return bad_par;
  };
}

void lineparams_galh::limit(size_t i){
  if ( w(i) < 0 ) w(i) = 0;
  if ( hi(i) < 0 ) hi(i) = 0.000001;
  if ( lor(i) < 0 ) lor(i) = -lor(i);
  if ( dop(i) < 0 ) dop(i) = -dop(i);
  if ( nar(i) < 0 ) nar(i) = -nar(i);
  if ( csh(i) < 0 ) csh(i) = -csh(i);
}

cmd_res lineparams_galh::cmd(string str){
  return false;
}

//-------------------------------------------------------------------------------

//string multiparams_LD::cline = "line";
//string multiparams_LD::cintens = "intens";
//string multiparams_LD::cself = "self";
//string multiparams_LD::cshift = "shift";
//string multiparams_LD::cair = "air";
//string multiparams_LD::cnt = "n_t";
//string multiparams_LD::ce0 = "e0";

multiparams_LD::multiparams_LD(splist& s):lineparams_iso(s){
  psize = 7;
  names.resize(psize);
  names[0] = "line";
  names[1] = "intens";
  names[2] = "self";
  names[3] = "shift";
  names[4] = "air";
  names[5] = "n_t";
  names[6] = "e0";
  w_ = &(s.getFieldNew<double>("d",names[0])->data);
  intens_ = &(s.getFieldNew<double>("d",names[1])->data);
  self_ = &(s.getFieldNew<double>("d",names[2])->data);
  shift_ = &(s.getFieldNew<double>("d",names[3])->data);
  air_ = &(s.getFieldNew<double>("d",names[4])->data);
  nt_ = &(s.getFieldNew<double>("d",names[5])->data);
  e0_ = &(s.getFieldNew<double>("d",names[6])->data);
  set_default("shift",0.01);
  set_default("air",0.01);
  set_default("self",0.01);
  set_default("nt",0.01);
  set_default("e0",100.);
  string str;
  str = s.params["temper"];
  if (str.empty()) T0(296.);
  else
    T0(atof(str.c_str()));
  str = s.params["zsum"];
  if (!str.empty()) {
    istringstream istr(str);
    string buf,siso,sz;
    size_t i;
    int Iso;
    mio<<"mLD: T0="<<T0();
    while(istr>>buf){
      if (buf.empty()) break;
      i = buf.find(':');
      if (i == 0 || i == string::npos) {
        Iso = 0; i = 0;
      }else
        Iso = atoi(buf.substr(0,i).c_str());
      Z0(Iso,atof(buf.substr(i+1).c_str()));
      mio<<"     Z0["<<Iso<<"]="<<Z0(Iso)<<'\n';
    };
  };
  def_shift = 0.;
  def_self = 0.4;
  def_nt =0.;
  def_e0=0.;
}


void multiparams_LD::new_line(size_t i,double xx,double yy,spectrum *sp){
  set(i,"iso",def_iso);
  set_line(i,xx,sp);
  set_height(i,yy,sp);
  self(i) = def_self;
  air(i) = def_air;
  shift(i) = def_shift;
  e0(i) = def_e0;
  nt(i) = def_nt;
  for(size_t j=0;j<size();j++)
    if (default_fix[j]) fix(i,j);
}

void multiparams_LD::set_line(size_t i,double xx,spectrum *sp){
  if (sp){
    double P = sp->exp_cond.P()/101300.;
    w(i) = xx - def_shift*P;
  }else
    w(i) = xx;
}

void multiparams_LD::set_height(size_t i,double xx,spectrum *sp){
  if (sp){
     static double s1 = sqrt(log(2.));
     static double s2 = 2*s1;
     double dd,dl,Ps,y;
     double T=sp->exp_cond.T()?(sp->exp_cond.T()):T0();
     double Z,L=sp->exp_cond.L();
     double A = 273.15*2.686763E19*L*s2/(T*sqrt(2.*acos(0.)))*100;
     static double c2 = 1.4387; // c2 = h*c/k
     double c2tt = c2*(1./T0()-1./T);

     Ps = sp->exp_cond.P(iso(i))/101300.;
     Z =  sp->exp_cond.Z(iso(i)) / Z0(iso(i));
     dl = 2*( def_self*Ps + def_air*(P - Ps) )*pow(T0()/T,def_nt);
     dd = 7.162403e-7*sqrt(T/mass(i))*w(i);
     if (dd < 0) dd *= -1;
     if (dl < 0) dl *= -1;
     dd = dd>0?dd:0.00001;
     y = s1*dl/dd;
//     double B = (A*Ps*erfc(y)/(dd*exp(-y*y)) /Z*exp(c2tt*def_e0));
     double B = (A*Ps*humlicek(0,y)/dd /Z*exp(c2tt*def_e0));
     intens(i) = xx/B;
  }else
     intens(i) = xx;
}

string multiparams_LD::print(size_t i){
  if (i>=nol()) return "";
  ostringstream ostr;
  ostr.precision(12);
  ostr <<"line "<<i<<' '<<line(i);
  if (fixed(i,0)) ostr<<'*';

  ostr.precision(5);
  ostr <<' '<<intens(i);
  if (fixed(i,1)) ostr<<'*';

  ostr <<' '<<self(i);
  if (fixed(i,2)) ostr<<'*';

  ostr <<' '<<shift(i);
  if (fixed(i,3)) ostr<<'*';

  ostr <<' '<<air(i);
  if (fixed(i,4)) ostr<<'*';

  ostr <<' '<<nt(i);
  if (fixed(i,5)) ostr<<'*';

  ostr.precision(12);
  ostr <<' '<<e0(i);
  if (fixed(i,6)) ostr<<'*';

  if (iso_)
    ostr <<"iso "<<iso(i);

  return ostr.str();
}

void multiparams_LD::set_default(string s,double val){
  if(s=="self"){
    def_self = val;
    return;
  };
  if(s=="shift"){
    def_shift = val;
    return;
  };
  if(s=="air"){
    def_air = val;
    return;
  };
  if(s=="nt"){
    def_nt = val;
    return;
  };
  if(s=="e0"){
    def_e0 = val;
    return;
  };
  lineparams_iso::set_default(s,val);

}

double& multiparams_LD::par(size_t i,size_t j){
  if (i>=nol()) return bad_par;
  switch(j){
    case 0: return w(i);
    case 1: return intens(i);
    case 2: return self(i);
    case 3: return shift(i);
    case 4: return air(i);
    case 5: return nt(i);
    case 6: return e0(i);
  };
  return bad_par;
}


void multiparams_LD::limit(size_t i){
}

cmd_res multiparams_LD::cmd(string str){
  istringstream istr(str);
  string command;
  istr >> command;

  // sets
  if (command == "line_error"){
    if (!error()) error(true);
    vector<double>* lerrp = spl->getDouble("lerr");
    if (!lerrp) {
      mio<<"Cannot find field lerr\n";
      return false;
    };
    vector<double>& lerr = *lerrp;
    double a0,a1,w1=1.,w2=1.,a;
    istr>>a0>>a1>>w1>>w2;
    for(size_t i=0;i<nol();i++){
      a = a0 + a1/height(i);
//      mio<<a0<<' '<<a1<<' '<<height(i)<<'\n';
      if (lerr[i]>a) a = lerr[i];
      if (a == 0.) mio<<"a("<<w(i)<<")="<<a<<'\n';
      (*errs[0])[i] = a;
      (*errs[1])[i] = w1*intens(i)*a;
      (*errs[2])[i] = w2*self(i)*a;
    };
    return false;
  };

  if (command == "T0"){
    double tt;
    istr >> tt;
    T0(tt);
    return false;
  };

  if (command == "Z0"){
    int Iso;
    double tt;
    istr >> Iso >> tt;
    Z0(Iso,tt);
    return false;
  };

  if (command == "voigt"){
    size_t is;
    istr>>is;
    if (!spl){
      mio<<"error\n";
      return false;
    };
    if (!spl->spectra.size()){
      mio<<"No spectrum attached\n";
      return false;
    };
    if (is >= spl->spectra.size()) {
      mio<<"No spectrum "<<is<<'\n';
      return false;
    };

    if (!spl->spectra[is].exp_sp){
      mio<<"error\n";
      return false;
    };
    spectrum *sp = spl->spectra[is].exp_sp;

    vector<double>* xhi = spl->getDoubleNew("height");
    vector<double>* xdl = spl->getDoubleNew("lorentz");
    vector<double>* xdd = spl->getDoubleNew("doppler");

    init(sp);
    for(size_t i=0;i<nol();i++){
      init_line(i,sp);
//      (*xhi)[i] = hi;
      (*xhi)[i] = Sprim;
      (*xdl)[i] = dl;
      (*xdd)[i] = dd;
    };
  };

  return false;
}

double multiparams_LD::dp(size_t i,size_t j){// i- number of line, j- number of parameter
  switch(j){
    case 0: return 0.0001;
    case 1: return intens(i)?intens(i)/1000:1e-30;
    case 2: return shift(i)?shift(i)/1000:0.0001;
    case 3: return self(i)?self(i)/1000:0.0001;
    case 4: return air(i)?air(i)/1000:0.0001;
    case 5: return nt(i)?nt(i)/1000:0.0001;
    case 6: return 0.0001;
  };
  return 0.;
}

double multiparams_LD::EW(size_t i,spectrum *sp){
  if (!sp) return 0.;
  static double s1 = sqrt(log(2.));
  static double s2 = 2*s1;
  double T=sp->exp_cond.T()?(sp->exp_cond.T()):T0();
  double Ps,Z,L=sp->exp_cond.L();
  static double c2 = 1.4387; // c2 = h*c/k
  double c2tt = c2*(1./T0()-1./T);

  Ps = sp->exp_cond.P(iso(i))/101300.;
  Z =  sp->exp_cond.Z(iso(i));
  return intens(i)*Z0(iso(i))/Z*273.15/T*2.686763E19*L  *Ps* exp(c2tt*e0(i));

}


void multiparams_LD::set(size_t i,string nam,double val){
  if (i >= nol()) return;
  else if (nam == "line") w(i) = val;
  else if (nam == "intens") intens(i) = val;
  else if (nam == "shift") shift(i) = val;
  else if (nam == "self") self(i) = val;
  else if (nam == "air") air(i) = val;
  else if (nam == "e0") e0(i) = val;
  else if (nam == "nt") nt(i) = val;
  else lineparams_iso::set(i,nam,val);
}



void multiparams_LD::init(spectrum* sp){
  s1 = sqrt(log(2.));
  s2 = 2*s1;
  P=sp->exp_cond.P()/101300.;
  T=sp->exp_cond.T()?(sp->exp_cond.T()):T0();
  L=sp->exp_cond.L();
//  A = 273.15*2.686763E19*L*s2/(T0()*sqrt(2.*acos(0.)))*100;
  A = 273.15*2.686763E19*L*s2/(T*sqrt(2.*acos(0.)))*100;
//  mio<<L<<' '<<s2<<' '<<T<<'\n';
  c2 = 1.4388; // c2 = h*c/k
  c2tt = c2*(1./T0()-1./T);
}

void multiparams_LD::init_line(size_t i,spectrum* sp){
  Ps = sp->exp_cond.P(iso(i))/101300.;
  double zz0 = Z0(iso(i));
  Z =  sp->exp_cond.Z(iso(i))/(zz0?zz0:1.);
  ww = w(i) + shift(i)*(P - Ps);
//  ww = w(i) + shift(i)*P;
  dl = 2*( self(i)*Ps + air(i)*(P - Ps) )*pow(T0()/T,nt(i));
  dd = 7.162403e-7*sqrt(T/mass(i))*ww;
  if (dd < 0) dd *= -1;
  if (dl < 0) dl *= -1;
  dd = dd>0?dd:0.00001;
  y = s1*dl/dd;
  sx = s2/dd;
  h0 = humlicek(0,y);
  hi = A*intens(i)*Ps*h0/dd /Z*exp(c2tt*e0(i));//*(1.-exp(-c2*ww/T))/(1.-exp(-c2*ww/T0()));
  Sprim = intens(i)/Z*exp(c2tt*e0(i));
//  mio<<"calc::OK "<<i<<' '<<A<<'\n';
}

double multiparams_LD::value(size_t i,double wx){
  if (i<nol()) return hi*humlicek(sx*(wx-ww),y)/h0;
  else
    return 0.;
}


//-----------------------------------------------------------------------

//char lineparams_ser::cline[] = "line";
//char lineparams_ser::cheight[] = "height";
//char lineparams_ser::cwidth[] = "width";
//char lineparams_ser::cdamping[] = "damping";
//char lineparams_ser::chi1[] = "hi1";
//char lineparams_ser::cwdt1[] = "wdt1";
Voigt lineparams_ser::voigt;

void lineparams_ser::init_line(size_t i,spectrum* sp){
  double w1 = fabs(wdt1(i));
  cc = (w1>0.0001)?(-log(2.)/(w1*w1)):0.;
  double dmp1 = dmp(i);
  if (dmp1 < 0) dmp1 = 0.;
  if (dmp1 > 1.) dmp1 = 1.;
  voigt.vstart(dmp1);
}

double lineparams_ser::value(size_t i,double wx){
  double x = wx-w(i);
  if (i<nol()) {
    if (cc) return hi(i)*( voigt.voigt(x,wdt(i)) + hi1(i)*exp(cc*x*x) );
    else
      return hi(i)*voigt.voigt(x,wdt(i));
  }else
    return 0.;
}

string lineparams_ser::print(size_t i){
  if (i>=nol()) return "";
  ostringstream ostr;
  ostr.precision(10);
  ostr << i<<" w:"<<w(i);
  if (fixed(i,0)) ostr<<'*';
  ostr <<" hi:"<<hi(i);
  if (fixed(i,1)) ostr<<'*';
  ostr <<" wdt:"<<wdt(i);
  if (fixed(i,2)) ostr<<'*';
  ostr <<" dmp:"<<dmp(i);
  if (fixed(i,3)) ostr<<'*';
  ostr <<" hi1:"<<hi1(i);
  if (fixed(i,4)) ostr<<'*';
  ostr <<" wdt1:"<<wdt1(i);
  if (fixed(i,5)) ostr<<'*';
  return ostr.str();
}

void lineparams_ser::new_line(size_t i,double xx,double yy,spectrum *sp){
  set_line(i,xx);
  set_height(i,yy);
  wdt(i) = def_wdt;
  dmp(i) = def_dmp;
  hi1(i) = def_hi1;
  wdt1(i) = def_wdt1;
  for(size_t j=0;j<size();j++)
    if (default_fix[j]) fix(i,j);
}

void lineparams_ser::set_default(string s,double val){
  if (s == "dmp") def_dmp = val;
  if (s == "wdt") def_wdt = val;
  if (s == "hi1") def_hi1 = val;
  if (s == "wdt1") def_wdt1 = val;
}

double& lineparams_ser::par(size_t i,size_t j){
  switch (j){
    case 0:return w(i);
    case 1:return hi(i);
    case 2:return wdt(i);
    case 3:return dmp(i);
    case 4:return hi1(i);
    case 5:return wdt1(i);
    default: return bad_par;
  };
}

void lineparams_ser::limit(size_t i){
  if ( w(i) < 0 ) w(i) = 0;
  if ( hi(i) < 0 ) hi(i) = 0.000001;
  if ( wdt(i) < 0 ) wdt(i) = -wdt(i);
  if ( dmp(i) < 0 ) dmp(i) = 0;
  if ( dmp(i) > 1. ) dmp(i) = 0.9;
}

cmd_res lineparams_ser::cmd(string str){
  return false;
}

//--------------------------------------------------------------
string lineparams_iso::ciso = "iso";

lineparams_iso::lineparams_iso(splist& s):lineparams(s){
  iso_ = 0;
  q_p = q0_p = 0;

  db_field<VJKG>* qf = s.getField<VJKG>("vjkg","q");
  if (qf){
     q_p = &qf->data;
     qf = s.getField<VJKG>("vjkg","q0");
     q0_p = qf?&qf->data:0;
  };
  if (!q_p || !q0_p){
    q_p = q0_p = 0;
    iso_ = &(s.getFieldNew<int>("i",ciso)->data);
  };
  def_iso = 11;
}


void lineparams_iso::set_default(string s,double val){
  if (s == "iso") def_iso = int(val);
}

void lineparams_iso::set(size_t i,string nam,double val){
  if (nam == "iso") {
    if (iso_) (*iso_)[i] = int(val);
    else if (q_p && q0_p) {
      q(i).iso(int(val));
      q0(i).iso(int(val));
    };
  };
}

//-------------------------------------------------------------------------

//char lineparams_rau::cline[] = "line";
//char lineparams_rau::cheight[] = "height";
//char lineparams_rau::clorentz[] = "lorentz";
//char lineparams_rau::cdoppler[] = "doppler";
//char lineparams_rau::cnarrow[] = "narrow";

lineparams_rau::lineparams_rau(splist& s):lineparams_iso(s){
  psize = 5;
  names.resize(psize);
  names[0] = "line";
  names[1] = "height";
  names[2] = "lorentz";
  names[3] = "doppler";
  names[4] = "narrow";
  w_ = &(s.getFieldNew<double>("d",names[0])->data);
  hi_ = &(s.getFieldNew<double>("d",names[1])->data);
  lor_ = &(s.getFieldNew<double>("d",names[2])->data);
  dop_ = &(s.getFieldNew<double>("d",names[3])->data);
  nar_ = &(s.getFieldNew<double>("d",names[4])->data);
  height_ = hi_;
  set_default("lor",0.01);
  set_default("dop",0.01);
  set_default("nar",0.005);
  exp_cond = &s.exp_cond;
}


void lineparams_rau::init_line(size_t i,spectrum* sp){
  static double sqpi = sqrt(2.*acos(0.));
  s1_ = sqrt(log(2.));
  s2_ = 2*s1_;
  dl_ = lor(i);
  dd_ = dop(i);
  if (dd_ < 0) dd_ *= -1;
  if (dl_ < 0) dl_ *= -1;

  dd_ = dd_>0?dd_:0.00001;
  y_ = s1_*dl_/dd_;
  z_ = nar(i)/dd_;
  y_ += z_;
  z_ *= sqpi;
  sx_ = s2_/dd_;
  double_complex pp = cprob(0,y_),hhh;
  hhh = pp/(1.-z_*pp);
  h0_ = hhh.real();
}

double lineparams_rau::value(size_t i,double wx){
  if (i<nol())  {
    double_complex pp = cprob(sx_*(wx-w(i)),y_),hhh;
    hhh = pp/(1.-z_*pp);
    return hi(i)*hhh.real()/h0_;
  }else
    return 0.;
}


string lineparams_rau::print(size_t i){
  if (i>=nol()) return "";
  ostringstream ostr;
  ostr << "rau "<< i;
  ostr<<" w:"<<w(i);
  if (fixed(i,0)) ostr<<'*';
  ostr<<" hi:"<<hi(i);
  if (fixed(i,1)) ostr<<'*';
  ostr<<" lor:"<<lor(i);
  if (fixed(i,2)) ostr<<'*';
  ostr<<" dop:"<<dop(i);
  if (fixed(i,3)) ostr<<'*';
  ostr<<" nar:"<<nar(i);
  if (fixed(i,4)) ostr<<'*';
  return ostr.str();
}

void lineparams_rau::new_line(size_t i,double xx,double yy,spectrum *sp){
  set(i,"iso",def_iso);
  set_line(i,xx);
  set_height(i,yy);
  lor(i) = def_lor;
  nar(i) = def_nar;
//  mio<<"new line "<<assigned()<<' '<<T()<<' '<<exp_cond->L()<<endl;
  if (default_fix[3] && (assigned()||iso_) && T() ) {
     dop(i) = 7.162403e-7*sqrt(T()/mass(i))*xx;
//     fix(i,3);
  }else
    dop(i) = def_dop;

  for(size_t j=0;j<size();j++)
    if (default_fix[j]) fix(i,j);
}

void lineparams_rau::set_default(string s,double val){
  if (s == "dop") def_dop = val;
  else if (s == "lor") def_lor = val;
  else if (s == "nar") def_nar = val;
  else lineparams_iso::set_default(s,val);
}

void lineparams_rau::set(size_t i,string nam,double val){
  if (i >= nol()) return;
  else if (nam == "line") w(i) = val;
  else if (nam == "height") hi(i) = val;
  else if (nam == "lorentz") lor(i) = val;
  else if (nam == "doppler") dop(i) = val;
  else if (nam == "narrow") nar(i) = val;
  else lineparams_iso::set(i,nam,val);
}

double& lineparams_rau::par(size_t i,size_t j){
  switch (j){
    case 0:return w(i);
    case 1:return hi(i);
    case 2:return lor(i);
    case 3:return dop(i);
    case 4:return nar(i);
    default: return bad_par;
  };
}

void lineparams_rau::limit(size_t i){
  if ( w(i) < 0 ) w(i) = 0;
  if ( hi(i) < 0 ) hi(i) = 0.000001;
  if ( lor(i) < 0 ) lor(i) = -lor(i);
  if ( dop(i) < 0 ) dop(i) = -dop(i);
  if ( nar(i) < 0 ) nar(i) = -nar(i);
}

cmd_res lineparams_rau::cmd(string str){
  return false;
}

//-------------------------------------------------------------------------

//char lineparams_grau::cline[] = "line";
//char lineparams_grau::cheight[] = "height";
//char lineparams_grau::clorentz[] = "lorentz";
//char lineparams_grau::cdoppler[] = "doppler";
//char lineparams_grau::cnarrow[] = "narrow";
//char lineparams_grau::cxi[] = "xi";

lineparams_grau::lineparams_grau(splist& s):lineparams_iso(s){
  psize = 6;
  names.resize(psize);
  names[0] = "line";
  names[1] = "height";
  names[2] = "lorentz";
  names[3] = "doppler";
  names[4] = "narrow";
  names[5] = "xi";
  w_ = &(s.getFieldNew<double>("d",names[0])->data);
  hi_ = &(s.getFieldNew<double>("d",names[1])->data);
  lor_ = &(s.getFieldNew<double>("d",names[2])->data);
  dop_ = &(s.getFieldNew<double>("d",names[3])->data);
  nar_ = &(s.getFieldNew<double>("d",names[4])->data);
  xi_ = &(s.getFieldNew<double>("d",names[5])->data);
  height_ = hi_;
  set_default("lor",0.01);
  set_default("dop",0.03);
  set_default("nar",0.0);
  set_default("xi",1.0);
  exp_cond = &s.exp_cond;
}


void lineparams_grau::init_line(size_t i,spectrum* sp){
  static double sqpi = sqrt(2.*acos(0.));
  s1_ = sqrt(log(2.));
  s2_ = 2*s1_;
  dl_ = lor(i);
  dd_ = dop(i);
  if (dd_ < 0) dd_ *= -1;
  if (dl_ < 0) dl_ *= -1;

  dd_ = dd_>0?dd_:0.00001;
  y_ = s1_*dl_/dd_;
  z_ = nar(i);
  c_ = xi(i)*y_;
  sx_ = s2_/dd_;
  double_complex pp = cprob(0,y_),hhh;
  hhh = pp/(1.-z_*cprob(0,c_));
  h0_ = hhh.real();
}

double lineparams_grau::value(size_t i,double wx){
  if (i<nol())  {
    double_complex pp = cprob(sx_*(wx-w(i)),y_),hhh;
    hhh = pp/(1.-z_*cprob(sx_*(wx-w(i)),c_));
    return hi(i)*hhh.real()/h0_;
  }else
    return 0.;
}


string lineparams_grau::print(size_t i){
  if (i>=nol()) return "";
  ostringstream ostr;
  ostr << "rau "<< i;
  ostr<<" w:"<<w(i);
  if (fixed(i,0)) ostr<<'*';
  ostr<<" hi:"<<hi(i);
  if (fixed(i,1)) ostr<<'*';
  ostr<<" lor:"<<lor(i);
  if (fixed(i,2)) ostr<<'*';
  ostr<<" dop:"<<dop(i);
  if (fixed(i,3)) ostr<<'*';
  ostr<<" nar:"<<nar(i);
  if (fixed(i,4)) ostr<<'*';
  ostr<<" xi:"<<xi(i);
  if (fixed(i,5)) ostr<<'*';
  return ostr.str();
}

void lineparams_grau::new_line(size_t i,double xx,double yy,spectrum *sp){
  set(i,"iso",def_iso);
  set_line(i,xx);
  set_height(i,yy);
  lor(i) = def_lor;
  nar(i) = def_nar;
  xi(i) = def_xi;
//  mio<<"new line "<<assigned()<<' '<<T()<<' '<<exp_cond->L()<<endl;
  if (default_fix[3] && (assigned()||iso_) && T() ) {
     dop(i) = 7.162403e-7*sqrt(T()/mass(i))*xx;
//     fix(i,3);
  }else
    dop(i) = def_dop;

  for(size_t j=0;j<size();j++)
    if (default_fix[j]) fix(i,j);
}

void lineparams_grau::set_default(string s,double val){
  if (s == "dop") def_dop = val;
  else if (s == "lor") def_lor = val;
  else if (s == "nar") def_nar = val;
  else if (s == "xi") def_xi = val;
  else lineparams_iso::set_default(s,val);
}

void lineparams_grau::set(size_t i,string nam,double val){
  if (i >= nol()) return;
  else if (nam == "line") w(i) = val;
  else if (nam == "height") hi(i) = val;
  else if (nam == "lorentz") lor(i) = val;
  else if (nam == "doppler") dop(i) = val;
  else if (nam == "narrow") nar(i) = val;
  else lineparams_iso::set(i,nam,val);
}

double& lineparams_grau::par(size_t i,size_t j){
  switch (j){
    case 0:return w(i);
    case 1:return hi(i);
    case 2:return lor(i);
    case 3:return dop(i);
    case 4:return nar(i);
    case 5:return xi(i);
    default: return bad_par;
  };
}

void lineparams_grau::limit(size_t i){
  if ( w(i) < 0 ) w(i) = 0;
  if ( hi(i) < 0 ) hi(i) = 0.000001;
  if ( lor(i) < 0 ) lor(i) = -lor(i);
  if ( dop(i) < 0 ) dop(i) = -dop(i);
  if ( nar(i) < 0 ) nar(i) = -nar(i);
}

cmd_res lineparams_grau::cmd(string str){
  return false;
}

} // Goblin
