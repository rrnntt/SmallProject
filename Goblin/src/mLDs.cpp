
#include "mLDs.h"
#include "humlicek.h"

multiparams_LDs::multiparams_LDs(splist& s):lineparams_iso(s){
  psize = 6;
  names.resize(psize);
  names[0] = "line";
  names[1] = "intens";
  names[2] = "self";
  names[3] = "shift";
  names[4] = "n_t";
  names[5] = "e0";
  w_ = &(s.getFieldNew<double>("d",names[0])->data);
  intens_ = &(s.getFieldNew<double>("d",names[1])->data);
  self_ = &(s.getFieldNew<double>("d",names[2])->data);
  shift_ = &(s.getFieldNew<double>("d",names[3])->data);
  nt_ = &(s.getFieldNew<double>("d",names[4])->data);
  e0_ = &(s.getFieldNew<double>("d",names[5])->data);
  set_default("shift",0.);
  set_default("self",0.4);
  set_default("nt",0.);
  set_default("e0",0.);
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
//    mio<<"mLD: T0="<<T0();
    while(istr>>buf){
      if (buf.empty()) break;
      i = buf.find(':');
      if (i == 0 || i == string::npos) {
        Iso = 0; i = 0;
      }else
        Iso = atoi(buf.substr(0,i).c_str());
      Z0(Iso,atof(buf.substr(i+1).c_str()));
//      mio<<"     Z0["<<Iso<<"]="<<Z0(Iso)<<'\n';
    };
  };
}


void multiparams_LDs::new_line(size_t i,double xx,double yy,spectrum *sp){
  set(i,"iso",def_iso);
  set_line(i,xx,sp);
  set_height(i,yy,sp);
  self(i) = def_self;
  shift(i) = def_shift;
  e0(i) = def_e0;
  nt(i) = def_nt;
  for(size_t j=0;j<size();j++)
    if (default_fix[j]) fix(i,j);
}

void multiparams_LDs::set_line(size_t i,double xx,spectrum *sp){
  if (sp){
    double P = sp->exp_cond.P()/101300.;
    w(i) = xx - def_shift*P;
  }else
    w(i) = xx;
}

void multiparams_LDs::set_height(size_t i,double xx,spectrum *sp){
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
     dl = 2*( def_self*Ps )*pow(T0()/T,def_nt);
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

string multiparams_LDs::print(size_t i){
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

  ostr <<' '<<nt(i);
  if (fixed(i,4)) ostr<<'*';

  ostr.precision(12);
  ostr <<' '<<e0(i);
  if (fixed(i,5)) ostr<<'*';

  if (iso_)
    ostr <<"iso "<<iso(i);

  return ostr.str();
}

void multiparams_LDs::set_default(string s,double val){
  if(s=="self"){
    def_self = val;
    return;
  };
  if(s=="shift"){
    def_shift = val;
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

double& multiparams_LDs::par(size_t i,size_t j){
  if (i>=nol()) return bad_par;
  switch(j){
    case 0: return w(i);
    case 1: return intens(i);
    case 2: return self(i);
    case 3: return shift(i);
    case 4: return nt(i);
    case 5: return e0(i);
  };
  return bad_par;
}


void multiparams_LDs::limit(size_t i){
}

cmd_res multiparams_LDs::cmd(string str){
  istringstream istr(str);
  string command;
  istr >> command;

  // sets
  if (command == "line_error"){
    if (!error()) error(true);
    vector<double>* lerrp = spl->getDoubleNew("lerr");
    if (!lerrp) {
//      mio<<"Cannot find field lerr\n";
      return false;
    };
    vector<double>& lerr = *lerrp;
    double a0,a1,w1=1.,w2=1.,a;
    istr>>a0>>a1>>w1>>w2;
    for(size_t i=0;i<nol();i++){
      a = a0 + a1/height(i);
//      mio<<a0<<' '<<a1<<' '<<height(i)<<'\n';
      if (lerr[i]>a) a = lerr[i];
//      if (a == 0.) mio<<"a("<<w(i)<<")="<<a<<'\n';
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
//      mio<<"error\n";
      return false;
    };
    if (!spl->spectra.size()){
//      mio<<"No spectrum attached\n";
      return false;
    };
    if (is >= spl->spectra.size()) {
//      mio<<"No spectrum "<<is<<'\n';
      return false;
    };

    if (!spl->spectra[is].exp_sp){
//      mio<<"error\n";
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

double multiparams_LDs::dp(size_t i,size_t j){// i- number of line, j- number of parameter
  switch(j){
    case 0: return 0.0001;
    case 1: return intens(i)?intens(i)/1000:1e-30;
    case 2: return shift(i)?shift(i)/1000:0.0001;
    case 3: return self(i)?self(i)/1000:0.0001;
    case 4: return nt(i)?nt(i)/1000:0.0001;
    case 5: return 0.0001;
  };
  return 0.;
}

double multiparams_LDs::EW(size_t i,spectrum *sp){
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


void multiparams_LDs::set(size_t i,string nam,double val){
  if (i >= nol()) return;
  else if (nam == "line") w(i) = val;
  else if (nam == "intens") intens(i) = val;
  else if (nam == "shift") shift(i) = val;
  else if (nam == "self") self(i) = val;
  else if (nam == "e0") e0(i) = val;
  else if (nam == "nt") nt(i) = val;
  else lineparams_iso::set(i,nam,val);
}



void multiparams_LDs::init(spectrum* sp){
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

void multiparams_LDs::init_line(size_t i,spectrum* sp){
  Ps = sp->exp_cond.P(iso(i))/101300.;
  double zz0 = Z0(iso(i));
  Z =  sp->exp_cond.Z(iso(i))/(zz0?zz0:1.);
  ww = w(i) + shift(i)*Ps;
//  ww = w(i) + shift(i)*P;
  dl = 2*( self(i)*Ps )*pow(T0()/T,nt(i));
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

double multiparams_LDs::value(size_t i,double wx){
  if (i<nol()) return hi*humlicek(sx*(wx-ww),y)/h0;
  else
    return 0.;
}



