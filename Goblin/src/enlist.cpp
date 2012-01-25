#include "Goblin/enlist.h"
#include "Goblin/mio.h"
#include "Goblin/gdata.h"
#include "Goblin/utils.h"

#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include <map>

namespace Goblin
{

using namespace std;

class comp_symm{
  enlist* en;
public:
  comp_symm(enlist& e):en(&e){}
  int operator()(const size_t i1,const size_t i2){
    VJKG& q1 = en->q(i1);
    VJKG& q2 = en->q(i2);
    if (q1.j < q2.j) return true;
    if (q1.j == q2.j) {
      if (q1.symm() < q2.symm()) return true;
      if (q1.symm() == q2.symm()) return (en->ener(i1) < en->ener(i2));
    };
    return false;
  }
};

class comp_q{
  enlist* en;
public:
  comp_q(enlist& e):en(&e){}
  int operator()(const size_t i1,const size_t i2){
    VJKG& q1 = en->q(i1);
    VJKG& q2 = en->q(i2);
    return q1 < q2;
  }
};

enlist::enlist(){
  //ener_p = &(getFieldNew<double>("d","ener")->data);
  ener_p = getDouble("ener");
  //q_p = &(getFieldNew<VJKG>("vjkg","q")->data);
  q_p = getColumn("q");
}
enlist::~enlist(){
}

size_t enlist::add(VJKG& qq,double ee){
  size_t i = appendRow();
  ener(i) = ee;
  q(i) = qq;
  return i;
}

double& enlist::ener(VJKG& qq){
  for(size_t i=0;i<size();i++)
    if ( q(i) == qq) return ener(i);
  return bad_double;
}

size_t enlist::index(VJKG& qq){
  for(size_t i=0;i<size();i++)
    if ( q(i) == qq) return i;
  return size();
}

size_t enlist::index(const VJKG& qq){
  for(size_t i=0;i<size();i++)
    if ( q(i) == qq) return i;
  return size();
}

//void enlist::sort(){
//}

bool enlist::load(string fn){
cerr<<fn<<'\n';
//  deleteFields();
  spbase::load(fn);
//  apply_params();
  q_p = getColumn("q");
  ener_p = getDoubleNew("ener");
  return true;
}

cmd_res enlist::cmd(string str){
  cmd_res ok(true),bad(false),repaint;
  repaint.repaint(true);
  istringstream istr(str);
  string com;
  istr>>com;
  string rest;
  rest = str.substr(str.find(com)+com.size());

  if (com=="table"){
    string en_name,fn;
    istr>>en_name;
    fn = rest.substr(rest.find(en_name)+en_name.size());
    if (fn.empty()){
      mio<<"Must specify the filename for the table\n";
      return bad;
    };
    if (en_name == object::name){
      mio<<"Comparing the same list\n";
      return bad;
    };
    object* o = owner->Obj(en_name);
    if (!o) {
      mio<<"No energies "<<en_name<<'\n';
      return bad;
    };
    table(*(enlist*)o,fn);
    return ok;
  
  };
  
  if (com=="flt"){
    string par;
    istr>>par;
    if (par == "iso"){
       int iso;
       char op =' ';
       istr >> iso >> op;
       if (!flt()) flt_make();
       for(size_t i=0;i<size();i++) flt_add(i,q(i).iso() == iso,op);
       return repaint;
    }else if (par == "j"){
       int iso,j;
       char op =' ';
       istr >> iso >> j >> op;
       if (!flt()) flt_make();
       for(size_t i=0;i<size();i++) flt_add(i,q(i).iso() == iso && q(i).j == j,op);
       return repaint;
    }else if (par == "vv"){ // v must be in the form 170 or 0_10_0
       int iso;
       char op =' ';
       VJKG qq;
       string v;
       istr >> iso >> v >> op;
       for (size_t i=0;i<v.size();i++) if (v[i]=='_') v[i] = ' ';
       v = '('+v+')';
       qq.iso(iso);
       qq.v = v;
       if (!flt()) flt_make();
       for(size_t i=0;i<size();i++) flt_add(i,(q(i).iso() == iso && q(i).v == qq.v),op);
       return repaint;
    }else if (par == "vj"){ // v must be in the form 170 or 0_10_0
       int iso;
       VJKG qq;
       string v;
       istr >> iso >> v >> qq.j;
       for (size_t i=0;i<v.size();i++) if (v[i]=='_') v[i] = ' ';
       v = '('+v+')';
       qq.iso(iso);
       qq.v = v;
       if (!flt()) flt_make();
       for(size_t i=0;i<size();i++) 
         flt_add(i,(q(i).iso() == iso && q(i).v == qq.v && q(i).j == qq.j));
       return repaint;
    }else if (par == "vk"){ // v must be in the form 170 or 0_10_0
       int iso;
       VJKG qq;
       string v;
       istr >> iso >> v >> qq.k;
       for (size_t i=0;i<v.size();i++) if (v[i]=='_') v[i] = ' ';
       v = '('+v+')';
       qq.iso(iso);
       qq.v = v;
       if (!flt()) flt_make();
       for(size_t i=0;i<size();i++) 
         flt_add(i,(q(i).iso() == iso && q(i).v == qq.v && q(i).k == qq.k));
       return repaint;
    }else if (par == "vkg"){ // v must be in the form 170 or 0_10_0
       int iso,g;
       VJKG qq;
       string v;
       istr >> iso >> v >> qq.k >> qq.g;
       for (size_t i=0;i<v.size();i++) if (v[i]=='_') v[i] = ' ';
       v = '('+v+')';
       qq.iso(iso);
       qq.v = v;
       if (!flt()) flt_make();
       for(size_t i=0;i<size();i++) {
         g = q(i).g + q(i).j + qq.g;
         flt_add(i,  q(i).iso() == iso && q(i).v == qq.v && q(i).k == qq.k &&
                     g/2*2==g  );
       };
       return repaint;
    }else return spbase::cmd(str);
    return ok;
  };

  if (com=="info"){
    size_t n=0;
    int iso=0;
    map<vibq,int> nn;
    string fn;
    istr>>iso>>fn;
    ofstream fil;
    if (!fn.empty()) fil.open(fn.c_str());
    if (!fil) fn = "";
    for(size_t i=0;i<size();i++){
      if (!allowed(i)) continue;
      if (q(i).iso()!=iso) continue;
      n++;
      nn[q(i).v]++;
    };
    mio<<"Energy list "<<object::name<<", iso="<<iso<<", N="<<n<<'\n';
    for(map<vibq,int>::iterator m=nn.begin();m!=nn.end();m++)
      if (fn.empty())
        mio<<m->first<<' '<<m->second<<'\n';
      else
        fil<<m->first<<' '<<m->second<<'\n';
    return ok;
  };

  if (com=="swapq"){
    if (!size()) return bad;
    string s1,s2;
    VJKG q1,q2;
    size_t i = rest.find('-');
    if (i == string::npos) {
      mio<<"swapq: wrong command format."<<'\n';
      return bad;
    };
    s1 = rest.substr(0,i);
    s2 = rest.substr(i+1);
    i = s1.find('['); // there is [iso] in s1
    if (i == string::npos){
      int iso = q(0).iso();
      q1.iso(iso);
      q2.iso(iso);
    };
    q1.assign(s1);
    q2.assign(s2);
    swapq(q1,q2);
    return ok;
  };

  if (com == "vseq"){ // vseq file v1 v2 v3 j k g
    if (!size()){
      mio<<"List is empty"<<'\n';
      return bad;
    };
    size_t n=q(0).nv();
    size_t j = n;
    vibq v;
    v.n(n);
    string s,fn;
    istr >> fn;
    for(size_t i=0;i<n;i++){
      istr>>s;
      if (s == "*") {
        v[i] = 255;
        j = i;
      }else
        v[i] = atoi(s.c_str());
    };
    if (j >= n){
      mio<<"Cannot make sequence"<<'\n';
      return bad;
    };
    int J,k,g;
    istr>>J>>k>>g;
    plot.clear();
    pair<double,double> p;
    for(size_t i=0;i<size();i++)
      if (q(i).v == v && q(i).j==J && q(i).k == k && q(i).g == g){
        p.first = double(q(i).v[j]);
        p.second = ener(i);
        plot.push_back(p);
      };
    savePlot(fn);
    return ok;
  };

  if (com == "jseq"){ // vseq file (vvv) j k symm
    if (!size()){     // if j == 0 -- even J, j == 1 -- odd, j == 2 -- both
      mio<<"List is empty"<<'\n';
      return bad;
    };
    string fn;
    istr>>fn;
    VJKG qq;
    qq.assign(rest);
    plot.clear();
    pair<double,double> p;
    int J;
    mio<<qq<<'\n'<<"------------------"<<'\n';
    for(size_t i=0;i<size();i++)
      if (q(i).v == qq.v && q(i).k == qq.k && q(i).symm() == qq.g){
        J = qq.j + q(i).j;
        if (qq.j < 2 && J/2*2 != J) continue;
        p.first = double(q(i).j);
        p.second = ener(i);
        plot.push_back(p);
        mio<<q(i)<<'\n';
      };
    savePlot(fn);
    return ok;
  };

  //if (com=="sorts"){
  //  sort_symm();
  //  return ok;
  //};

  //if (com=="sortq"){
  //  sort_q();
  //  return ok;
  //};

  if (com=="curr"){
    VJKG qq;
    qq.assign(rest);
    size_t i = index(qq);
    if (i < size()) setCurr(i);
    return ok;
  };

  if (com == "next") {
     size_t i = curr();
     while(next() < size()-1 && !allowed());
     if (!allowed()) setCurr(i);
     return ok;
  };

  if (com == "prev") {
     size_t i = curr();
     while(prev() > 0 && !allowed());
     if (!allowed()) setCurr(i);
     return ok;
  };
  
  if (com == "diff"){
    string en_name;
    istr>>en_name;
    if (en_name == object::name){
      mio<<"Comparing the same list\n";
      return bad;
    };
    object* o = owner->Obj(en_name);
    if (!o) {
      mio<<"No energies "<<en_name<<'\n';
      return bad;
    };
    diff(*(enlist*)o);
    return ok;
  };

  if (com == "cmp_q"){
    string en_name;
    int Jmax;
    istr>>en_name>>Jmax;
    if (en_name == object::name){
      mio<<"Comparing the same list\n";
      return bad;
    };
    object* o = owner->Obj(en_name);
    if (!o) {
      mio<<"No energies "<<en_name<<'\n';
      return bad;
    };
    cmp_q(*(enlist*)o,Jmax);
    return ok;
  };

  return spbase::cmd(str);
}

void enlist::swapq(VJKG& q1,VJKG& q2){
  if (q1.symm() != q2.symm()) {
    mio<<"swapq: different symmetries"<<'\n';
    return;
  };
  size_t i1 = index(q1);
  size_t i2 = index(q2);
  if (i1 >= size() || i2 >= size()) {
    mio<<"swapq: cannot find one of the states"<<'\n';
    return;
  };
  q(i1) = q2;
  q(i2) = q1;
}

//void enlist::sort_symm(){
//  vector<size_t> ind;
//  ind.resize(size());
//  for(size_t i=0;i<ind.size();i++)
//    ind[i] = i;
//  sort(ind.begin(),ind.end(),comp_symm(*this));
//  for(size_t i=0;i<fields.size();i++) {
//    fields[i]->sort(ind);
//  };
//}
//
//void enlist::sort_q(){
//  vector<size_t> ind;
//  ind.resize(size());
//  for(size_t i=0;i<ind.size();i++)
//    ind[i] = i;
//  sort(ind.begin(),ind.end(),comp_q(*this));
//  for(size_t i=0;i<fields.size();i++) {
//    fields[i]->sort(ind);
//  };
//}

void enlist::diff(enlist& en){
  vector<double> *dep = getDoubleNew("de");
  if (!dep){
    mio<<"Cannot allocate d:de\n";
    return;
  };
  vector<double> &de = *dep;
  for(size_t i=0;i<size();i++){
    de[i] = ener(i) - en.ener(q(i));
  };
}

fun_res enlist::fun(string str){
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

  if (nam == "ener"){
    ostr << ener(curr());
    return ostr.str();
  };

  if (nam == "q"){
    ostr << q(curr());

    return ostr.str();
  };

  return spbase::fun(str);
}

//  makes a multicolumn table with different vib states in different columns taken from
//  enlist en and writes it down into file fn.
//  this enlist must contain rotational quanta to be tabulated before calling table(...)
void enlist::table(enlist& en,string fn_and_states){
  map<vibq,string> vv;
  map<vibq,bool> vt;
  vibq v0;
  string fn;
  istringstream istr(fn_and_states);
  istr>>fn;
  
  string str;
  while(istr>>str){
    v0 = str;
    vt[v0] = true;
    vv[v0] = str;
  };
  
  if (vv.empty()){
    for(size_t i=0;i<en.size();i++){
      v0 = en.q(i).v;
      vv[v0] = v0.str();
      vt[v0] = true;
    };
  };
  
  vector<double>* p;
  map<vibq,vector<double>*> vp;
  for(map<vibq,string>::iterator v=vv.begin();v!=vv.end();v++){
    p = getDoubleNew(v->second);
    if (!p) {
      mio<<"enlist::table -- error happend\n";
      return;
    };
    vp[v->first] = p;
  };
  
  v0 = q(0).v; // vib state in this enlist
  VJKG qq;
  size_t j;
  en.flt_make();// to filter out energies in en which will not get into table
  for(size_t i=0;i<en.size();i++){
    qq = en.q(i);
    en.flt_rm(i);
    if (!vt[qq.v]) continue;
    p = vp[qq.v];
    if (!p) {
      mio<<"enlist::table -- error happend(1)\n";
      return;
    };
    // placing energy from en in the raw with the same rot quanta
    qq.v = v0;
    j = index(qq);
    if (j<size()){
      (*p)[j] = en.ener(i);
    }else{
      en.flt_add(i);
    };
  };
  
  ofstream fil(fn.c_str());
  if (!fil){
    mio<<"enlist::table -- cannot open file "<<fn<<" for writing\n";
    return;
  };
  
  
  fil<<"  J Ka Kc";
  for(map<vibq,string>::iterator v=vv.begin();v!=vv.end();v++){
    if (!vt[v->first]) continue;
    fil.width(12);
    fil<<v->second;
  };
  fil<<'\n';
  for(j=0;j<size();j++){
    qq = q(j);
    fil.width(3);
    fil<<qq.j;
    fil.width(3);
    fil<<qq.k;
    fil.width(3);
    fil<<qq.g;
    for(map<vibq,vector<double>*>::iterator pp=vp.begin();pp!=vp.end();pp++){
      if (!pp->second) continue;
      fil.precision(8);
      fil.width(12);
      fil<<(*pp->second)[j];
    };
    fil<<'\n';
  };
}

void enlist::cmp_q(enlist& en,int Jmax){
  VJKG qq,qq1;
  size_t sym;
  double dmin,e,de;
  DataObjects::ColumnVector<VJKG> qp = getColumn("q1");
  if (!qp){
    mio<<"Error in enlist::cmp_q\n";
    return;
  };
  vector<double>* dde = getDoubleNew("de");
  for(size_t i=0;i<size();i++){
    qq = q(i);
    sym = qq.symm();
    e = ener(i);
    dmin = 1e30;
    (qp)[i] = qq;
    (*dde)[i] = 0.;
    for(size_t j=0;j<en.size();j++){
      qq1 = en.q(j);
      de = e - en.ener(j);
      if (qq.j > Jmax) (qp)[i] = qq;
      else if (qq.iso() == qq1.iso() && qq.j == qq1.j && sym == qq1.symm() && fabs(de) < dmin){
        dmin = fabs(de);
	    (qp)[i] = qq1;
	    (*dde)[i] = de;
      };
    };
  };
}


//bool enlist::draw(canvas& c){
//  if (c.type == "graph" && c.out_type == "def"){
//   graphics& g = *(graphics*)(&c);
//   g.clip_clientRect();
//   g.pen(color);
//   if (out_type == "qk"){
//     if (!ener_p || !q_p) return false;
//     double x,y;
//     int xx,yy,p2=pnt_size/2;
//     for(size_t i=0;i<size();i++){
//        if (!allowed(i)) continue;
//        x = q(i).k;
//        y = ener(i);
//        xx = g.X(x);
//        yy = g.Y(y);
//        if (x > g.uMin() && x < g.uMax() && y > g.vMin() && y < g.vMax())
//          g.drawRect(rect(xx-p2,yy-p2,pnt_size,pnt_size));
//     };
//     return true;
//   };
//   if (out_type == "qj"){
//     if (!ener_p || !q_p) return false;
//     double x,y;
//     int xx,yy,p2=pnt_size/2;
//     for(size_t i=0;i<size();i++){
//        if (!allowed(i)) continue;
//        x = q(i).j;
//        y = ener(i);
//        xx = g.X(x);
//        yy = g.Y(y);
//        if (x > g.uMin() && x < g.uMax() && y > g.vMin() && y < g.vMax())
//          g.drawRect(rect(xx-p2,yy-p2,pnt_size,pnt_size));
//     };
//     return true;
//   };
//  };
//  return spbase::draw(c);
//}

} // Goblin
