#ifndef VOIGT_H
#define VOIGT_H

#include "Goblin/DllExport.h"

namespace Goblin
{

class GOBLIN_EXPORT Voigt{
  double c,ca,cb,cc,cd,ce,ulim,vf[50+1][26+1];
  int lastv;
  double *p,*bgg,xlast;
  double vt[50+1],ds[50+1],dm[50+1];
  double old_dmp;
public:
  void vstart(double dmp);
  double voigt(double uu,double wdth);
  Voigt();
  ~Voigt(){delete[] p; delete[] bgg;}
  double operator()(double h,double w,double wdth,double dmp);
};

} // Goblin

#endif
