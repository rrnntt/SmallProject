#ifndef _H
#define _H

#include "Goblin/DllExport.h"

#include "../Troll1/object.h"
#include <string>
#include <vector>

namespace Goblin
{

using namespace std;

class GOBLIN_EXPORT text: public object{
public:
  size_t ifirst,ipage,cx,cy,cwidth,cheight;
  size_t sx0,sy0,sx1,sy1;
  enum sel_type{none,rows,cols,words};
  sel_type sel;
  int g_height;
  vector<string> txt;
  text();
  bool load(string fn);
  bool save(string fn);
  bool save();
  cmd_res cmd(string str);
  fun_res fun(string str);
  bool draw(canvas& c);
  cmd_res mouseClick(canvas* c,int x,int y,int shft=0);
  cmd_res mouseDoubleClick(canvas* c,int x,int y,int shft=0){return cmd_res(false);}
  //------------------------------------------
};

} // Goblin

#endif
