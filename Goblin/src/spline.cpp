#include "Goblin/spline.h"
#include "Goblin/mio.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>

namespace Goblin
{

bool operator<(const spoint& s1,const spoint& s2){
  return s1.x < s2.x;
}

spline::spline():fixed_(true){
  curr_=0;
  color=rgb(0,0,255);
  cur_color=rgb(255,0,0);
  in_state(in_normal);
}

spline::~spline(){
}

void spline::insert(double xx,double yy,double d){
  spoint p(xx,yy,d);
  if (n() < 0) {
    v.push_back(p);
    return;
  };
  vector<spoint>::iterator i,j=v.end(),k;
  size_t jj=0;

  if (xx > v.back().x) {
    v.push_back(p);
    j = v.end();j--;
    jj = n();
  }else
  for(i=v.begin(),jj=0;i<v.end();i++,jj++){
    if (xx < i->x){
      v.insert(i,p);
      for(k=v.begin();k<v.end();k++) if (k->x == xx) j = k;
      break;
    };
    if (xx == i->x){
      j = i;
      break;
    };
  };
  refit(j);
  curr(jj);
}

void spline::refit(vector<spoint>::iterator jj){
  vector<spoint>::iterator i,j=jj;
  double dx;
  if (j != v.end()-1){
    i = j+1;
    dx = i->x - j->x;
    j->d2 = (3*(i->y - j->y - j->d*dx) - (i->d - j->d)*dx)/(dx*dx);
    j->d3 = ((i->d - j->d)*dx - 2*(i->y - j->y - j->d*dx) )/(dx*dx*dx);
  };
  if (j != v.begin()){
    j--;
    i = j+1;
    dx = i->x - j->x;
    j->d2 = (3*(i->y - j->y - j->d*dx) - (i->d - j->d)*dx)/(dx*dx);
    j->d3 = ((i->d - j->d)*dx - 2*(i->y - j->y - j->d*dx) )/(dx*dx*dx);
  };
}

double spline::val(double x){
  double dx;
  vector<spoint>::iterator i;
  if (v.size() == 1) return 0.;
  for(i=v.begin();i<v.end()-1;i++)
    if (x >= i->x && x < (i+1)->x) {
      dx = x - i->x;
      return i->y + dx*(i->d + dx*(i->d2 + dx*i->d3));
    };
  return 0.;
}

double spline::der(double x){
  double dx;
  vector<spoint>::iterator i;
  for(i=v.begin();i<v.end()-1;i++)
    if (x >= i->x && x < (i+1)->x) {
      dx = x - i->x;
      return i->d + dx*(i->d2*2 + dx*i->d3*3);
    };
  return 0.;
}

double spline::der2(double x){
  double dx;
  vector<spoint>::iterator i;
  for(i=v.begin();i<v.end()-1;i++)
    if (x >= i->x && x < (i+1)->x) {
      dx = x - i->x;
      return i->d2*2 + dx*i->d3*6;
    };
  return 0.;
}

void spline::moveX(double dx){
  v[curr_].x += dx;
  refit(curr_);
}

void spline::moveY(double dy){
  v[curr_].y += dy;
  refit(curr_);
}

void spline::X(double xx){
  v[curr_].x = xx;
  refit(curr_);
}

void spline::Y(double yy){
  v[curr_].y = yy;
  refit(curr_);
}

void spline::D(double dd){
  v[curr_].d = dd;
  refit(curr_);
}

void spline::XY(double xx,double yy){
  v[curr_].x = xx;
  v[curr_].y = yy;
  refit(curr_);
}

void spline::XYD(double xx,double yy,double dd){
  v[curr_].x = xx;
  v[curr_].y = yy;
  v[curr_].d = dd;
  refit(curr_);
}

void spline::X(size_t i,double xx){
  if (i >= v.size()) return;
  v[i].x = xx;
  refit(i);
}

void spline::Y(size_t i,double yy){
  if (i >= v.size()) return;
  v[i].y = yy;
  refit(i);
}

void spline::D(size_t i,double dd){
  if (i >= v.size()) return;
  v[i].d = dd;
  refit(i);
}

void spline::XY(size_t i,double xx,double yy){
  if (i >= v.size()) return;
  v[i].x = xx;
  v[i].y = yy;
  refit(i);
}

void spline::XYD(size_t i,double xx,double yy,double dd){
  if (i >= v.size()) return;
  v[i].x = xx;
  v[i].y = yy;
  v[i].d = dd;
  refit(i);
}

void spline::refit(int i){
  vector<spoint>::iterator j;
  int k=0;
  for(j=v.begin();j!=v.end();j++,k++)
    if (k == i) {
      refit(j);
      return;
    };
}

void spline::refit(){
  vector<spoint>::iterator j;
  sort(v.begin(),v.end());
  for(j=v.begin();j!=v.end();j++)  refit(j);
}

void spline::changeD(double dd){
  v[curr_].d += dd;
  refit(curr_);
}

bool spline::load(string fn){
  ifstream from(fn.c_str());
  if (!from) return false;
  clear();
  double x_,y_,d_,d2_,d3_;
  while((from>>x_>>y_>>d_>>d2_>>d3_)){
    insert(x_,y_,d_);
  };
  from.close();
  return true;
}

bool spline::save(string fn){
  ofstream to(fn.c_str());
  to.precision(14);
  for(size_t i=0;i<v.size();i++)
    to<<v[i].x<<' '<<v[i].y<<' '<<v[i].d<<' '<<v[i].d2<<' '<<v[i].d3<<endl;
  to.close();
  return true;
}


void spline::erase(size_t j){
  vector<spoint>::iterator i;
  size_t k=0;
  for(i=v.begin();i<v.end()-1;i++,k++)
    if (k == j){
      v.erase(i);
      refit(k-1);
      break;
    };
}

double& spline::par(size_t i,size_t j){
  if (i > n()) return v[n()].d3;
  switch (j){
    case 0: return v[i].x;
    case 1: return v[i].y;
    case 2: return v[i].d;
  };
}

double spline::dp(size_t j){
  switch (j){
    case 0: return 0.0001;
    case 1: return 0.0001;
    case 2: return 0.0001;
  };
}

bool spline::fixed(size_t i,size_t j){
  if (i > n()) return true;
  switch (j){
    case 0: return v[i].fixx;
    case 1: return v[i].fixy;
    case 2: return v[i].fixd;
  };
}

void spline::fix(size_t i,size_t j){
  if (i > n()) return;
  switch (j){
    case 0: v[i].fixx = true;
             return;
    case 1: v[i].fixy = true;
             return;
    case 2: v[i].fixd = true;
             return;
  };
}

void spline::unfix(size_t i,size_t j){
  if (i > n()) return;
  switch (j){
    case 0: v[i].fixx = false;
             return;
    case 1: v[i].fixy = false;
             return;
    case 2: v[i].fixd = false;
             return;
  };
}


void spline::copy(spline& spl){
	curr_ = spl.curr_;
  fixed_ = spl.fixed_;
  v.resize(spl.v.size());
  v = spl.v;
//  fname;
}


bool spline::save(){
  if (write()) return save(fname);
  return false;
}

cmd_res spline::cmd(string str){
  cmd_res ok,bad(false),repaint;
  repaint.repaint(true);
  istringstream istr(str);
  string com;
  istr>>com;
  string rest;
  rest = str.substr(str.find(com)+com.size());

  size_t i = com.find('.');
  if (i != string::npos){
    com[i] = ' ';
    istringstream is(com);
    string who,itscom;
    is>>who>>itscom;
//    if (who == "lpar") return lparams().cmd(itscom+' '+rest);
    return bad;
  };

  if (com=="info"){
    mio<<"spline "<<name<<'\n';
  };
  
  if (com=="harm"){
    double a,a2,h,c=v[n()].x;
    istr>>a>>h;
    if (a<=0) return bad;
    a2 = a/2;
    for(double b=v[0].x+a;b<c;b+=a){
      insert(b,h,0.);
      insert(b-a2,0.,0.);
    };
    return repaint;
  };
  
  if (com=="xy"){
    double a,b;
    istr>>a>>b;
    XY(a,b);
    return repaint;
  };
  
  if (com=="d"){
    double a;
    istr>>a;
    D(a);
    return repaint;
  };
  
  if (com=="wr"){
    write(true);
    return ok;
  };
  
  if (com=="rd"){
    write(false);
    return ok;
  };
  
  if (com=="save"){
    if (!save()) return bad;
    return ok;
  };
  
  if (com=="saveAs"){
    string fn;
    istr>>fn;
    if (!save(fn)) return bad;
    return ok;
  };
  
  if (com=="load"){
    string fn;
    istr>>fn;
    if (!load(fn)) return bad;
    return ok;
  };
  
  if (com=="mins"){//  mouse insert
    in_state(in_insert);
    return repaint;
  };
  
  if (com=="keypress"){
    int key;
    string in_type;
    char sh;
    istr>>key>>in_type>>sh;
//    if (in_type == "in"){
      switch (key){
        case key_Insert: in_state(in_insert); return ok;
        case key_Delete: {erase(curr()); return repaint;};
      };
      return bad;
//    };
    return bad;
  };
  

  return object::cmd(str);
}

fun_res spline::fun(string str){
  string s = str;
  string b,c;
  while( !(b = brakets(s)).empty() ){
    if (b == "$ERROR$") return b;
    c = fun(b)();
    s.replace(s.find('['),b.size()+2,c);
  };
  istringstream istr(s);
  ostringstream ostr;
  string nam;
  istr >> nam;
  if (nam.empty()) return fun_res();
  
  if (nam == "size"){
    ostr << 0;
    return ostr.str();
  };
  
  return object::fun(str);			      
}

void spline::clear(){
  if (size()<2) return;
  double x0=v[0].x;
  double x1=v[n()].x;
  v.clear();
  insert(x0,0,0);
  insert(x1,0,0);
}

bool spline::draw(canvas& c){
  if (c.type == "graph"){
   graphics& g = *(graphics*)(&c);
     if (!v.size()) return false;
     double w,h,x,y;
     int xx,yy;
     g.pen(color);
     g.moveTo(0,g.Y(val(g.uMin())));
     for(int j=0;j<g.width();j++){
       w = g.U(j);
       h = val(w);
       yy = g.Y(h);
       g.lineTo(j,yy);
     };
     if (v[curr()].x < g.uMin())
       for(size_t i=curr();i<v.size();i++)
        if (v[i].x > g.uMin()) {
           curr(i);
           break;
       };

     if (v[curr()].x > g.uMax()){
       for(size_t i=curr();i>0;i--)
        if (v[i].x < g.uMax()) {
           curr(i);
           break;
       };
       if (v[curr()].x > g.uMax()) curr(0);
     };

     for(size_t i=0;i<v.size();i++){
       x = v[i].x;
       y = v[i].y;
       xx = g.X(x);
       yy = g.Y(y);
       if (x > g.uMin() && x < g.uMax() && y > g.vMin() && y < g.vMax())
         g.drawRect(rect(xx-2,yy-2,5,5));
     };

     g.pen(cur_color);
     x = v[curr()].x;
     y = v[curr()].y;
     xx = g.X(x);
     yy = g.Y(y);
     g.drawRect(rect(xx-2,yy-2,5,5));

   return true;
  };
  return false;
}

cmd_res spline::mouseClick(canvas* c,int x,int y,int shft){
  cmd_res ok,bad(false),repaint;
  repaint.repaint(true);
  if (c->type != "graph") return bad;
  graphics& g = *(graphics*)c;
  double rx = g.U(x);
  double ry = g.V(y);
  if (in_state() == in_insert) {
    in_state(in_normal);
    insert(rx,ry,der(rx));
    return repaint;
  };
  if (shft == 1){//  SHIFT
    XY(rx,ry);
    return repaint;
  };
  if (shft == 2){//  CTRL
    D((ry-currY())/(rx-currX()));
    return repaint;
  };
  int xx,yy,mx,my;
  mx = g.X(rx);
  my = g.Y(ry);
  for(size_t i=0;i<v.size();i++){
     xx = g.X(v[i].x);
     yy = g.Y(v[i].y);
     if (abs(xx-mx) < 3 && abs(yy-my) < 3) {
       curr(i);
       return repaint;
     };
  };
  
  return ok;
}

} // Goblin


