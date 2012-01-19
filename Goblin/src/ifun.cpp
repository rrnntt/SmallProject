#include <sstream>
#include "ifun.h"


ifun *new_ifun(string str){
  ifun *iff = 0;
  istringstream istr(str);
  string nam;
  istr >> nam;
  if (nam.empty()) return 0;
  if (nam == "box"){
    double w;
    string L;
    istr >> w>>L;
    if (L.empty()) iff = new boxcar(w);
    else
     iff = new boxcar(w,atof(L.c_str()));
    if (iff == 0) mio<<"bad_alloc\n";
    return iff;
  };
  mio<<"Cannot recognize afun "<<nam<<'\n';
/*  if (nam == "cub_filter"){
    double w1,w2;
    istr >> w1 >> w2;
    iff = new cub_filter(w1,w2);
    return iff;
  };*/
  return 0;
}

float boxcar::tr(double x){
  static float pi = 2*acos(0.);
  if (L() && fabs(x) > L()) return 0.;
  float a = 2*pi*wdt_*x;
//  return x?2*wdt_*sin(a)/(a):1.;
  return a?sin(a)/(a):1.;
}

string boxcar::input(){
  ostringstream ostr;
  ostr << "box " << wdt_;
  if (L()) ostr<<' '<<L();
  return ostr.str();
}

/*
void cub_filter::make(){
  s.clear();
  s.insert(wdt1_,1.,0.);
  s.insert(wdt2_,0.,0.);
  s.refit();
}

float cub_filter::tr(double x){
  double xx = fabs(x);
  return xx>wdt1_?s(xx):1.;
}

string cub_filter::input(){
  ostringstream ostr;
  ostr << "cub_filter " << wdt1_<<' '<<wdt2_;
  return ostr.str();
}

*/
