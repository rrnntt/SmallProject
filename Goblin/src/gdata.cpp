#include "Goblin/gdata.h"
#include "Goblin/utils.h"
#include "Goblin/mio.h"

#include <sstream>
#include <fstream>
#include <iostream>

namespace Goblin
{

using namespace std;

bool operator<(const object& o1,const object& o2){
  return o1.name < o2.name;
}

gdata::gdata(){
  createObject = 0;
#ifdef GRAPHICS
  plt.plo = this;
#endif
}

cmd_res gdata::cmd(string str){
  cmd_res ok;
  cmd_res bad(false);
  string str1 = str;
  string b,c;
  size_t i;
  
  if (str1.empty()) return bad;
  if (str1[0] == '#') return bad;
  i = str1.find("//");
  if (i != string::npos) str1.erase(i);
  while(!(b=brakets(str1)).empty()){
    if (b == "$ERROR$"){
      mio<<"Syntax error\n";
      return cmd_res(false);
    };
    c = fun(b)();
    str1.replace(str1.find('['),b.size()+2,c);
  };
  
  while((i=str1.find(';'))!=string::npos){
    b = str1.substr(0,i);
    cmd(b);
    str1.erase(0,i+1);
  };
  istringstream istr(str1);
  string com;
  istr >> com;
  if (com.empty()) return cmd_res(false);
  string rest = str1.substr(str1.find(com)+com.size());
  
  if (!command_obj.empty()){
    object* o = Obj(command_obj);
    if (!o) command_obj = "";
    else if (com[0]=='.'){
      com = command_obj+com;
    };
  };

  string who,itscom;
  size_t n;
  if ( complex_command(com,who,n,itscom)){
#ifdef GRAPHICS
    if (who == "plot"){
      return plt.cmd(itscom+" "+rest);
    };
#endif
    if (who == "obj" && n != string::npos && n < obj.size()){
      return obj[n]->cmd(itscom+" "+rest);
    };
    obj_it o ;
    string type;
    i = who.find(':');
    if (i != string::npos){
      who.replace(i,1," ");
      istringstream iss(who);
      string s;
      iss >> type >> s;
      who = s;
    };
    for(o=obj.begin();o!=obj.end();o++)
         if ((*o)->name==who) {
	          if (type.empty() || (*o)->type == type){
                 //mio<<type<<':'<<who<<' '<<com<<' '<<rest<<'\n';
                 return (*o)->cmd(itscom+" "+rest);
              };
         };
    return bad;
  };

  if (com == "del"){
    string s;
    istr>>s;
    //mio<<"delete "<<s<<'\n';
    delete_obj(s);
    ok.repaint(true);
    ok.iup(true);
    return ok;
  };

  if (com == "out"){
    mio<<rest<<'\n';
    return ok;
  };

  if (com == "iup"){
    ok.iup(true);
    return ok;
  };

  if (com == "info"){
    mio<<"Objects:\n";
    for(obj_it o=obj.begin();o!=obj.end();o++){
      mio<<(*o)->name;
      if ((**o).focused()) mio<<'*';
      mio<<' '<<(*o)->fname<<'\n';
    };
#ifdef GRAPHICS
    mio<<"Views:\n";
    for(plot::can_it c=plt.cans.begin();c!=plt.cans.end();c++){
      if (c->second){
        mio<<c->second->type<<'.'<<c->second->name;
        if (c->second->has_focus()) mio<<'*';
        mio<<' '<<(c->second->visible()?"visible":"hidden")<<'\n';
      };
    };
#endif
    return ok;
  };

  if (com == "eval"){
    return cmd(rest);
  };

  if (com == "load"){
    string what,fn,canv;
    istr>>what>>fn>>canv;
    object* o = loadObj(what,fn);
    //if (o && !canv.empty()) {
    //  o->show(canv);
    //  set_focus(o->type+":"+o->name,canv);
    //};
    ok.iup(true);
    ok.repaint(true);
    return ok;
  };

  if (com == "new"){
    string what,fn,canv;
    istr>>what>>fn>>canv;
    object* o = newObj(what,fn);
    if (!o) return bad;
    o->fname = dir()+fn+'.'+o->ext;
    //if (!canv.empty()) {
    //  o->show(canv);
    //  set_focus(o->type+":"+o->name,canv);
    //};
    ok.iup(true);
    ok.repaint(true);
    return ok;
  };

  if (com == "save"){
    for(obj_it o=obj.begin();o!=obj.end();o++){
      if ( (**o).write() ) (**o).cmd("save");
    };
    return ok;
  };

  if (com == "script"){
    string what,log;
    istr>>what>>log;
    ifstream ifil(what.c_str());
    if (!ifil) {
      mio<<"Cannot open file "<<what<<'\n';
      return bad;
    };
    //if (!log.empty()) {
    //  ofstream ofil(log.c_str());
    //  if (ofil) {
    //    mio.log = &ofil;
    //    while(getline(ifil,what))  cmd(what);
    //    mio.log = 0;
    //  }else
    //    while(getline(ifil,what))  cmd(what);
    //}else  
      while(getline(ifil,what))  cmd(what);
    
    ok.repaint(true);
    ok.iup(true);
   return ok;
  };
  
  if (com == "dir"){
    string sdir;
    istr>>sdir;
    dir(sdir);
    return ok;
  };
  
  if (com == "com"){
    istr>>command_obj;
    return ok;
  };

  //if (com == "focus"){
  //  string oname,vname;
  //  istr>>oname>>vname;
  //  if (vname.empty()) vname = plt.focname;
  //  set_focus(oname,vname);
  //  ok.iup(true);
  //  ok.repaint(true);
  //  return ok;
  //};
  
  return ok;
}

fun_res gdata::fun(string str){
  fun_res error("$ERROR$");
  fun_res nul;
  string str1 = str;
  string b,c;
  while(!(b=brakets(str1)).empty()){
    if (b == "$ERROR$"){
      return error;
    };
    c = fun(b)();
    str1.replace(str1.find('['),b.size()+2,c);
  };

  size_t i;
  istringstream istr(str1);
  string com;
  istr >> com;
  if (com.empty()) return nul;
  string arg = str1.substr(str1.find(com)+com.size());

  if (!command_obj.empty()){
    object* o = Obj(command_obj);
    if (!o) command_obj = "";
    else if (com[0]=='.'){
      com = command_obj+com;
    };
  };

  if ( (i = com.find('.')) != string::npos){
    string who;
    int n;
    com.replace(i,1," ");
    i = com.find('#');
    if (i != string::npos){
      com.replace(i,1," ");
      istringstream iss(com);
      iss >> who >> n >> com;
      if (who == "obj" && n < obj.size()) return obj[n]->fun(com+" "+arg);
    }else{
      istringstream iss(com);
      iss >> who >> com;
//#ifdef GRAPHICS
//      if (who == "plot"){
//        return plt.fun(com+" "+arg);
//      };
//#endif
      for(obj_it o=obj.begin();o!=obj.end();o++) if ((*o)->name==who) return (*o)->fun(com+" "+arg);
    };
  };

  ostringstream ostr;

  if (com == "tst") {
    int n,nn=0;
    istr>>n;
    for(int i=1;i<=n;i++) nn += i*i;
    ostr<<nn;
    return fun_res(ostr.str());
  };

  if (com == "no") {
    ostr<<obj.size();
    return fun_res(ostr.str());
  };

  if (com == "dir") {
    string fn;
    istr>>fn;
    return fun_res(dir()+fn);
  };

  if (com == "=") {
    string nam,rest;
    istr >> nam;
    rest = str1.substr(str1.find(nam)+nam.size());
    size_t i = rest.find_first_not_of(' ');
    if (i != 0 && i != string::npos) rest = rest.substr(i);
    vars[nam] = rest;
    return fun_res(rest);
  };

  if (com == ":=") {
    string nam,rest;
    istr >> nam;
    rest = str1.substr(str1.find(nam)+nam.size());
    size_t i = rest.find_first_not_of(' ');
    if (i != 0 && i != string::npos) rest = rest.substr(i);
    vars[nam] = rest;
    return fun_res();
  };

  if (com == "+") {
    double v1,v2;
    istr >> v1 >> v2;
    ostr.precision(12);
    ostr<<v1+v2;
    return (ostr.str());
  };

  if (com == "-") {
    double v1,v2;
    istr >> v1 >> v2;
    ostr.precision(12);
    ostr<<v1-v2;
    return (ostr.str());
  };

  if (com == "*") {
    double v1,v2;
    istr >> v1 >> v2;
    ostr.precision(12);
    ostr<<v1*v2;
    return (ostr.str());
  };

  if (com == "/") {
    double v1,v2;
    istr >> v1 >> v2;
    ostr.precision(12);
    ostr<<v1/v2;
    return (ostr.str());
  };

  if (com == "inv") {
    double v1;
    istr >> v1;
    ostr.precision(12);
    ostr<<1./v1;
    return (ostr.str());
  };

  if (com == "out"){
    mio<<arg<<'\n';
    return fun_res();
  };

  return vars[com];
}

//object* createObject(string type,string name);

object* gdata::newObj(string type,string name){
  if (!createObject){
    mio<<"No createObject function defined\n";
    return 0;
  };
  object* o = createObject(type,name);
  if (o) {
    obj.push_back(o);
    //o->owner = this;
    o->type = type;
    o->name = name;
  };

