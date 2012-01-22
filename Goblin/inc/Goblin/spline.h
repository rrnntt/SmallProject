#ifndef SPLINE_H
#define SPLINE_H

#include "Goblin/DllExport.h"

#include "Goblin/object.h"
//#include "Goblin/graphics.h"
#include "Goblin/utils.h"

#include <vector>
#include <string>

#define BADVALUE  0.9898e30

namespace Goblin
{

class GOBLIN_EXPORT spoint{
public:
  double x,y;
  double d;
  double d2,d3;
  bool smooth,fixx,fixy,fixd;
  spoint():x(0),y(0),d(0),d2(0),d3(0),smooth(true),fixx(true),fixy(false),fixd(false){}
  spoint(double xx,double yy,double dd):x(xx),y(yy),d(dd),d2(0),d3(0),smooth(true),fixx(true),fixy(false),fixd(false){}
};

bool operator<(const spoint&,const spoint&);

class GOBLIN_EXPORT spline:public object{
  size_t curr_;
  bool fixed_;
public:
  enum input_state {in_normal,in_insert};
  input_state in_state_;
  input_state in_state(){return in_state_;}
  void in_state(input_state st){in_state_ = st;}
  vector<spoint> v;
  spline();
  ~spline();
  int n(){return v.size()-1;}// number of line segments
  int size(){return v.size();}// number of points
  void insert(double xx,double yy,double d);
  void refit(vector<spoint>::iterator j);
  void refit();
  double val(double x);
  double der(double x);
  double der2(double x);
  double operator()(double x){return val(x);}
  size_t curr(){return curr_;}
  void curr(size_t i){if (i<v.size()) curr_=i;}
  void next(){if (v.size() && curr_<v.size()-1) curr_++;} 
  void prev(){if (curr_>0) curr_--;}
  void refit(int i);
  void moveX(double dx);
  void moveY(double dy);
  void X(double xx);
  void Y(double yy);
  void D(double yy);
  void XY(double xx,double yy);
  void XYD(double xx,double yy,double dd);
  void X(size_t i,double xx);
  void Y(size_t i,double yy);
  void D(size_t i,double yy);
  void XY(size_t i,double xx,double yy);
  void XYD(size_t i,double xx,double yy,double dd);
  void changeD(double dd);// adds dd to the first derivative of the current point
  void clear();
//  void draw(graphics& g,rgb color = rgb(1,0,0));
//  void loadFromFile(const string fn);
//  void saveToFile(const string fn);
  void erase(size_t);
  double& currX(){return v[curr()].x;}
  double& currY(){return v[curr()].y;}
  double& currD(){return v[curr()].d;}
  double& par(size_t i,size_t j);
  double dp(size_t j);
  bool fixed(){return fixed_;}
  void fix(){fixed_=true;}
  void unfix(){fixed_=false;}
  bool fixed(size_t i,size_t j);
  void fix(size_t i,size_t j);
  void unfix(size_t i,size_t j);
  size_t psize(){return 3;}
  void copy(spline& spl);
//-------------------------------------------------------
  //rgb color,cur_color;
  bool load(string fn);
  bool save(string fn);
  bool save();
  cmd_res cmd(string str);
  fun_res fun(string str);
//  bool draw(canvas& c);
//  cmd_res mouseClick(canvas* c,int x,int y,int shft=0);
//  cmd_res mouseDoubleClick(canvas* c,int x,int y,int shft=0){return cmd_res(false);}
};

} // Goblin

#endif
