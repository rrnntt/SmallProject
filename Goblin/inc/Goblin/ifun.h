#ifndef IFUN_H
#define IFUN_H

//#include "spline.h"

#include <math.h>
#include <stdlib.h>
#include <string>

namespace Goblin
{

/*
class ifun {
public: 
	ifun(){}
	virtual ~ifun(){}
	virtual float val(double x){return 1.;} // to use in frequency domain
	virtual float tr(double x){return 1.;}  // to use in Fourier (time) domain
	virtual float operator()(double x){return val(x);}
	virtual string input(){return "dlt";}
};
*/


class boxcar{
  double wdt_,L_;
public:
  boxcar():wdt_(0.0000001),L_(){}
  boxcar(double w):wdt_(w){}
  boxcar(double w,double l):wdt_(w),L_(l){}
  double width(){return wdt_;} // half-width of the box
  double L(){return L_;}
  float val(double x){return abs(x)<=wdt_?1./wdt_:0.;}
  float tr(double x);
  float operator()(double x){return val(x);}
  string input();
};

typedef  boxcar ifun;
/*
class cub_filter:public ifun{
  double wdt1_,wdt2_;
  spline s;
public:
  cub_filter():wdt1_(0.5),wdt2_(1.){make();}
  cub_filter(double w1,double w2):wdt1_(w1),wdt2_(w2){make();}
  double width1(){return wdt1_;}
  double width2(){return wdt2_;}
  void make();
  float val(double x){return 1.;}
  float tr(double x);
  float operator()(double x){return val(x);}
  string input();
};
*/
ifun *new_ifun(string str);

} // Goblin

#endif
