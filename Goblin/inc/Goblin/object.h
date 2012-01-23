#ifndef GOBLIN_OBJECT_H
#define GOBLIN_OBJECT_H

#include "Goblin/DllExport.h"
#include "Goblin/cmd.h"

#include <string>
#include <map>
#include <vector>

namespace Goblin
{

class gdata;

using namespace std;

class GOBLIN_EXPORT object{
  bool focused_;
public:
  gdata* owner;
  string name;
  string type;
  map<string,bool> cnames;
  string fname,ext;
  bool wr_;
  object():focused_(),wr_(false){ext="txt";}
  bool focused(){return focused_;}
  void focused(bool c){focused_ = c;}
  bool write(){return wr_;}
  void write(bool wr){wr_ = wr;}
  bool cname(string str){return cnames.find(str) != cnames.end();}
  cmd_res send(string m,bool all=false);
  bool save(){if (write()) return this->save(fname);return false;}
  virtual cmd_res cmd(string);
  virtual fun_res fun(string){return fun_res();}
  virtual bool load(string fn){return false;}
  virtual bool save(string fn){return false;}
  virtual cmd_res message(object* s,string m){return cmd_res(false);}

//#ifdef GRAPHICS
//  void show(string str);
//  void hide(string str);
//  virtual bool draw(canvas&){return true;}
//  virtual cmd_res mouseClick(canvas* c,int x,int y,int shft=0){return cmd_res(false);}
//  virtual cmd_res mouseDoubleClick(canvas* c,int x,int y,int shft=0){return cmd_res(false);}
//  virtual cmd_res keyPress(keyboard,int){return cmd_res(false);}
//#endif
};

bool operator<(const object& o1,const object& o2);



} // namespace Goblin

#endif // GOBLIN_OBJECT_H