  return o;
}

object* gdata::loadObj(string type,string fname){
  string name;
  size_t i = fname.find_last_of('\\');
  if (i == string::npos) name = fname;
  else
     name = fname.substr(i+1);
  i = name.find('.');
  if (i != string::npos) name.erase(i);
  object* o = newObj(type,name);
  if (o == 0) {
    mio<<"Cannot create object "<<type<<'\n';
    return 0;
  };

  ifstream fil(fname.c_str());
  if (fil) fil.close();
  else{
    mio<<"Cannot find file "<<fname<<'\n';
    delete_obj(type+":"+name);
    return 0;
  };
  if (!o->load(fname)){
    mio<<"Error opening file "<<fname<<'\n';
    delete_obj(type+":"+name);
    return 0;
  };
  o->name = name;
  o->fname = fname;
  return o; 
}

//void gdata::set_focus(string oname,string vname){
//  object* ocurr = Obj(oname);
//  if (!ocurr) return;
//  if (!(*ocurr).cname(vname)) return;
//  if (ocurr) {
//    for(obj_it o=obj.begin();o!=obj.end();o++){
//      if ((**o).cname(vname)) (**o).focused(false);
//    };
//    ocurr->focused(true);
//    cmd("com "+oname);
//  };
//}

object* gdata::Obj(string str){
 string name,type;
 size_t i = str.find(':');
 if (i == string::npos) name = str;
 else{
   string s = str;
   s.replace(i,1," ");
   istringstream istr(s);
   istr>> type >> name;
 };
 for(obj_it o=obj.begin();o!=obj.end();o++) 
   if ( (**o).name == name && (type.empty() || type == (**o).type) ) return *o;
 return 0;
}

void gdata::dir(string str){
  dir_ = str;
  size_t i = dir_.find_last_not_of(' ');
  if (i+1 < dir_.size()) dir_.erase(i+1);
  if (dir_[i] != '\\') dir_ += '\\';
}

bool gdata::delete_obj(string str){
  object* o = Obj(str);
//  mio<<str<<' '<<(*o).name<<'\n';
  if (!o) return false;
  for(obj_it oo=obj.begin();oo!=obj.end();oo++)
    if ((**oo).type==(*o).type && (**oo).name==(*o).name){
//      mio<<(*o).name<<' '<<(**oo).name<<'\n';
      delete(o);
      obj.erase(oo);
//      mio<<"OK\n";
      break;
    };
}

cmd_res gdata::message(object* s,string m,bool all){
  cmd_res res;
  for(obj_it o=obj.begin();o!=obj.end();o++){
     res = (**o).message(s,m);
     if (res.ok && !all) return res;
  };
  return all?cmd_res():cmd_res(false);
}

// --------------- GRAPHICS stuff --------------------
#ifdef GRAPHICS
void gdata::draw(painter& pnt){
  pnt.brush(rgb(255,255,255));
  pnt.fillRect(rect(plt.x0(),plt.y0(),plt.width(),plt.height()));
  pnt.pen(rgb(1,1,1));
  pnt.drawRect(rect(plt.x0(),plt.y0(),plt.width(),plt.height()));

  plt.setLayout();
  plt.begin(pnt);
  canvas* c;
  object* ocurr;
  for(plot::can_it can=plt.cans.begin();can!=plt.cans.end();can++){
    c = can->second;
    if (c && c->visible()) {
      c->draw();
      ocurr = 0;
      for(obj_it o=obj.begin();o!=obj.end();o++){
//        mio<<(**o).type<<':'<<(**o).name<<' '<<(**o).cnames.size()<<'\n';
        if ( !(**o).cname(c->name) ) continue;
        if ((*o)->focused()) {
          ocurr = *o;
          continue;
        };
        if (c && c->visible()) {
          (*o)->draw(*c);
        };
      };
      if (ocurr != 0) ocurr->draw(*c);
    };
  };
}


cmd_res gdata::mouseClick(canvas& g,int x,int y,int shft){
  cmd_res bad(false),ok;
  object* oo;
  canvas* c;
  for(obj_it o=obj.begin();o!=obj.end();o++){
    if ( (**o).cname(plt.focname) && (**o).focused() ) {
      c = plt.cans[plt.focname];
      if (!c) return bad;
      ok = (**o).mouseClick(c,x,y,shft);
      if (ok.ok) {
        if (!(**o).focused()) set_focus((**o).name,plt.focname);
        return ok;
      };
    };
  };
  return bad;
}

cmd_res gdata::mouseDoubleClick(canvas& g,int x,int y,int shft){
  cmd_res bad(false),ok;
  object* oo;
  canvas* c;
  for(obj_it o=obj.begin();o!=obj.end();o++){
    if ( (**o).cname(plt.focname) && (**o).focused() ) {
      c = plt.cans[plt.focname];
      if (!c) return bad;
      return (**o).mouseDoubleClick(c,x,y,shft);
//      if (ok.ok) {
//        if (!(**o).focused()) set_focus((**o).name,plt.focname);
//        return ok;
//      };
    };
  };
  return bad;
}

cmd_res gdata::keyPress(keyboard key,int shft){
  object* oo;
  canvas* c;
  char sh = '0';
  if (shft == 1) sh = 's';
  else if (shft == 2) sh = 'c';
  else if (shft == 3) sh = 'b';
  cmd_res bad(false),res;
  for(obj_it o=obj.begin();o!=obj.end();o++){
    if ( (**o).cname(plt.focname) && (**o).focused()) {
      c = plt.cans[plt.focname];
      if (!c) break;
      res = (**o).cmd("keypress "+strInt(key)+' '+c->in_type+' '+sh);
      if (res.ok) return res;
    };
  };
  if (key == key_QuoteLeft){
    if (sh == 'c') return cmd("[qcom]");
  };
  if (key == key_Space){
    bool ok=false;
    string str = mio.in(&ok);
    return ok?cmd(str):bad;
  };
  return bad;
}


object* gdata::focused(){
  if (!obj.size()) return 0;
  for(obj_it o=obj.begin();o!=obj.end();o++)
    if ( (**o).focused() && (**o).cname(plt.focname) ) return *o;
  return 0;
}

void gdata::find_focus(string vname){
  if (!obj.size()) return;
  for(obj_it o=obj.begin();o!=obj.end();o++)
    if ( (**o).cname(plt.focname) && (**o).focused() ) {
      cmd("com "+(**o).type+":"+(**o).name);
      return;
    };

  for(obj_it o=obj.begin();o!=obj.end();o++)
    if ( (**o).cname(plt.focname) ) {
       set_focus((**o).name,vname);
       return;
    };
}

#endif

} // Goblin
