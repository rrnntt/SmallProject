#ifndef MATRIX_H
#define MATRIX_H

#include "Goblin/DllExport.h"

#include <valarray>
#include <iostream>
#include <fstream>
//#include "../Troll1/mio.h"

namespace Goblin
{

using namespace std;

class matrix;
class jac_matrix;

class GOBLIN_EXPORT vec{
public:
  valarray<double> v;
  vec(){}
  vec(size_t n):v(n){}
  ~vec(){}
  size_t size()const{return v.size();}
  size_t dim()const{return v.size();}
  void resize(size_t n){v.resize(n);v=0.;}
  double& val(size_t i){return v[i];}
  void mul(matrix& m,vec& b);
  void mul(vec& a,matrix& m);
  double& operator[](size_t i){return v[i];}
  double& operator()(size_t i){return v[i];}
  double operator()(size_t i)const{return v[i];}
  vec& operator=(double x){v = x;return *this;}
};

class GOBLIN_EXPORT matr_object{
public:
  matrix *m;
  matr_object(matrix &mm):m(&mm){}
  virtual double operator()(size_t i,size_t j)const;
};

class GOBLIN_EXPORT matr_show:public matr_object{
public:
  double vmax,vmin;
  matr_show(matrix &mm,double ma=0.,double mi=0.);
};

class GOBLIN_EXPORT matrix{
  static double bad_value;
  valarray<double> v;
  size_t d1,d2;
public:
  matrix(){}
  matrix(size_t x,size_t y){make(x,y);}
  ~matrix(){}
  void make(size_t x,size_t y);
  void resize(size_t x,size_t y){make(x,y);}
  size_t size()const{return v.size();}
  size_t dim1()const{return d1;}
  size_t dim2()const{return d2;}

  double& val(size_t i,size_t j){return v[i*d2+j];}
  double val(size_t i,size_t j)const{return v[i*d2+j];}
  double& operator()(size_t i,size_t j){return v[i*d2+j];}
  double operator()(size_t i,size_t j)const{return v[i*d2+j];}
  double operator[](size_t i)const{return v[i];}
  void copy(matrix& m1){make(m1.dim1(),m1.dim2());v = m1.v;}
  void mul(matrix& m1,matrix& m2);
  void mul(matrix& m1);
  void zero(){v=0.;}
  void identity(size_t x);
  void read(string fn);
  bool solve(vec& x);
  bool ludcmp(valarray<size_t>& indx,double& d);
  bool lubksb(valarray<size_t>& indx,vec& b);
  void inverse();
  void svd(vec& w,matrix& v);
  void jacob2(size_t i,size_t j,matrix *V1=0,matrix *V2=0);
  // Householder elimination of column c and rows below r
  void hous_col(size_t c,size_t r,matrix &P);
  void hous_row(size_t r,size_t c,matrix &P);
  void qr_tall(matrix &R);
  void d3(matrix &R);
  void diag(vec &d);
  void transpose(matrix &a);
  double min(){return v.min();}
  double max(){return v.max();}
  double sum(){return v.sum();}
//  void transform(matrix &a);
};

class GOBLIN_EXPORT jac_matrix:public matrix{
  double& operator()(size_t i,size_t j);
  public:
  jac_matrix(){}
  jac_matrix(size_t nn,size_t ii,size_t jj);
  void make(size_t nn,size_t ii,size_t jj);
  size_t n,i,j;
//  size_t size()const{return n*n;}
  size_t dim1()const{return n;}
  size_t dim2()const{return n;}
  double operator()(size_t ii,size_t jj)const;
};

ostream& operator << (ostream& ostr,const matrix& m);
ostream& operator << (ostream& ostr,const vec& m);
ostream& operator << (ostream& ostr,const matr_show& m);

} // Goblin

#endif
