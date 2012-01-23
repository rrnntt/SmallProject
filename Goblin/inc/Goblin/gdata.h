#ifndef GDATA_H
#define GDATA_H

#include "Goblin/DllExport.h"
#include "Goblin/cmd.h"
#include "Goblin/object.h"

#include <string>
#include <map>
#include <vector>

namespace Goblin
{

using namespace std;

class GOBLIN_EXPORT gdata
{
public:
  gdata();
  ~gdata(){}
  map<string,string> vars;
  vector<object*> obj;
  string command_obj;
  typedef vector<object*>::iterator obj_it;
  object* Obj(string str);
  string dir_;
  string& dir(){return dir_;}
  void dir(string str);
  cmd_res cmd(string);
  fun_res fun(string);
  cmd_res message(object* s,string m,bool all=false);
  object* newObj(string type,string name);
  object* loadObj(string type,string fname);
  object* (*createObject)(string type,string name);
  bool delete_obj(string);
  void (*update_interface)(void);
  void interface_update(){if(update_interface)update_interface();}
//#ifdef GRAPHICS
//  plot plt;
//  void draw(painter&);
//  cmd_res mouseClick(canvas& g,int x,int y,int shft=0);
//  cmd_res mouseDoubleClick(canvas& g,int x,int y,int shft=0);
//  cmd_res keyPress(keyboard,int);
//  void set_focus(string oname,string vname);
//  void find_focus(string vname);
//  object* focused();
//#endif
};


template <class T>
class obj_list{
public:
  vector<T*> data;
  object* o;
  string type;
  obj_list(gdata* gd,string ttype){
    type = ttype;
    for(size_t i=0;i<gd->obj.size();i++){
      o = gd->obj[i];
      if (o->type == type) data.push_back((T*)(o));
    };
  }
  size_t size(){return data.size();}
  T& operator[](size_t i){return *data[i];}
  T* operator()(size_t i){return data[i];}
  T* operator()(string nam){
    size_t i = nam.find(':');
    string nnam;
    if (i == string::npos) nnam = nam;
    else{
      string typ = nam.substr(0,i);
      nnam = nam.substr(i+1);
//      mio<<typ<<' '<<nnam<<'\n';
      if (typ != type) return 0;
    };
    for(size_t i=0;i<size();i++)
    if (data[i]->name == nnam) return data[i];
    return 0;
  }
  bool has(T* t){
     for(size_t i=0;i<data.size();i++) if (data[i]==t) return true;
     return false;
  }
};

  template<class T>
  T* newObject(string type,string name,string ext){
    object* o = (object*) new T;
    if (!o){
      mio<<"Error creating "<<type<<'\n';
      return 0;
    };
    o->ext = ext;
  };

  template<class T>
  T* newCanvas(string type,string out_type,string in_type,string name){
    table* g = new table;
    g->name = name;
    c = (canvas*)g;
  };

} // Goblin
  
#endif
