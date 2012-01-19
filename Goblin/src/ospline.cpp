#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "ospline.h"
#include "../Troll2/mio.h"

ospline::ospline(){
  fixed_=true;
  color(rgb(0,0,255));
  cur_color(rgb(255,0,0));
  in_state(in_normal);
}

ospline::~ospline(){
}

bool ospline::save(){
//  if (write()) return save(fname);
  return false;
}

cmd_res ospline::cmd(string str){
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

fun_res ospline::fun(string str){
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

bool ospline::draw(canvas& c){
  if (c.type == "graph"){
   graphics& g = *(graphics*)(&c);
     if (!v.size()) return false;
     double w,h,x,y;
     int xx,yy;

     g.pen(color());
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

     g.pen(cur_color());
     x = v[curr()].x;
     y = v[curr()].y;
     xx = g.X(x);
     yy = g.Y(y);
     g.drawRect(rect(xx-2,yy-2,5,5));

   return true;
  };
  return false;
}

cmd_res ospline::mouseClick(canvas* c,int x,int y,int shft){
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



