#ifndef CMD_H
#define CMD_H

#include <string>

namespace Goblin
{

using namespace std;

class cmd_res{
public:
  bool ok,repaint_,iup_;
  size_t i;
  int n;
  double d;
  void* p;
  cmd_res():ok(true),repaint_(false),iup_(false),i(),n(),d(),p(){}// ?????
  cmd_res(bool OK):ok(OK),repaint_(false),iup_(false),i(),n(),d(),p(){}// ?????
  bool repaint(){return repaint_;}
  void repaint(bool r){repaint_=r;}
  bool iup(){return iup_;}//  interface update
  void iup(bool r){iup_=r;}
};

class fun_res{
public:
  string res;
  void* p;
  fun_res():res(),p(){}
  fun_res(string str):res(str),p(){}
  string& operator()(){return res;}
};

} // Goblin

#endif
