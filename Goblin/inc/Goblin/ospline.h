#ifndef OSPLINE_H
#define OSPLINE_H

#include "Goblin/DllExport.h"

#include <vector>
#include <string>
#include "../Troll2/object.h"
#include "../Troll2/graphics.h"
#include "../Troll2/utils.h"
#include "../spline/spline.h"

namespace Goblin
{

using namespace std;

class GOBLIN_EXPORT ospline:public object,public spline{
protected:
  enum input_state {in_normal,in_insert};
  input_state in_state_;
  rgb color_,cur_color_;
public:
  ospline();
  ~ospline();
  void in_state(input_state st){in_state_ = st;}
  input_state in_state(){return in_state_;}
  rgb color(){return color_;}
  void color(rgb c){color_=c;}
  rgb cur_color(){return cur_color_;}
  void cur_color(rgb c){cur_color_=c;}
  bool save();
  virtual bool save(string fn){return spline::save(fn);}
  virtual bool load(string fn){return spline::load(fn);}
  cmd_res cmd(string str);
  fun_res fun(string str);
  bool draw(canvas& c);
  cmd_res mouseClick(canvas* c,int x,int y,int shft=0);
//  cmd_res mouseDoubleClick(canvas* c,int x,int y,int shft=0){return cmd_res(false);}
};

} // Goblin

#endif

