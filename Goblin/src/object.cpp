#include "Goblin/object.h"
#include "Goblin/gdata.h"
#include "Goblin/mio.h"

#include <sstream>

namespace Goblin
{

cmd_res object::cmd(string str){
  cmd_res ok,bad(false);
  istringstream istr(str);
  string com,rest;
  istr>>com;
  rest = str.substr(str.find(com)+com.size());
  
/*  if (com == "save"){
    this->save(fname);
    return ok;
  };*/

  if (com=="save"){
    if (!this->save()) return bad;
    return ok;
  };
  
  if (com=="saveAs"){
    string fn;
    istr>>fn;
    if (!this->save(fn)) return bad;
    return ok;
  };
  
  if (com=="load"){
    string fn;
    istr>>fn;
    if (!this->load(fn)) return bad;
    return ok;
  };
  
  if (com == "wr"){
    write(true);
    return ok;
  };

  if (com == "ro"){
    write(false);
    return ok;
  };
  
  //if (com == "show"){
  //  string s;
  //  istr>>s;
  //  show(s);
  //  ok.repaint(true);
  //  return ok;
  //};
  //
  //if (com == "hide"){
  //  string s;
  //  istr>>s;
  //  hide(s);
  //  ok.repaint(true);
  //  return ok;
  //};
  
  //if (com == "next"){
  //  if (!owner){
  //    mio<<"object is not in gdata\n";
  //    return bad;
  //  };
  //  string focname = owner->plt.focname;
  //  if (!cname(focname)) return bad;
  //  gdata::obj_it o;
  //  vector<gdata::obj_it> ov;
  //  size_t i;
  //  for(o=owner->obj.begin();o!=owner->obj.end();o++)
  //    if ((**o).cname(focname)) {
  //      if ((**o).name == name && (**o).type == type) i = ov.size();
  //      ov.push_back(o);
  //    };
  //  if (ov.size()<2) return ok;
  //  if (i == ov.size()-1) o = ov[0];
  //  else
  //    o = ov[i+1];
  //  (**ov[i]).focused(false);
  //  (**o).focused(true);
  //  mio<<(**o).name<<' '<<(**o).type<<'\n';
  //  ok.repaint(true);
  //  return ok;
  //};
  //
  //if (com == "prev"){
  //  if (!owner){
  //    mio<<"object is not in gdata\n";
  //    return bad;
  //  };
  //  string focname = owner->plt.focname;
  //  if (!cname(focname)) return bad;
  //  gdata::obj_it o;
  //  vector<gdata::obj_it> ov;
  //  size_t i;
  //  for(o=owner->obj.begin();o!=owner->obj.end();o++)
  //    if ((**o).cname(focname)) {
  //      if ((**o).name == name && (**o).type == type) i = ov.size();
  //      ov.push_back(o);
  //    };
  //  if (ov.size()<2) return ok;
  //  if (i == 0) o = ov[ov.size()-1];
  //  else
  //    o = ov[i-1];
  //  (**ov[i]).focused(false);
  //  (**o).focused(true);
  //  ok.repaint(true);
  //  return ok;
  //};
  
  return bad;

};

//void object::show(string str){
//  cnames[str] = 1;
//  if (owner) owner->find_focus(str);
//}
//
//
//void object::hide(string str){
//  map<string,bool>::iterator b = cnames.find(str);
//  if (b != cnames.end()) cnames.erase(b);
//  if (owner) {
//    focused(false);
//    owner->find_focus(str);
//  };
//}

cmd_res object::send(string m,bool all){
  return owner?owner->message(this,m,all):cmd_res(false);
}

} // namespace Goblin
