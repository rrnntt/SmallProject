#include <malloc.h>

#include "Goblin/lineparams.h"
#include "Goblin/splist.h"
#include "Goblin/enlist.h"
#include "Goblin/matrix.h"
#include "Goblin//mio.h"
#include "Goblin/gdata.h"

#include <fstream>
#include <sstream>
#include <valarray>
#include <stdlib.h>
#include <algorithm>
#include <iomanip>

namespace Goblin
{

using namespace std;

splist::splist(){
  lp = new lineparams;
  style = FROM_ZERO;
  q_p = 0;
  q0_p = 0;
  cnnct = 0;
  find_dw = 0.1;
  find_eh = 4.;
  find_max = 50;
  follow = true;
  fit_i1 = fit_i2 = 0;
  //color = rgb(0,0,255);
  //cur_color = rgb(255,0,0);
  noise_ = true;
  fit_vis = false;
  mouse_add_line = false;
  cd.sp = this;
}
splist::~splist(){
  delete lp;
}

bool splist::load(string fn){
  dbase::loadFromFile(fn);
//  apply_params();
  db_field<VJKG>* qf = getField<VJKG>("vjkg","q");
  q_p = qf?&qf->data:0;
  qf = getField<VJKG>("vjkg","q0");
  q0_p = qf?&qf->data:0;
  vector<double>* lin = getDouble("line");
  if (lin){
    for(size_t i=1;i<lin->size();i++)
      if ((*lin)[i] == 0.) (*lin)[i] = (*lin)[i-1];
  };
  line_ = lin;
  height_ = getDouble("height");
  if (!height_) height_ = getDouble("intens");
  return true;
}


string splist::print(size_t i){
  if (i>=size()) return "";
  ostringstream ostr;
  ostr << lparams().print(i);
  if (q_p) ostr<<' '<<q(i);
  if (q0_p) ostr<<" - "<<q0(i);
  return ostr.str();
}

void splist::apply_params(){
  string value;
  setParamType(params["par"]);
  exp_cond.T(atof(params["temper"].c_str()));
  exp_cond.L(atof(params["length"].c_str()));
  value = params["style"];
  if (value == "zero") style =  FROM_ZERO;
  else if (value == "top") style =  FROM_TOP;
  else if (value == "whole") style =  WHOLE;
  else if (value == "no") style =  NONE;

  value = params["nmix"];
  if (!value.empty()) {
    int nmix = atoi(value.c_str());
    string prs;
    char ch[]="0123456789";
    size_t j;
    for(int i=0;i<nmix;i++){
      prs = "P";
      prs += ch[i];
      value = params[prs];
      exp_cond.addMix(atof(value.c_str()));
      prs = "Iso";
      prs += ch[i];
      value = params[prs];
      while((j = value.find_last_of('+')) != string::npos){
        prs = value.substr(j+1);
        exp_cond.addIso(i,atoi(prs.c_str()));
        value.erase(j);
      };
      exp_cond.addIso(i,atoi(value.c_str()));
    };
  };
  mio<<"----- exp -----"<<name<<'\n';
  mio<<"T="<<exp_cond.T()<<'\n';
  mio<<"L="<<exp_cond.L()<<'\n';
  for(size_t i=0;i<exp_cond.nmix();i++){
    mio<<"P="<<exp_cond.P(i)<<'\n';
    for(size_t j=0;j<exp_cond.niso(i);j++)
      mio<<"Iso["<<j<<"]="<<exp_cond.Iso(i,j)<<'\n';
  };
}


bool splist::save(){
  if (write()) return save(fname);
  return false;
}

bool splist::save(string fn){
  switch(style){
    case FROM_ZERO: params["style"] = "zero"; break;
    case FROM_TOP: params["style"] = "top"; break;
    case WHOLE: params["style"] = "whole"; break;
    case NONE: params["style"] = "no"; break;
  };
  vector<int> *f = 0;
  if (flt() && flt_enabled()) f = flt_;
  dbase::saveToFile(fn.c_str(),f);
  return true;
}

void splist::sort(){
  sort_rows<double>(string("d"),string("line"));
}

void splist::make_quanta(){
  db_field<VJKG>* qf = getFieldNew<VJKG>("vjkg","q");
  q_p = qf?&qf->data:0;
  qf = getFieldNew<VJKG>("vjkg","q0");
  q0_p = qf?&qf->data:0;
};

// Calculates upper state energies and stores them in an existing
//  enlist en.
// e0f - name of the field with lower energies
// If field line_err exists make_ener uses it to weight the frequencies.
void splist::make_ener(enlist& en,const string e0f){
  if (!assigned()) return;
  vector<double> *e0p = getDouble(e0f);
  vector<double> *line_err = getDouble("line_err");
  if (!e0p) {
    mio<<"Field "<<e0f<<" doesnt exist.\n";
    return;
  };
  vector<double>& e0 = *e0p;
  vector<double>& err = (en.getFieldNew<double>("d","err")->data);
  vector<int>& nl = (en.getFieldNew<int>("i","n")->data);
  vector<double>& wl = (en.getFieldNew<double>("d","wl")->data);;
  size_t j;
  double ee,de,c=1.;
  for(size_t i=0;i<size();i++){
    if (!q(i).assigned()) continue;
    j = en.index(q(i));
    if (j >= en.size()) { // new energy
      j = en.add(q(i),line(i)+e0[i]);
      nl[j] = 1;
      if (line_err){
        de = (*line_err)[i];
        err[j] = de;
        wl[j] = c/de;
      }else
        err[j] = 0.;
    }else{
      ee = line(i) + e0[i];
      if (line_err){
        de = (*line_err)[i];
        err[j] += de;
        de = c/de;
        en.ener(j) = (wl[j]*en.ener(j) + de*ee) / (wl[j] + de);
        wl[j] += de;
      }else{
        en.ener(j) = (nl[j]*en.ener(j) + ee) / (nl[j] + 1);
        de = abs(ee - en.ener(j));
        if (de > err[j]) err[j] = de;
      };
      ++nl[j];
    };

  };
  if (line_err){
    for(j=0;j<err.size();j++){
//      err[j] = sqrt(c*err[j]/wl[j]);
//      err[j] = c/wl[j];
      err[j] = sqrt(c*err[j]/wl[j]/nl[j]);
    };
  }else{
    for(j=0;j<err.size();j++){
      err[j] = sqrt(c*err[j]/nl[j]);
    };
  };
  en.sort_q();
}

// Calculates upper state energies and stores them in an existing
//  enlist en.
// e0f - name of the field with lower energies
// If field s:tag exists make_ener_tag tries not to use lines tagged "w", "b", or "s"
//  (weak, blend, saturated)
void splist::make_ener_tag(enlist& en,const string e0f){
  if (!assigned()) return;
  vector<double> *e0p = getDouble(e0f);
  vector<string> *ptag = getString("tag");
  string* tg;
  if (!e0p) {
    mio<<"Field "<<e0f<<" doesnt exist.\n";
    return;
  };
  vector<double>& e0 = *e0p;
//  vector<double>& err = (en.getFieldNew<double>("d","err")->data);
  vector<int>& nl = (en.getFieldNew<int>("i","n")->data);
  map< size_t,vector<size_t> > indx;
  vector<size_t>* indxj;
  map<size_t,bool> del_tagged;
  bool tagged;
  size_t i,j;
  double ee,de,c=1.;
  for(i=0;i<size();i++){
    if (!q(i).assigned()) continue;
    j = en.index(q(i));
    if (j >= en.size()){  // new energy
      j = en.add(q(i),0.);
      del_tagged[j] = false;
      nl[j] = 0;
    };
    indx[j].push_back(i);
    if (ptag){
      tg = &(*ptag)[i];
      tagged = tg->find("w")!=string::npos || tg->find("b")!=string::npos || 
           tg->find("s")!=string::npos;
      if (!tagged) del_tagged[j] = true;
    };
  };
  ofstream ferr("tmp_err.txt");
  for(j=0;j<en.size();j++){
    indxj = &indx[j];
    ferr<<en.q(j)<<'\n';
    if (del_tagged[j]){
      for(vector<size_t>::iterator ii=indxj->begin();ii!=indxj->end();ii++){
        tg = &(*ptag)[*ii];
        tagged = tg->find("w")!=string::npos || tg->find("b")!=string::npos || 
           tg->find("s")!=string::npos;
        if (!tagged){
          ee = line(*ii) + e0[*ii];
          en.ener(j) = (nl[j]*en.ener(j) + ee) / (nl[j] + 1);
          nl[j]++;
          ferr<<q0(*ii)<<' '<<line(*ii)<<endl;
        };
      };
    }else{
      for(vector<size_t>::iterator ii=indxj->begin();ii!=indxj->end();ii++){
        ee = line(*ii) + e0[*ii];
        en.ener(j) = (nl[j]*en.ener(j) + ee) / (nl[j] + 1);
        nl[j]++;
        ferr<<q0(*ii)<<' '<<line(*ii)<<endl;
      };
    };
    ferr<<endl;
  };
  en.sort_q();
}

template<class T>
string put_str(T v,size_t wdt){
  ostringstream ostr;
  ostr.precision(wdt);
//  ostr.width(wdt);
  ostr<<v;
  string str = ostr.str();
  if (str.size() > wdt) return str.substr(0,wdt);
  size_t l = str.size();
  if (str.size() < wdt)
     for(size_t i=0;i<wdt-l;i++) str += ' ';
  return str;
}

struct xyq{
  double x,y;
  VJKG q,q0;
  xyq(double xx,double yy,VJKG& qq,VJKG& qq0):x(xx),y(yy),q(qq),q0(qq0){}
} ;

bool lt_by(double a,double diff=0.){
  return a < diff;
};

bool le_by(double a,double diff=0.){
  return a <= diff;
};

bool gt_by(double a,double diff=0.){
  return a  > diff;
};

bool ge_by(double a,double diff=0.){
  return a  >= diff;
};

bool eq_by(double a,double diff=0.){
  return a  == diff;
};

cmd_res splist::cmd(string str){
  cmd_res ok,bad(false),repaint;
  repaint.repaint(true);
  istringstream istr(str);
  string com;
  istr>>com;
  string rest;
  rest = str.substr(str.find(com)+com.size());

  string who,itscom;
  size_t n;
  if (complex_command(com,who,n,itscom)){
    if (who == "lpar") return lparams().cmd(itscom+' '+rest);
    if (who == "sp"){
      if (n >= spectra.size()){
        mio<<"No spectrum #"<<n<<'\n';
        return bad;
      };
      return spectra[n].cmd(itscom+' '+rest);
    };
    return bad;
  };
  
  obj_list<splist> lines(owner,"splist");
  splist* pcll = 0;
  for(size_t i=0;i<lines.size();i++)
    if (connectedTo(lines[i])){
      pcll = lines(i);
      break;
  };
  
  if (com=="tst"){
    VJKG qq1,qq2;
    map< pair<VJKG,VJKG>,size_t > tst;
    ofstream fil("tst.txt");
    for(size_t i=0;i<size();i++){
      if (line(i) < 9000.) continue;
      if (line(i) > 12000.) break;
      qq1 = q(i);
      qq2 = q0(i);
      qq1.v = "(000)";
      qq2.v = "(000)";
      tst[ make_pair(qq1,qq2) ] += 1;
    };
    for(map< pair<VJKG,VJKG>,size_t >::iterator tt=tst.begin();tt!=tst.end();tt++)
       fil<<(tt->first.first)<<' '<<(tt->first.second)<<' '<<tt->second<<endl;
    return ok;
  };

  if (com=="tag"){
    vector<string>* p = getStringNew("tag");
    if (!p){
      mio<<"Something is wrong!\n";
      return bad;
    };
    vector<string>& tag = *p;
    string what;
    istr>>what;
    if (what == "untag") tag[curr()] = "";
    if (!what.empty() && tag[curr()].find(what)==string::npos) {
      tag[curr()] += what;
      tag[curr()] += ' ';
    };
    mio<<curr()<<' '<<tag[curr()]<<'\n';
    return ok;
  };
  
  if (com=="tagall"){
    vector<string>* p = getStringNew("tag");
    if (!p){
      mio<<"Something is wrong!\n";
      return bad;
    };
    vector<string>& tag = *p;
    string what;
    istr>>what;
    for(size_t i=0;i<size();i++)
      if (allowed(i)){
       if (what == "untag") {tag[i] = "";continue;};
       if (!what.empty() && tag[i].find(what)==string::npos) {
         tag[i] += what;
         tag[i] += ' ';
       };
      };
    return ok;
  };

  if (com=="coadd"){
    float nse;
    string sn;
    int n;
    istr>>sn>>nse>>n;
    obj_list<spectrum> sps(owner,"spectrum");
    spectrum* s = sps(sn);
    if (!s){
        mio<<"Cannot find spectrum "<<sn<<'\n';
        return bad;
    };
    spectrum *c = 0;
    for(int i=0;i<spectra.size();i++)
      if (spectra[i].exp_sp == s){
        c = &spectra[i];
        break;
      };
    if (!c) cmd("spectrum "+sn);
    s->cmd("lines "+name);
    float ii;
    for(int i=0;i<n;i++){
      renew();
      c->add_noise(nse);
      ii = float(i)/(i+1);
      for(size_t j=0;j<c->nop();j++)
        s->r[j] = ii*s->r[j] + c->r[j]/(i+1);
    };
    renew();
  };
  
  if (com=="prep4efit"){
    prep4efit();
    return ok;
  };
  
  if (com=="efd"){
    vector<int> ind;
    string sen,int_fit;
    istr>>sen>>int_fit;
    obj_list<enlist> ener(owner,"enlist");
    enlist* pen = ener(sen);
    if (!pen){
        mio<<"Cannot find enlist "<<sen<<'\n';
        return bad;
    };
    for(size_t i=0;i<cd.found[0].size();i++)
      ind.push_back(cd.found[0][i].line);
    efit(*pen,ind,(int_fit=="int"));
    return repaint;
  };  

  if (com=="efitl"){
    vector<int> ind;
    string sen;
    istr>>sen;
    obj_list<enlist> ener(owner,"enlist");
    enlist* pen = ener(sen);
    if (!pen){
        mio<<"Cannot find enlist "<<sen<<'\n';
        return bad;
    };
    string s;
    bool int_fit = false,out=true;
    while(istr>>s) 
      if (s == "int") int_fit = true;
      else if (s == "slnt") out = false;
      else 
        ind.push_back(atoi(s.c_str()));
    efit(*pen,ind,int_fit,out);
    return repaint;
  };
  
  if (com=="fq"){
    string sen,k;
    istr>>sen>>k;
    obj_list<enlist> ener(owner,"enlist");
    enlist* pen = ener(sen);
    if (!pen){
        mio<<"Cannot find enlist "<<sen<<'\n';
        return bad;
    };
    VJKG qq;
    if (!k.empty())  qq = q(atoi(k.c_str()));
    else
      qq = q();
    if (!qq.assigned()){
      mio<<"Line "<<k<<" is not assigned!\n";
      return bad;
    };
    prep_q4efit(*pen,qq);
    return repaint;
  };
  
  if (com=="fq_info"){
    string sen,k;
    istr>>sen>>k;
    obj_list<enlist> ener(owner,"enlist");
    enlist* pen = ener(sen);
    if (!pen){
        mio<<"Cannot find enlist "<<sen<<'\n';
        return bad;
    };
    VJKG qq;
    if (!k.empty())  qq = q(atoi(k.c_str()));
    else
      qq = q();
    if (!qq.assigned()){
      mio<<"Line "<<k<<" is not assigned!\n";
      return bad;
    };
    prep_q4efit(*pen,qq,true);
    return repaint;
  };
  
  if (com=="fitq"){
    double h1,h2;
    string sen;
    istr>>h1>>h2>>sen;
    if (sen.empty())  fitq(h1,h2);
    else{
      obj_list<enlist> ener(owner,"enlist");
      enlist* pen = ener(sen);
      if (!pen){
        mio<<"Cannot find enlist "<<sen<<'\n';
        return bad;
      };
      fitq(h1,h2,pen);
    };
    return repaint;
  };
  
  if (com=="checkue"){
    if (!assigned()){
      mio<<"Line list is not assigned\n";
      return bad;
    };
    string sen;
    double ddmax,dd;
    istr>>sen>>ddmax;
    obj_list<enlist> ener(owner,"enlist");
    enlist* pen = ener(sen);
    if (!pen){
        mio<<"Cannot find enlist "<<sen<<'\n';
        return bad;
    };
    string fn = owner->dir()+"tmp.txt";
    ofstream fil(fn.c_str());
    vector<double>* e0 = getDouble("e0");
    for(size_t i=0;i<size();i++)
      if (allowed(i) && q(i).assigned()){
        dd = (line(i)+(*e0)[i]-pen->ener(q(i)));
        if (fabs(dd) < ddmax) continue;
        fil<<setprecision(12)
        <<line(i)<<' '<<height(i)<<' '<<q(i)<<" - "<<q0(i)
        <<' '<<dd<<'\n';
      };
    return ok;
  };
  
  if (com=="setue"){ // set energy of upper level
    if (!assigned()){
      mio<<"Line list is not assigned\n";
      return bad;
    };
    string see;
    double ee;
    istr>>see;
    VJKG qq;
    qq = q();//  current line
    vector<double>* lin = getDouble("line");
    vector<double>* e0 = getDouble("e0");
    if (!lin)  return bad;
    if (!e0){ 
      mio<<"d:e0 is needed\n";
      return bad;
    };
    if (see.empty()) ee = line() + (*e0)[curr()];
    else
      ee = atof(see.c_str());
    for(size_t i=0;i<size();i++)
      if (q(i) == qq) {
        (*lin)[i] = ee - (*e0)[i];
      };
    renew();
    return repaint;
  };
  
  if (com=="setui"){ // set energy of upper level
    if (!assigned()){
      mio<<"Line list is not assigned\n";
      return bad;
    };
    if (!connected()){
      mio<<"Linelist "<<name<<" is not connected\n";
      return bad;
    };
    if (!pcll){
      mio<<"Linelist "<<name<<" is not connected\n";
      return bad;
    };
    splist& cll = *pcll;
    string see;
    double ee;
    istr>>see;
    VJKG qq;
    qq = q();//  current line
    vector<double>* lin = getDouble("line");
    vector<double>* hi = getDouble("intens");
    vector<double>* chi = cll.getDouble("intens");
    if (!lin)  return bad;
    if (!hi || !chi){ 
      mio<<"d:intens is needed\n";
      return bad;
    };
    if (con(curr()) < 0) return bad;
    if (see.empty()) ee = (*hi)[curr()]/(*chi)[con(curr())];
    else
      ee = atof(see.c_str());
    for(size_t i=0;i<size();i++)
      if (q(i) == qq && con(i) >= 0) {
        (*hi)[i] = ee * (*chi)[con(i)];
      };
    renew();
    return repaint;
  };
  
  if (com=="dass"){
    if (!assigned()){
      mio<<"Line list is not assigned\n";
      return bad;
    };
    cmd("flt new");
    map< pair<VJKG,VJKG>,int > da;
    int *ida;
    for(size_t i=0;i<size();i++){
      if (!q(i).assigned()) {
        flt_rm(i);
        continue;
      };
      ida = &da[make_pair(q(i),q0(i))];
      if (*ida) flt_add(i);
      else
        flt_rm(i);
      (*ida)++;
    };
    cmd("flt en");
    return repaint;
  };
  
  if (com=="tst_symm"){
    if (!assigned()){
      mio<<"Line list is not assigned\n";
      return bad;
    };
    cmd("flt new");
    for(size_t i=0;i<size();i++){
      if (!q(i).assigned()) flt_rm(i);
      else if (q(i).tsymm() != q0(i).symm()) flt_add(i);
      else
        flt_rm(i);
    };
    cmd("flt en");
    return repaint;
  };
  
  if (com=="goto"){
     double w;
     istr>>w;
     size_t j=0;
     for(size_t i=0;i<size();i++)
       if (line(i) > w){
         j = i;
         break;
       };
     if (j && fabs(line(j-1)-w) < fabs(line(j)-w)) j--;
     setCurr(j);
     return cmd("center");
  };
  
  if (com=="line_error"){
    string e0f;
    double h;
    istr>>e0f>>h;
    line_error(e0f,h);
    return ok;
  };

  if (com=="hitran"){
    string fn;
    istr >> fn;
    ostringstream ostr;
    vector<double> *prob = getDoubleNew("prob");
    vector<double> *air = getDoubleNew("air");
    vector<double> *self = getDoubleNew("self");
    vector<double> *e0 = getDoubleNew("e0");
    vector<double> *n_t = getDoubleNew("n_t");
    vector<double> *shift = getDoubleNew("shift");
    db_field<string>* f = getField<string>("s","txt");
    vector<string> *txt = f?&(f->data):0;
    ofstream fil(fn.c_str());
    for(size_t i=0;i<size();i++){
       if (!allowed(i)) continue;
       fil<<q(i).iso()<<' ';
       fil.precision(12);
       fil<<line(i)<<' ';
       fil<<height(i)<<' ';
       fil<<(*prob)[i]  <<' ';
       fil<<(*air)[i]<<' ';
       fil<<(*self)[i]<<' ';
       fil.precision(12);
       fil<<(*e0)[i]<<' ';
       fil<<(*n_t)[i]<<' ';
       fil<<(*shift)[i]<<' ';
       fil<<size_t(q(i).v(0))<<' '<<size_t(q(i).v(1))<<' '<<size_t(q(i).v(2))<<' ';
       fil<<q(i).j<<' '<<q(i).k<<' '<<q(i).g<<' ';
       fil<<size_t(q0(i).v(0))<<' '<<size_t(q0(i).v(1))<<' '<<size_t(q0(i).v(2))<<' ';
       fil<<q0(i).j<<' '<<q0(i).k<<' '<<q0(i).g;
       if (txt) fil<<'\n'<<(*txt)[i];
       fil<<'\n';
    };
    return ok;
  };
  //if (com=="color"){
  //  int r,g,b;
  //  istr>>r>>g>>b;
  //  color = rgb(r,g,b);
  //  return repaint;
  //};
  //if (com=="cur_color"){
  //  int r,g,b;
  //  istr>>r>>g>>b;
  //  cur_color = rgb(r,g,b);
  //  return repaint;
  //};
  if (com=="errors"){
    lparams().error(true);
    return ok;
  };

  if (com=="lpar"){
    lparams().cmd(str.substr(str.find(com)+com.size()));
    return repaint;
  };
  
  if (com == "goto"){
    double w;
    istr>>w;
    size_t j = curr();
    for(size_t i=0;i<size();i++){
      if (w > line(i)) continue;
      if (i && fabs(w-line(i-1)) < fabs(w-line(i))) j = i - 1;
      else
        j = i;
      break;
    };
    setCurr(j);
    return repaint;
  };
  
  //if (com=="keypress"){
  //  int key;
  //  string in_type;
  //  char sh;
  //  istr>>key>>in_type>>sh;
  //  string inp;
  //  bool inp_ok;
  //  double pp;
  //  if (in_type == "def"){
  //    switch (key){
  //      case key_A: mouse_add_line = true; return repaint;
  //      case key_C: return (sh!='s')?cmd("d ch"):cmd("c intf");
  //      case key_F: fit(curr(),curr()); return repaint;
  //      case key_E: return cmd("fit_vis");
  //      case key_K: return cmd("nextl");
  //      case key_J: return cmd("prevl");
  //      case key_R: return cmd("renew");
  //      case key_Q: return cmd("fq efit");
  //      case key_I: return cmd("fq_info efit");
  //      case key_T: return (sh == 's')?cmd("setue"):bad;
  //      case key_Y: return (sh == 's')?cmd("setui"):bad;
  //      case key_W: return (sh == 's')?cmd("tag"):cmd("tag w");
  //      case key_D: return cmd("tag b");
  //      case key_S: return cmd("tag s");
  //      case key_Tab: return cmd("next");
  //      case key_Slash: return cmd("center");
  //      case key_Delete: if (mio.confirm("Do you really want to delete this line?")){
  //                          deleteLine(curr()); return repaint;
  //                       }else return ok;
  //      case key_0:   
  //      case key_1:   
  //      case key_2:   
  //      case key_3:   
  //      case key_4:   
  //      case key_5:   
  //      case key_6:   
  //      case key_7:   
  //      case key_8:   
  //      case key_9:   if (sh == 's'){
  //                       ostringstream ostr;
  //                       ostr<<"d goto "<<key-key_0;
  //                       return cmd(ostr.str());
  //                    }else return bad;
  //      case key_F1:   
  //      case key_F2:   
  //      case key_F3:   
  //      case key_F4:   
  //      case key_F5:   
  //      case key_F6:   
  //      case key_F7:   
  //      case key_F8:   
  //      case key_F9:   if (sh == 's'){
  //                        inp = mio.in(&inp_ok);
  //                        if (inp_ok) {
  //                           pp = atof(inp.c_str());
  //                           lparams().par(curr(),key-key_F1) = pp;
  //                           renew();
  //                        };
  //                     }else{
  //                        lparams().fixed(curr(),key-key_F1)?          
  //                        lparams().unfix(curr(),key-key_F1):
  //                        lparams().fix(curr(),key-key_F1);
  //                      };
  //                       return repaint;
  //    };
  //    return bad;
  //  };
  //  return bad;
  //};

  
  //if (com == "center"){
  //  if (!cname(owner->plt.focname)) return bad;
  //  double u0,u1,v0,v1,u;
  //  canvas* c = owner->plt.cans[owner->plt.focname];
  //  if (c->type != "graph") return bad;
  //  graphics& g = *(graphics*)c;
  //  u = line();
  //  u0 = u - (g.uMax() - g.uMin())/2;
  //  u1 = u + (g.uMax() - g.uMin())/2;
  //  v0 = g.vMin();
  //  v1 = g.vMax();
  //  g.resize_uv(u0,u1,v0,v1);
  //  return repaint;
  //};
  
  if (com=="idist"){
    int is;
    string vv,fn;
    VJKG qq;
    istr>>is>>vv;
    qq.iso(is);
    qq.v = vv;
    vector< map<int,double> > out(100);
    fn = owner->dir()+"idist.txt";
    ofstream fil(fn.c_str());
    if (!fil){
      mio<<"Cannot open "<<fn<<'\n';
      return bad;
    };
    int j;
    double d;
    map<int,double>::iterator o;
    for(size_t i=0;i<size();i++){
      j = q(i).j;
      d = height(i);
      if (q(i).iso() == qq.iso() && q(i).v == qq.v && j < 100 && d > out[j][q(i).k]){
        out[j][q(i).k] = d;
      };
    };
    for(j=0;j<out.size();j++){
      for(o=out[j].begin();o!=out[j].end();o++) fil<<o->first<<' '<<o->second<<'\n';
      fil<<'\n';
    };
    return ok;
  };

  if (com=="swapq"){// swapq {11} (111) 2 2 1 - (120) 2 1 1
    if (!size()) return 0;// swapq $ (120) 2 1 1
    string s1,s2;
    VJKG q1,q2;
    size_t i;
    string a;
    istr>>a;
    if (a == "$"){
      q1 = q(curr());
      q2.iso(q1.iso());
      s2 = rest;
    }else{
      i = rest.find('-');
      if (i == string::npos) {
        mio<<"swapq: wrong command format."<<'\n';
        return bad;
      };
      s1 = rest.substr(0,i);
      s2 = rest.substr(i+1);
      if (s1.find('{') == string::npos && s1.find('[') == string::npos){
        int iso = q(0).iso();
        q1.iso(iso);
        q2.iso(iso);
      };
      q1.assign(s1);
    };
    q2.assign(s2);
    if (q1.symm() != q2.symm()) {
      mio<<"swapq: different symmetries"<<'\n';
      return bad;
    };
    for(size_t i=0;i<size();i++){
      if (q(i) == q1) q(i) = q2;
      else if (q(i) == q2) q(i) = q1;
    };
    return repaint;
  };

  if (com=="flt"){
    string par;
    istr>>par;
    if (par == "ass"){
      char op=' ';
      istr >> op;
      if (!assigned()){
        flt_make();
        for(size_t i=0;i<size();i++)
           flt_add(i,false,' ');
        return repaint;
      };
      if (flt())
        for(size_t i=0;i<size();i++)
           flt_add(i,q(i).assigned(),op);
      else{
        flt_make();
        for(size_t i=0;i<size();i++)
           flt_add(i,q(i).assigned(),' ');
      };
      return repaint;
    }else if (par == "iso"){
       int iso;
       char op =' ';
       istr >> iso >> op;
       if (!assigned()) {
         mio<<"List not assigned\n";
         return bad;
       };
       if (!flt()) flt_make();
       for(size_t i=0;i<size();i++) flt_add(i,q(i).iso() == iso,op);
       return repaint;
    }else if (par == "vv"){ // v must be in the form 170 or 0_10_0
       if (!assigned()) {
         mio<<"List not assigned\n";
         return bad;
       };
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
    }else if (par == "v0"){ // v must be in the form 170 or 0_10_0
       if (!assigned()) {
         mio<<"List not assigned\n";
         return bad;
       };
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
       for(size_t i=0;i<size();i++) flt_add(i,(q(i).iso() == iso && q0(i).v == qq.v),op);
       return repaint;
    }else if (par == "j-j0"){
       if (!assigned()) {
         mio<<"List not assigned\n";
         return bad;
       };
       int j,j0;
       char op =' ';
       istr >> j >> j0 >> op;
       if (!flt()) flt_make();
       for(size_t i=0;i<size();i++) 
         flt_add(i,
         ((q(i).j == j && q0(i).j == j0)||(q(i).j == j0 && q0(i).j == j)||
          (q(i).j == j && q0(i).j == j)||(q(i).j == j0 && q0(i).j == j0))
         ,op);
       return repaint;
    }else if (par == "dj"){
       if (!assigned()) {
         mio<<"List not assigned\n";
         return bad;
       };
       int dj;
       char op =' ';
       istr >> dj >> op;
       if (!flt()) flt_make();
       for(size_t i=0;i<size();i++) 
         flt_add(i,  (q(i).j - q0(i).j == dj) ,op);
       return repaint;
    }else if (par == "dk"){
       if (!assigned()) {
         mio<<"List not assigned\n";
         return bad;
       };
       int dk;
       char op =' ';
       istr >> dk >> op;
       if (!flt()) flt_make();
       for(size_t i=0;i<size();i++) 
         flt_add(i,  (q(i).k - q0(i).k == dk) ,op);
       return repaint;
    }else return spbase::cmd(str);
  };
  
  if (com == "deass"){
    size_t i = curr();
    if (allowed(i)){
      q(i).deassign();
      q0(i).deassign();
    };
    return repaint;
  };
  
  if (com == "deass_all"){
    for(size_t i=0;i<size();i++)
      if (allowed(i)) {
        q(i).deassign();
        q0(i).deassign();
      };
    return repaint;
  };
    
  if (com == "info"){
    size_t na=0,n=0;
    map<int,int> iso_a, iso;
    int is;
    mio<<"Linelist "<<name<<'\n';
    if (assigned()){
      for(size_t i=0;i<size();i++){
        if (!allowed(i)) continue;
        n++;
        iso[q(i).iso()]++;
        if (q(i).assigned()) iso_a[q(i).iso()]++;
      };
      map<int,int>::iterator m;
      for(m=iso.begin();m!=iso.end();m++){
        is = m->first;
        mio<<"Lines of iso "<<is<<" = "<<m->second<<" assigned "<<iso_a[is]<<'\n';
        na += iso_a[is];
      };
    };
    mio<<"Total number of lines "<<size()<<'\n';
    mio<<"Lines considered "<<n<<" assigned "<<na<<'\n';
    return ok;
  };

  if (com == "blends"){
    string par;
    istr>>par;
    if (par=="eq"){ // makes some parameters of blended lines equal to ones of the strongest
      vector<string> flds;
      string f;
      while(istr>>f) flds.push_back(f);
      if (!flds.size()) return 0;
      vector<vector<double>*> pf;
      vector<double>* p;
      for(size_t j=0;j<flds.size();j++){
        p = getDouble(flds[j]);
        if (p) pf.push_back(p);
      };
      if (!pf.size()) return 0;
      blend_struct b;
      for(size_t i=0;i<size();){
        if (!isblend(i,1.,&b)) i++;
        else{
          for(size_t j=b.i1;j<=b.i2;j++){
            if (j == b.im) continue;
            for(size_t k=0;k<pf.size();k++)
              (*pf[k])[j] = (*pf[k])[b.im];
          };
          i = b.i2 + 1;
        };
      };
      return 1;
    };

    if (par=="flt"){
      flt_make();
      double lin = line(0);
      for(size_t i=1;i<size();i++){
        if (line(i) == lin) {
          flt_add(i-1);
          flt_add(i);
          if (assigned() && q(i)==q(i-1)) mio<<"Double assignment "<<i<<" at "<<line(i)<<'\n';
        }else
          flt_rm(i);
        lin = line(i);
      };
      return ok;
    };
    if (par=="split"){
      double lin = line(0);
      vector<double> *wp = getDouble("line");
      if (!wp){
        mio<<"No line centers!!!"<<'\n';
        return bad;
      };
      vector<double> &w = *wp;
      double shift;
      istr >> shift;
      if (shift == 0.) shift = 0.00001;
      for(size_t i=1;i<size();i++){
        if (line(i) == lin) {
          w[i] += shift;
        };
        lin = line(i);
      };
      return repaint;
    };
    if (par=="join"){
      double lin = line(0);
      string hei = "";
      istr >> hei;
      if (hei.empty()) hei = "intens";
      vector<double> *hp = getDouble(hei);
      if (!hp){
        mio<<"No line intensities!"<<'\n';
        return bad;
      };
      vector<double> &h = *hp;
      size_t i0 = 0;
      double h0;
      VJKG qq,qq0;
      for(size_t i=1;i<size();i++){
        if (line(i) == lin) {
          h[i0] += h[i];
          if (h[i] > h0){
            h0 = h[i];
            qq = q(i);
            qq0 = q0(i);
            q(i) = q(i0);
            q0(i) = q0(i0);
            q(i0) = qq;
            q0(i0) = qq0;
          };
          h[i] = 0.;
        }else {
          i0 = i;
          h0 = h[i];
        };
        lin = line(i);
      };
      return repaint;
    };
    return ok;
  };

  if (com == "sticks"){
    string sy;
    istr>>sy;
    vector<double>* y = getDouble(sy);
    if ( !y ){
       mio<<"Field "<<sy<< " is missing."<<'\n';
       return bad;
    };
    string fn = owner->dir() + "gobo.out";
    ofstream fil(fn.c_str());
    if (!fil){
      mio<<"Cannot open file "<<fn<<'\n';
      return bad;
    };
    double x;
    for(size_t i=0;i<size();i++){
      if (!allowed(i)) continue;
      fil.precision(12);
      x = line(i);
      fil<<x<<' '<<0.<<'\n';
      fil<<x<<' '<<(*y)[i]<<'\n';
      fil<<x<<' '<<0.<<'\n';
    };

    return ok;
  };

  if (com == "d"){
    string par;
    istr>>par;
    
    if (par=="goto"){
      size_t i;
      istr>>i;
      if (cd.found.size() && i<cd.found[0].size()){
        ostringstream os;
        os<<"goto "<<line(cd.found[0][i].line);
        return cmd(os.str());
      };
      return ok;
    };
    if (par=="dW"){
      double d;
      istr>>d;
      cd.dW = d;
      return ok;
    };
    if (par=="dw"){
      double d;
      istr>>d;
      cd.dw = d;
      return ok;
    };
    if (par=="dk"){
      int dk;
      istr>>dk;
      cd.dk_max = dk;
      return ok;
    };
    if (par=="set"){  //  d set 12345.6 (110) 1 0 1
      double e;
      VJKG qq;
      istr>>e;
      qq.assign(rest);
      if (!cd.set(e,qq)){
        mio<<"Energies not set."<<'\n';
      };
      return ok;
    };
    
    if (par=="fc"){ // find for current line upper state
      double e;
      VJKG qq = q();
      bool OK = false;
      obj_list<enlist> ener(owner,"enlist");
      for(size_t i=0;i<ener.size();i++)
        if (ener(i) == cd.low_ener){
          OK = true;
          break;
        };
      if (!OK){
        mio<<"Energy lost, set e0 again"<<'\n';
        return bad;
      };
      if (!cd.set(0,qq)){
        mio<<"Energies not set."<<'\n';
      };
//      cd.check();
//      cd.set(cd.found_e(0),qq);
      cd.set(line()+(*cd.low_ener).ener(q0()),qq);
      cd.find();
      mio<<cd;
      return ok;
    };

    if (par=="cfc"){ // find for current line upper state
      if (!connected()){
        mio<<"Linelist "<<name<<" is not connected\n";
        return bad;
      };
      if (!pcll){
        mio<<"Linelist "<<name<<" is not connected\n";
        return bad;
      };
      splist& cll = *pcll;
      bool OK = false;
      obj_list<enlist> ener(owner,"enlist");
      for(size_t i=0;i<ener.size();i++)
        if (ener(i) == cd.low_ener){
          OK = true;
          break;
        };
      if (!OK){
        mio<<"Energy lost, set e0 again"<<'\n';
        return bad;
      };
      if (!cd.set(line()+(*cd.low_ener).ener(cll.q0()),cll.q())){
        mio<<"Energies not set."<<'\n';
      };
      cd.find();
      mio<<cd;
      return ok;
    };

    if (par == "get"){
      size_t i;
      istr>>i;
      cd.get(i);
      mio<<cd;
      return ok;
    };
    
    if (par == "del"){
      size_t i;
      istr>>i;
      cd.del(i);
      mio<<cd;
      return ok;
    };
    
    if (par == "a" || par == "ass"){
      cd.assign();
      mio<<cd;
      return repaint;
    };
    
    if (par == "at"){
      VJKG qq;
      qq.assign(rest);
      cd.assign(qq);
      mio<<cd;
      return repaint;
    };
    
    if (par=="e0"){
      string ener_name;
      istr >> ener_name;
      object* o = owner->Obj(ener_name);
      if (!o || o->type != "enlist") {
        mio<<"No energy list "<<ener_name<<'\n';
        return bad;
      };
      cd.low_ener = (enlist*) o;
      cd.q1.assign(rest);
      return ok;
    };
    if (par=="find"){
      bool OK = false;
      obj_list<enlist> ener(owner,"enlist");
      for(size_t i=0;i<ener.size();i++)
        if (ener(i) == cd.low_ener){
          OK = true;
          break;
        };
      if (!OK){
        mio<<"Energy lost, set e0 again"<<'\n';
        return bad;
      };
      cd.find();
      mio<<cd;
      return ok;
    };
    if (par=="check" || par == "ch"){
      string iso,vvv,j,k,g;
      istr >> iso>>vvv>>j>>k>>g;
      if (iso.empty()) {
        if (!assigned()){
          mio<<"Line list not assigned\n";
          return bad;
        };
        ostringstream os;
        os<<q();
        rest = os.str();
      }else
        rest = "["+iso+"] ("+vvv+") "+j+' '+k+' '+g;
      par="checkq";
    };
    
    if (par=="checkq" || par == "chq"){
      VJKG qq;
      qq.assign(rest);
      if (!cd.set(0.,qq)){
        mio<<"Energies not set."<<'\n';
    	return bad;
      };
      bool OK = false;
      obj_list<enlist> ener(owner,"enlist");
      for(size_t i=0;i<ener.size();i++)
        if (ener(i) == cd.low_ener){
          OK = true;
          break;
        };
      if (!OK){
        mio<<"Energy lost, set e0 again"<<'\n';
        return bad;
      };
      cd.check();
      mio<<cd;
      return ok;
    };
    return ok;
  };

  if (com == "ass"){
    assign(curr(),rest);
    return repaint;
  };

  if (com == "find_dW"){
    istr>>find_dw;
    return ok;
  };

  if (com == "find_eH"){
    istr>>find_eh;
    return ok;
  };

  if (com == "conn"){
    string ll_name;
    string hei,hei1;
    istr>>ll_name>>hei>>hei1;
    object* o = owner->Obj(ll_name);
    if (!o || ll_name==name) return bad;
    connect(*(splist*)o,hei,hei1);
    return repaint;
  };

  if (com == "reconn"){
    string ll_name;
    istr>>ll_name;
    object* o = owner->Obj(ll_name);
    if (!o || ll_name==name) return bad;
    reconnect(*(splist*)o);
    return repaint;
  };

  if (com == "conq"){
    string ll_name;
    istr>>ll_name;
    object* o = owner->Obj(ll_name);
    if (!o || ll_name==name) return bad;
    connect_q(*(splist*)o);
    return repaint;
  };

  if (com == "conq0"){
    string ll_name,ll_hei;
    istr>>ll_name>>ll_hei;
    object* o = owner->Obj(ll_name);
    if (!o || ll_name==name) return bad;
    if (ll_hei.empty()) ll_hei = "intens";
    connect_q0(*(splist*)o,ll_hei);
    return repaint;
  };

  if (com == "c"){
    if (!connected()){
      mio<<"Linelist "<<name<<" is not connected\n";
      return bad;
    };
    if (!pcll){
      mio<<"Linelist "<<name<<" is not connected\n";
      return bad;
    };
    splist& cll = *pcll;
    string par;
    istr>>par;

    if (par=="QS"){
      int i = curr();
      int j = cll.curr();
      mio<<"QS("<<i<<','<<j<<")="<<QS(line(i),height(i),cll.line(j),cll.height(j))<<'\n';
      return ok;
    };
    
    if (par=="intf"){
      VJKG qq = q();
      int j;
      mio<<'\n';
      for(size_t i=0;i<size();i++){
        j = con(i);
        if (q(i) == qq && j != -1)
          mio<<qq<<" - "<<q0(i)<<' '<<height(i)/cll.height(j)<<'\n';
      };
      return ok;
    };
    
    if (par=="setif"){
      VJKG qq = q();
      int j;
      double ff;
      istr>>ff;
      vector<double>* hi = getDouble("intens");
      for(size_t i=0;i<size();i++){
        j = con(i);
        if (q(i) == qq && j != -1)
          (*hi)[i] = ff*cll.height(j);
      };
      return ok;
    };
    
    if (par == "mk_intf") {
      string en_name;
      istr>>en_name;
      object* o = owner->Obj(en_name);
      if (!o || o->type != "enlist"){
        mio<<"No energy list "<<en_name<<'\n';
        return bad;
      };
      enlist& en = *(enlist*)o;
      vector<double> *intfp = en.getDoubleNew("intf");
      vector<double> &intf = *intfp;
      vector<int> *np = en.getIntNew("n");
      vector<int> &n = *np;
      for(size_t i=0;i<en.size();i++) {
        intf[i] = 0.;
        n[i] = 0;
      };
      int j;
      size_t k;
      for(size_t i=0;i<size();i++){
        if (!allowed(i)) continue;
        if (!q(i).assigned()) continue;
        j = con(i);
        if (j == -1) continue;
        k = en.index(q(i));
        if (k >= en.size()) continue;
        n[k]++;
//        intf[k] += ((n[k]-1)*intf[k] + height(i)/cll.height(j))/n[k];
        intf[k] += height(i)/cll.height(j);
      };
      for(size_t i=0;i<en.size();i++) if (n[i]) intf[i] /= n[i];
    return ok;
    };

    if (par=="split"){
      string s1,s2,s3,s4,s5;
      vector<int> ind;
      istr>>s1>>s2>>s3>>s4>>s5;
      if (s1.empty()) return bad;
      ind.push_back(atoi(s1.c_str()));
      if (!s2.empty()) ind.push_back(atoi(s2.c_str()));
      if (!s3.empty()) ind.push_back(atoi(s3.c_str()));
      if (!s4.empty()) ind.push_back(atoi(s4.c_str()));
      if (!s5.empty()) ind.push_back(atoi(s5.c_str()));
      int ii = curr(),jmax;
      double w = line(ii);
      double h = height(ii);
      double h0 = 0.,hj,hmax=0.;
      for(size_t j=0;j<ind.size();j++){
        hj = cll.height(ind[j]);
        h0 += hj;
        if (hj > hmax){
          hmax = hj;
          jmax = j;
        };
      };
      height(ii,hmax*h/h0);
      for(size_t j=0;j<ind.size();j++)
      if (j != jmax){
        set_conn(addLine(w,cll.height(ind[j])*h/h0),ind[j]);
      };
      return repaint;
    };
    
    if (par=="blend"){
      if (!assigned()) {
        mio<<"Linelist is not assigned.\n";
        return bad;
      };
      if (!cll.assigned()) {
        mio<<"Connected linelist is not assigned.\n";
        return bad;
      };
      if (!cll.q().assigned()) {
        mio<<"Line is not assigned.\n";
        return bad;
      };
      if (q() == cll.q() && q0() == cll.q0()){
        mio<<"Lines have the same assignment.\n";
        return bad;
      };
      int i = addLine(line(),0.);
      q() = cll.q();
      q0() = cll.q0();
      set_conn(i,cll.curr());
      return repaint;
    };

    if (par=="flt"){
      string ff;
      istr>>ff;
      if (ff.empty()){
        flt_make();
        for(size_t i=0;i<size();i++)
          if (con(i) != -1) flt_add(i);
          else
            flt_rm(i);
        return repaint;
      }else{
      };
      return bad;
    };

    if (par=="info"){
      size_t na=0,nna=0,nnew=0,nre=0,nda=0,nnc=0,nrl=0;
      int j;
      for(size_t i=0;i<size();i++){
        j = con(i);
        if (j < 0 || !allowed(i)){
          nnc ++;
          continue;
        };
        if (q(i).assigned()){
          na++;
          if (cll.q(j).assigned()){
            if (q(i) != cll.q(j)){
                if (q0(i) != cll.q0(j)) nre++;
                else
                  nrl++;
            };
          }else nnew++;
        }else{
          nna++;
          if (cll.q(j).assigned()) nda++;
        };
      };
      mio<<"Linelist "<<name<<'\n';
      mio<<"Total number of lines "<<size()<<'\n';
      mio<<"Number of lines compared " <<size()-nnc<<'\n';
      mio<<"Assigned lines       "<<na<<'\n';
      mio<<"Newly assigned lines "<<nnew<<'\n';
      mio<<"Reassigned lines     "<<nre<<'\n';
      mio<<"Relabeled lines      "<<nrl<<'\n';
      mio<<"Deassigned lines     "<<nda<<'\n';
      mio<<"Unassigned lines     "<<nna<<'\n';
      return ok;
    };

    if (par=="fol"){
      follow = follow?false:true;
      return ok;
    };

    if (par=="ass"){
       int j = curr();
       if (con(j) == -1) return bad;
       int jc = con(j);
       if (!cll.assigned()) return bad;
       if (!assigned()){
         db_field<VJKG>* qf = getFieldNew<VJKG>("vjkg","q");
         q_p = qf?&qf->data:0;
         qf = getFieldNew<VJKG>("vjkg","q0");
         q0_p = qf?&qf->data:0;
       };
       q(j) = cll.q(jc);
       q0(j) = cll.q0(jc);
       return repaint;
    };
    
    if (par=="assall"){
       int j,jc;
       if (!cll.assigned()) return bad;
       if (!assigned()){
         db_field<VJKG>* qf = getFieldNew<VJKG>("vjkg","q");
         q_p = qf?&qf->data:0;
         qf = getFieldNew<VJKG>("vjkg","q0");
         q0_p = qf?&qf->data:0;
       };
       for (j=0;j<size();j++){
         if (!allowed(j)) continue;
         jc = con(j);
         if (jc == -1) continue;
         q(j) = cll.q(jc);
         q0(j) = cll.q0(jc);
       };
       return repaint;
    };
    
    if (par=="conn"){
       size_t j = curr();
       string sjc;
       int jc;
       istr >> sjc;
       if (sjc.empty()) jc = cll.curr();
       else
         jc = atoi(sjc.c_str());
       if (jc >= cll.size()) jc = cll.curr();
       double w0 = line(j) - find_dw;
       double w1 = line(j) + find_dw;
       for(size_t i=j;i>=0 && line(i)>=w0;i--)
         if (con(i) == jc) set_conn(i,-1);
       for(size_t i=j+1;i<size() && line(i)<=w1;i++)
         if (con(i) == jc) set_conn(i,-1);

       set_conn(j,jc);
       return repaint;
    };

    if (par=="disconn"){
       set_conn(curr(),-1);
       return repaint;
    };

    if (par=="diff"){
       string sy,sx;
       char way=0;
       istr>>sy>>sx>>way;
       vector<double>* y = getDouble(sy);
       vector<double>* x = getDouble(sx);
       vector<double>* y1 = cll.getDouble(sy);
       if ( !y || !x || !y1){
          mio<<"One of the fields does not exist."<<'\n';
          return bad;
       };
       string fn = owner->dir() + "gobo.diff";
       ofstream fil(fn.c_str());
       if (!fil){
         mio<<"Cannot open file "<<fn<<'\n';
         return bad;
       };
       int j;
       for(size_t i=0;i<size();i++){
         if (!allowed(i)) continue;
         j = con(i);
         if (j < 0) continue;
         fil.precision(12);
         fil<<(*x)[i]<<' ';
         if (way == 'r') fil<<(*y)[i]/(*y1)[j];
         else if (way == 'p') fil<<((*y)[i]-(*y1)[j])/(*y)[i];
         else fil<<(*y)[i]-(*y1)[j];
         fil<<'\n';
       };
       return ok;
    };
    
    if (par=="copy"){
       string sy,sx;
       istr>>sy>>sx;
       vector<double>* x = getDoubleNew(sx);
       vector<double>* y = cll.getDouble(sy);
       if ( !y || !x ){
          mio<<"One of the fields does not exist."<<'\n';
          return bad;
       };
       int j;
       for(size_t i=0;i<size();i++){
         if (!allowed(i)) continue;
         j = con(i);
         if (j < 0) continue;
         (*x)[i] = (*y)[j];
       };
       return ok;
    };
    
    if (par=="addblends"){
      vector<double>* x = getDouble("intens");
      vector<double>* y = cll.getDouble("cintens");
      if (!x){
        mio<<"There is no intens in the first llist\n";
        return bad;
      };
      if (!y){
        mio<<"There is no cintens in the second llist\n";
        return bad;
      };
      if (!assigned()){
        mio<<"Linelist is not assigned\n";
        return bad;
      };
      if (!cll.assigned()){
        mio<<"Connected linelist is not assigned\n";
        return bad;
      };
      int j,k1,k2;
      double w,h,ch;
      vector< xyq > newlines;
//      vector< pair<double,double> > newlines;
      for(size_t i=0;i<size();i++){
        if (!allowed(i)) continue;
        j = con(i);
        if (j < 0) continue;
        for(k1=j;k1>=0;k1--) if (cll.line(k1) != cll.line(j)) break;
        if (k1 != j) k1++;
        for(k2=j;k2<cll.size();k2++) if (cll.line(k2) != cll.line(j)) break;
        if (k2 != j) k2--;
//        mio<<k2-k1+1<<'\n';
        if (k1 < k2){
//          mio<<"found "<<i<<k2-k1+1<<'\n';
          w = line(i);
          h = height(i);
          ch = 0.;
          for(int k=k1;k<=k2;k++) ch += (*y)[k];
          (*x)[i] = (*y)[j]/ch*h;
          for(int k=k1;k<=k2;k++){
            if (k == j) continue;
            newlines.push_back(xyq(w,(*y)[k]/ch*h,cll.q(k),cll.q0(k)));
          };
        };
      };
      for(vector< xyq >::iterator il=newlines.begin();il!=newlines.end();il++){
        addLine(il->x,il->y);
        q(curr()) = il->q;
        q0(curr()) = il->q0;
        set_conn(curr(),-1);
      };
      return ok;
    };

    if (par=="badq"){
      if (!assigned() || !cll.assigned()) return bad;
      char op=' ';
      istr >> op;
      int j;
      bool OK;
      if (!flt()){
        flt_make();
        for(size_t i=0;i<size();i++){
          j = con(i);
          if (j != -1){
            if (q(i) != cll.q(j) || q0(i) != cll.q0(j)) flt_add(i);
            else
              flt_rm(i);
          }else
            flt_rm(i);
        };
      }else{
        for(size_t i=0;i<size();i++){
          j = con(i);
          if (j != -1){
            OK = q(i) != cll.q(j) || q0(i) != cll.q0(j);
              if (op == '&') flt_and(i,OK);
              else if (op == '|') flt_or(i,OK);
              else if (op == '^') flt_xor(i,OK);
          };
        };
      };
      return repaint;
    };

    return ok;
  };
/*
  if (com == "n"){
    if (llists.curr()==llists.size()-1) llists.setCurr(0);
    else
      llists.next();
    return 1;
  };

  if (com == "p"){
    if (llists.curr()==0) llists.setCurr(llists.size()-1);
    else
      llists.prev();
    return 1;
  };
*/
  if (com == "eline"){
    if (!assigned()) {
      mio<<"Linelist is not assigned\n";
      return bad;
    };
    string en_name;
    istr>>en_name;
    object* o = owner->Obj(en_name);
    if (!o || o->type != "enlist"){
      mio<<"No energy list "<<en_name<<'\n';
      return bad;
    };
    enlist& en = *(enlist*)o;
    cmd("new d:eline");
    vector<double> *eline = getDouble("eline");
    if (eline == 0){
      mio<<"Field d:eline has not been created\n";
      return bad;
    };
    for(size_t i=0;i<size();i++){
      (*eline)[i] = en.ener(q(i)) - en.ener(q0(i));
    };
    return ok;
  };

  if (com == "eline1"){
    if (!assigned()) {
      mio<<"Linelist is not assigned\n";
      return bad;
    };
    string en_name,e0f = "e0";
    istr>>en_name>>e0f;
    double w,w1;
    object* o = owner->Obj(en_name);
    if (!o || o->type != "enlist"){
      mio<<"No energy list "<<en_name<<'\n';
      return bad;
    };
    enlist& en = *(enlist*)o;
    vector<double> *eline = getDouble("line");
    vector<double> *e0 = getDouble(e0f);
    vector<double> *err = getDouble("line_err");
    vector<double> *dl = 0;
    string fn = owner->dir() + "tmp.txt";
    ofstream fil(fn.c_str());
    if (err)
      dl = getDoubleNew("dl");
    if (eline == 0){
      mio<<"Cannot find d:line !!!\n";
      return bad;
    };
    for(size_t i=0;i<size();i++){
      if (!q(i).assigned()) continue;
      w = line(i);
      w1 = en.ener(q(i)) - (*e0)[i];
      if (dl) (*dl)[i] = w1 - w;
      if (err && fabs(w1-w) > (*err)[i]){
        mio<<"Line "<<i<<" at "<<w<<" moved too far "<<w1<<"\n";
        fil<<q(i)<<q0(i)<<' '<<w<<' '<<(*err)[i]<<' '<<(*dl)[i]<<'\n';
        //continue;
      };
      (*eline)[i] = w1;
    };
    return ok;
  };

  if (com == "set_e0"){
    if (!assigned()) {
      mio<<"Linelist is not assigned\n";
      return bad;
    };
    string en_name;
    istr>>en_name;
    object* o = owner->Obj(en_name);
    if (!o || o->type != "enlist"){
      mio<<"No energy list "<<en_name<<'\n';
      return bad;
    };
    enlist& en = *(enlist*)o;
    vector<double> *e0 = getDoubleNew("e0");
    if (e0 == 0){
      mio<<"Field d:e0 has not been created\n";
      return bad;
    };
    for(size_t i=0;i<size();i++){
      if (!allowed(i)) continue;
      //if (q0(i).assigned())
      (*e0)[i] = en.ener(q0(i));
      //mio<<(*e0)[i]<<'\n';
    };
    return ok;
  };
  
  if (com == "ener") {
    string e0f;
    string en_name,tag;
    istr>>en_name>>e0f>>tag;
    object* o = owner->Obj(en_name);
    if (!o || o->type != "enlist"){
      mio<<"No energy list "<<en_name<<'\n';
      return bad;
    };
    enlist& en = *(enlist*)o;
    if (tag.empty()) make_ener(en,e0f);
    else
      make_ener_tag(en,e0f);
    return ok;
  };

  //  Compares energies calculated from line freuencies and lower state energies
  //  in the field e0f with energies from an enlist en_name.
  //  The differences are written to the file gobo.diff
  if (com == "ener_diff") {
    string e0f;
    string en_name;
    istr>>en_name>>e0f;
    object* o = owner->Obj(en_name);
    if (!o || o->type != "enlist"){
      mio<<"No energy list "<<en_name<<'\n';
      return bad;
    };
    enlist& en = *(enlist*)o;
    vector<double>* e0 = getDouble(e0f);
    size_t j;
    string fn = owner->dir()+"gobo.diff";
    ofstream fil(fn.c_str());
    for(size_t i=0;i<size();i++){
      if (!allowed(i) || !q(i).assigned()) continue;
      j = en.index(q(i));
      if (j == 0 || j >= en.size()) continue;
      fil.precision(12);
      fil<<line(i)<<' ';
      fil<<(line(i)+(*e0)[i]-en.ener(j))<<'\n';
    };
    return ok;
  };

  if (com == "sort") {
     sort();
     renew();
     return repaint;
  };

  if (com == "nextl") {
     size_t j = curr();
     while(next() < size()-1 && !allowed() );
     if (!allowed()) setCurr(j);
     if (pcll && follow) pcll->setCurr(con());
     return repaint;
  };

  if (com == "prevl") {
     size_t j = curr();
     while(prev() > 0 && !allowed());
     if (!allowed()) setCurr(j);
     if (pcll && follow) pcll->setCurr(con());
     return repaint;
  };
  if (com == "add") {
     double w,h;
     size_t is;
     istr >> w >> h >> is;
     if (spectra.size() && spectra[is].type() != spectrum::transmittance)
        h -= spectra[is].bline(w);
     addLine(w,h,&spectra[is]);
     //llists[nl].renew();
     return 1;
  };
  if (com == "add") {
     double w,h;
     istr >> w >> h;
     addLine(w,h);
     return repaint;
  };
  if (com == "delete") {
     size_t i;
     istr >> i;
     if (i >= size()) i = curr();
     deleteLine(i);
     return repaint;
  };

  if (com == "exp") {
    string what;
    istr >> what;
    if (what == "T") {
      double tmp;
      istr >> tmp;
      exp_cond.T(tmp);
    };
    if (what == "L") {
      double tmp;
      istr >> tmp;
      exp_cond.L(tmp);
    };
    if (what == "mix") {
      double prs;
      istr >> prs;
      exp_cond.addMix(prs);
    };
    if (what == "iso") {
      int im,Iso;
      istr >> im >> Iso;
      if (im < exp_cond.nmix())
        exp_cond.addIso(im,Iso);
    };
    return ok;
  };
  
  //  replaces upper state labels according to enlist en_name
  //  en_name must contain vjkg:q1 with new labels
  //  vjkg:q1 can be made with enlist.cmp_q 
  if (com == "change_q"){
    string en_name;
    istr>>en_name;
    object* o = owner->Obj(en_name);
    if (!o || o->type != "enlist"){
      mio<<"No energy list "<<en_name<<'\n';
      return bad;
    };
    enlist& en = *(enlist*)o;
    db_field<VJKG>* fp = en.getField<VJKG>("vjkg","q1");
    if (!fp){
      mio<<"No q1 in "<<en_name<<'\n';
      return bad;
    };
    vector<VJKG>* qp = &(fp->data);
    size_t j;
    for (size_t i=0;i<size();i++){
      if (!q(i).assigned()) continue;
      j = en.index(q(i));
      if (j >= en.size()) continue;
      q(i) = (*qp)[j];
      ;
    };
    return ok;
  };
  
  if (com == "style") {
     string value;
     istr >> value;
     if (value == "zero") style =  splist::FROM_ZERO;
     else if (value == "top") style =  splist::FROM_TOP;
     else if (value == "whole") style =  splist::WHOLE;
     else if (value == "no") style =  splist::NONE;
     return repaint;
  };

  if (com == "noise_f"){
    noise_f(true);
    return ok;
  };

  if (com == "width_f"){
    noise_f(false);
    return ok;
  };



  if (com == "der"){
    lparams().fit_t(lineparams::DER);
    return ok;
  };

  if (com == "dud"){
    lparams().fit_t(lineparams::DUD);
    return ok;
  };

  if (com == "reg"){
    lparams().fit_t(lineparams::REG);
    return ok;
  };

  if (com == "setp"){
    size_t i,j;
    double val;
    istr >> i>> j>> val;
    erase(i,i);
    lparams().par(i,j) = val;
    renew(i,i);
    return repaint;
  };

  if (com == "set"){
    size_t i;
    string name;
    double val;
    istr >> i>> name>> val;
    erase(i,i);
    lparams().set(i,name,val);
    renew(i,i);
    return repaint;
  };

  if (com == "unfit"){
    unfit();
    return repaint;
  };

  if (com == "fixall"){
    string ff;
    istr >> ff;
    mio<<ff<<'\n';
    if (ff.size()<lparams().size()) return 0;
    for(size_t i=0;i<size();i++){
      if (!allowed(i)) continue;
      for(size_t j=0;j<lparams().size();j++){
        if (ff[j] != '0') lparams().fix(i,j);
        else{
          if (lparams().fixed(i,j)) lparams().unfix(i,j);
        };
      };
    };
    return repaint;
  };

  if (com == "ew") {
    if (!size()) return 0;
    size_t j = curr();
    for(size_t i=0;i<spectra.size();i++)
      if (spectra[i].exp_sp){
        if (!allowed(i)) continue;
        mio<<"In spectrum "<<i<<" EW["<<j<<"]="<<lparams().EW(j,spectra[i].exp_sp)<<'\n';
      };
    return ok;
  };

  if (com == "EW") {
    vector<double>& ew = *getDoubleNew("EW");
    for(size_t i=0;i<size();i++){
      if (!allowed(i)) continue;
      ew[i] = lparams().EW(i);
    };
    return ok;
  };

  if (com == "def") {
     string value;
     double v;
     istr >> value >> v;
     lparams().set_default(value,v);
     return ok;
  };

  if (com == "deffix") {
     string value;
     istr >> value;
     lparams().def_fix(value);
     return ok;
  };

  if (com == "fit_vis") {
     if (!fit_vis) fit_vis = true;
     else{
       fit_vis = false;
       for(size_t i=0;i<spectra.size();i++)
         spectra[i].fit = true;
     };
     return ok;
  };
  
  if (com == "fit") {
     size_t n = size();
     size_t i1=n,i2=n;
     istr >> i1 >> i2;
     if (i1 >= n) i1 = curr();
     if (i2 >= n) i2 = i1;
     fit(i1,i2);
     //llists[nl].renew();
     return repaint;
  };

  if (com == "erase") {
     size_t i1,i2;
     istr >> i1 >> i2;
     erase(i1,i2);
     return repaint;
  };

  if (com == "renew") {
     renew();
     return repaint;
  };

  if (com == "spectrum") {
     string what,spname;
     istr >> spname >> what;
     if (what == "del") {
       delSpectrum(spname);
       return repaint;
     }else
       if (!setSpectrum(spname)) return bad;
     spectrum *s = &spectra[spectra.size()-1];
     if (!what.empty()) s->cmd("show "+what);
     //s->color(rgb(0,255,0));
     //s->icolor(rgb(255,0,255));
     return repaint;
  };

  if (com == "fix") {
     size_t i,j;
     istr >> i >> j;
     lparams().fix(i,j);
     return repaint;
  };

  if (com == "unfix") {
     size_t i,j;
     istr >> i >> j;
     lparams().unfix(i,j);
     return repaint;
  };

  return spbase::cmd(str);
}
/*
cmd_res splist::set_lines(string s){
  istringstream istr(s);
  string par,value;
  size_t nl,nlc;
  istr >> nl >> par;
  if (!llists.size()) return 0;
  splist& ll = llists[nl];
  nlc = nl;
  if (ll.connected())
  for(size_t i=0;i<llists.size();i++)
    if (i!=nl && ll.connectedTo(llists[i])){
      nlc = i;
      break;
    };
  string rest;
  rest = s.substr(s.find(par)+par.size());





  return ll.cmd(par+" "+rest);
}
*/

void splist::residual(size_t j){
  if (j >= spectra.size()) return;
  spectrum* s = &spectra[j];
  if (!s->exp_sp) return;
  for(size_t i=0;i<s->nop();i++)
    s->set(i,s->r[i],s->bline(s->w(i)) + s->exp_sp->weight(i)*(s->exp_sp->r[i] - s->r[i]));
};

cmd_res splist::message(object* s,string m){
  cmd_res ok,bad(false);
  string msg,rest;
  istringstream istr(m);
  istr>>msg;
  rest = m.substr(m.find(msg)+msg.size());
  
  if (msg == "bl_ch"){
    if (s->type != "spectrum") return bad;
    for(size_t i=0;i<spectra.size();i++)
      if (spectra[i].exp_sp == s){
        spectra[i].sub(spectra[i].bline);
        spectra[i].bline.copy(spectra[i].exp_sp->bline);
        spectra[i].add(spectra[i].bline);
        residual(i);
        return ok;
      };
    return bad;
  };
  
  if (msg == "renew"){
    if (s->type != "spectrum") return bad;
    for(size_t i=0;i<spectra.size();i++)
      if (spectra[i].exp_sp == s){
        renew();
        return ok;
      };
    return bad;
  };
  
  return object::message(s,m);
}

void splist::copy(splist& ll){
  spbase::copy(ll);
  setParamType(params["par"]);
}

double splist::QS(double w, double h,double w1, double h1){
  double dW,eH;
  dW = fabs(w-w1)/find_dw;
  if (dW > 1.){
    mio<<"dW > find_dw!\n";
    mio<<w<<" - "<<w1<<'\n';
    return 0;//exit(-1);
  };
  eH = h/h1;
  if (eH == 0.) eH = find_eh;
  if (eH > 1.) {
    eH = 1./eH;
  };
  eH = 1. - eH;
  eH /= (1. - 1./find_eh);
  if (dW >= 1. || eH >= 1.) return 0.;
  else return 1. - (dW + eH)/2;
}

void splist::connect(splist& ll,string height,string height1){
  cnnct = getIntNew("cnnct");
  if (!cnnct){
    mio<<"Cannot connect. Memory problems?..."<<'\n';
    return;
  };
  vector<double> *pline = getDouble("line");
  if (!pline){
    mio<<"Cannot connect. No frequencies in list "<<name<<'\n';
    return;
  };
  vector<double> *pline1 = ll.getDouble("line");
  if (!pline1){
    mio<<"Cannot connect. No frequencies in list "<<ll.name<<'\n';
    return;
  };
  vector<double> *phei = getDouble(height);
  if (!phei){
    mio<<"Cannot connect. No field d:"<<height<<" in list "<<name<<'\n';
    return;
  };
  vector<double> *phei1;
  string height2 = (height1.empty())?height:height1;
  phei1 = ll.getDouble(height2);
  if (!phei1){
    mio<<"Cannot connect. No field d:"<<height2<<" in list "<<ll.name<<'\n';
    return;
  };
  vector<double>& line = *pline;
  vector<double>& line1 = *pline1;
  vector<double>& hei = *phei;
  vector<double>& hei1 = *phei1;
  vector<int>& c = *cnnct;
  matrix a(find_max,find_max);
  size_t i0,i1,j0,j1;
  int j_max;
  double qs_max;

  mio<<"Connecting..."<<'\n';
  for(size_t i=0;i<c.size();i++) c[i] = -1;


  double w0,w1,dw=find_dw*0.5;
  i0 = j0 = 0;
  for(w0=line[0];w0<line[size()-1];w0+=dw){
    w1 = w0 + find_dw;
    //mio<<w0<<'-'<<w1<<' ';

    for(;i0<size();i0++)
      if (line[i0]>=w0) break;
    if (i0 == size()) i0--;

    for(i1=i0;i1<size();i1++)
      if (line[i1]>w1) {if (i1) i1--;break;};
    if (i1 == size()) i1--;

    for(;j0<ll.size();j0++)
      if (line1[j0]>=w0) break;
    if (j0 == ll.size()) j0--;

    for(j1=j0;j1<ll.size();j1++)
      if (line1[j1]>w1) {if (j1) j1--;break;};
    if (j1 == ll.size()) j1--;

    if (i1<i0 || j1<j0) continue;
    if (i1==i0 && (line[i0]<w0 || line[i0]>w1)) continue;
    if (j1==j0 && (line1[j0]<w0 || line1[j0]>w1)) continue;

    if (i1-i0+1 >  find_max || j1-j0+1 >  find_max) {
      if (i1-i0+1 > j1-j0+1) find_max = i1-i0+1;
      else
        find_max = j1-j0+1;
      a.make(find_max,find_max);
    };
    //mio<<size()<<"----------------------"<<ll.size()<<'\n';
    for(size_t i=i0;i<=i1;i++){
      //mio<<i<<": ";
      for(size_t j=j0;j<=j1;j++){
        a(i-i0,j-j0) = QS(line[i],hei[i],line1[j],hei1[j]);
        //mio<<a(i-i0,j-j0)<<' ';
      };
      //mio<<'\n';
    };

    for(size_t i=0;i<=i1-i0;i++){
      for(size_t k=0;k<=j1-j0;k++){
        qs_max = 0.;j_max = -1;
        // looking for a maximum in the row
        for(size_t j=0;j<=j1-j0;j++) {
        if (a(i,j) > qs_max){
          qs_max = a(i,j);
          j_max = j;
        };};
        if (j_max < 0) break;// if all 0 -- no matches
        // check corresponding column if there is bigger value
        // if so throw smaller one away and take another try
        for(size_t l=i+1;l<=i1-i0;l++) {
          if (a(l,j_max) > qs_max){a(i,j_max) = 0.;break;}
          else {a(l,j_max) = 0.;};
        };
        if (a(i,j_max) > 0.){// if max is found connect lines and jump for the next
          c[i+i0] = j_max+j0;
          break;
        };
      };
      //mio<<i+i0<<'-'<<c[i+i0]<<' '<<find_max<<'\n';
    }; // ---- i ----
  };
  params["connect"] = ll.name;
  ostringstream ostr;
  ostr<<ll.size();
  params["consize"] = ostr.str();
}

void splist::reconnect(splist& ll){
  cnnct = getInt("cnnct");
  if (!cnnct || !connectedTo(ll)){
    mio<<"Linelists are not connected."<<'\n';
    disconnect();
  };
}

void splist::connect_q(splist& ll){
  cnnct = getIntNew("cnnct");
  if (!cnnct){
    mio<<"Cannot connect. Memory problems?..."<<'\n';
    return;
  };
  vector<double> *pline = getDouble("line");
  if (!pline){
    mio<<"Cannot connect. No frequencies in list "<<name<<'\n';
    return;
  };
  vector<double> *pline1 = ll.getDouble("line");
  if (!pline1){
    mio<<"Cannot connect. No frequencies in list "<<ll.name<<'\n';
    return;
  };
  if (!assigned()) {
    mio<<"Linelist "<<name<<" is not assigned\n";
    return;
  };
  if (!ll.assigned()) {
    mio<<"Linelist "<<ll.name<<" is not assigned\n";
    return;
  };
  vector<double>& line = *pline;
  vector<double>& line1 = *pline1;
  vector<int>& c = *cnnct;

  mio<<"Connecting..."<<'\n';
  for(size_t i=0;i<c.size();i++) c[i] = -1;


  double w0,w1;
  int j0=0;

  for(int i=0;i<size();i++){
    w0 = line[i];
    for(int j=j0;j<ll.size();j++){
      w1 = line1[j];
      if (w0 - w1 > find_dw) j0++;
      if (w1 - w0 > find_dw) break;
      if (q(i) == ll.q(j) && q0(i) == ll.q0(j)) {
         c[i] = j;
         break;
      };
    };
  };
  params["connect"] = ll.name;
  ostringstream ostr;
  ostr<<ll.size();
  params["consize"] = ostr.str();
}

//  connects only allowed lines
void splist::connect_q0(splist& ll,string ll_hei){
  cnnct = getIntNew("cnnct");
  if (!cnnct){
    mio<<"Cannot connect. Memory problems?..."<<'\n';
    return;
  };
  vector<double> *pline = getDouble("line");
  vector<double> *pheight = getDouble(ll_hei);
  if (!pline || !pheight){
    mio<<"Cannot connect "<<name<<'\n';
    if (!pline) mio<<"No lines\n";
    if (!pheight) mio<<"No intensities\n";
    return;
  };
  vector<double> *pline1 = ll.getDouble("line");
  vector<double> *pheight1 = ll.getDouble(ll_hei);
  if (!pline1 || !pheight1){
    mio<<"Cannot connect to "<<ll.name<<'\n';
    if (!pline1) mio<<"No lines\n";
    if (!pheight1) mio<<"No intensities\n";
    return;
  };
  if (!assigned()) {
    mio<<"Linelist "<<name<<" is not assigned\n";
    return;
  };
  if (!ll.assigned()) {
    mio<<"Linelist "<<ll.name<<" is not assigned\n";
    return;
  };
  
  vector<double>& line = *pline;
  vector<double>& hi = *pheight;
  vector<double>& line1 = *pline1;
  vector<double>& hi1 = *pheight1;
  vector<int>& c = *cnnct;
//  mio<<"OK\n";return;

  mio<<"Connecting..."<<'\n';

  double w0,w1,h0,h1,qs,qsj;
  int j0=0,jj;

  for(int i=0;i<size();i++){
    if (!allowed(i)) continue;
    w0 = line[i];
    h0 = hi[i];
    jj = -1;
    qsj = 0.;
    for(int j=j0;j<ll.size();j++){
      w1 = line1[j];
      h1 = hi1[j];
      if (w0 - w1 > find_dw) {j0++;continue;};
      if (w1 - w0 > find_dw) break;
      if (q0(i) == ll.q0(j)) {
        qs = QS(w0,h0,w1,h1);
        if (jj < 0) {jj = j; qsj = qs;}
        else if ( qs > qsj ){ jj = j; qsj = qs;};
      };
    };
    c[i] = jj;
  };
  params["connect"] = ll.name;
  ostringstream ostr;
  ostr<<ll.size();
  params["consize"] = ostr.str();
}


void splist::disconnect(){
  deleteField("i","cnnct");
  cnnct = 0;
  params["connect"] = "";
  params["consize"] = "";
}

bool splist::connectedTo(splist& ll){
  return cnnct && params["connect"]==ll.name &&
    atoi(params["consize"].c_str())==ll.size();
}

double splist::line(size_t i){
  return lparams().line(i);
}

double splist::height(size_t i){
  return lparams().height(i);
}

void splist::height(size_t i,double h){
  lparams().set_height(i,h);
}

bool splist::assignment(){
  if (assigned()) return true;
  db_field<VJKG>* qf = getField<VJKG>("vjkg","q");
  q_p = qf?&qf->data:0;
  qf = getField<VJKG>("vjkg","q0");
  q0_p = qf?&qf->data:0;
  if (q_p == 0 || q0_p == 0) return false;
  return true;
}

void splist::assign(size_t i,string str){
  if (i>=size()) return;
  if (!assignment()){
    mio<<"Cannot allocate memory !"<<'\n';
    return;
  };
  size_t j_ = str.find('-');
  string sup,slo;
  sup = str.substr(0,j_);
  if (j_ != string::npos) slo = str.substr(j_+1);
  else
    slo = "";
  if (!sup.empty()) q(i).assign(sup);
  if (!slo.empty()) {
    q0(i).iso(q(i).iso());
    q0(i).assign(slo);
  };
}


// 0 < h < 1
bool splist::isblend(size_t i,double h,blend_struct *b){
  if (i >= size()) return false;
  size_t i1=0,i2;
  if (i) {
    for(i1=i-1;i1<size();i1--) if (line(i1) != line(i)) break;
    if (i1 < size()) i1++;
    else i1 = 0;
  };
  for(i2=i+1;i2<size();i2++) if (line(i2) != line(i)) break;
  if (i2 < size()) i2--;
  else i2 = size()-1;
  if (i1 == i2) return false;
  if (h == 1. && b == 0) return true;
  double hm = 0.,hmax = 0.,hh;
  size_t jm = i1;
  for(size_t j=i1;j<=i2;j++) {
    hh = height(j);
    hm += hh;
    if (hh > hmax) {
      hmax = hh;
      jm = j;
    };
  };
  if (b) *b = blend_struct(i1,i2,jm);
  if (height(i) < h*hm) return true;
  return false;
}

struct line_error_struct{
  double e;
  double h;
  size_t i;
  line_error_struct():e(),h(),i(){};
  line_error_struct(double ee,double hh,size_t ii):e(ee),h(hh),i(ii){};
};

// field e0f must contain lower energies
// h - minimum fraction of a blended line intensity
//     to be included in analysis
void splist::line_error(string e0f,double h){
  vector<double> *e0p = getDouble(e0f);
  if (!e0p) {
    mio<<"Cannot find field "<<e0f<<'\n';
    return;
  };
  vector<double>& e0 = *e0p;
  vector<double>& err = (getFieldNew<double>("d","lerr")->data);
  map<VJKG,vector<line_error_struct> > en;
  map<VJKG,vector<line_error_struct> >::iterator e;
  vector<line_error_struct>::iterator v;
  vector<line_error_struct> *en0;
  double hmin = 1e30, hmax = 0.,hh,eav;
  map<double,double> herr;
  map<double,double>::iterator ih,ih1;
  for(size_t i=0;i<size();i++)
    if (!isblend(i,h) && q(i).assigned()){
       hh = height(i);
       en0 = &en[q(i)];
       for(v = en0->begin();v != en0->end(); v++){
         if (q0(i) == q0(v->i)){
            mio<<"Double assignment "<<i<<" & "<<v->i<<"   ";
            mio<<q(i)<<" - "<<q0(i)<<'\n';
            continue;
          };
       };
       en[q(i)].push_back( line_error_struct(line(i) + e0[i],hh,i) );
       if (hh > hmax) hmax = hh;
       if (hh < hmin) hmin = hh;
    };
  for(e=en.begin();e!=en.end();e++){
    if (e->second.size() < 3) continue;
    eav = 0.;
    for(v=e->second.begin();v!=e->second.end();v++){
      eav += v->e;
    };
    eav /= e->second.size();
    for(v=e->second.begin();v!=e->second.end();v++){
      if (fabs(eav - v->e) > 1.) continue;
      err[v->i] = fabs(eav - v->e);
      herr[log(v->h)] = err[v->i];
    };
  };
  string fn = owner->dir()+"tmp.txt";
  ofstream fil(fn.c_str());
  for(ih=herr.begin();ih!=herr.end();ih++){
    //if (ih->second > 1.) continue;
    fil<<ih->first<<' '<<ih->second<<'\n';
  };
  eav = 0.;
  size_t i=0,n=300,j,n90 = n*8/10;
  map<double,int> hist;
  for(ih=herr.begin();i<n && ih!=herr.end();ih++,i++){
    hist[ih->second] = i;
  };
  mio<<"hist.size="<<hist.size()<<'\n';
  if (hist.size() < n-1){
    mio<<"Not enough points to calculate uncertanties\n";
    return;
  };
  fn = owner->dir()+"tmp1.txt";
  ofstream fil1(fn.c_str());
  ih1=herr.begin();
  for(;i<herr.size();i++){
    j = 0;
    for(map<double,int>::iterator hi=hist.begin();hi!=hist.end();hi++,j++){
      if (j == n90){
        hist.erase(ih1->second);
        ih1->second = hi->first;
        break;
      };
    };
    ih++;
    ih1++;
    hist[ih->second] = i;
  };

  ih1=herr.end();
  for(i=0;i<=n;i++)ih1--;
  ih=herr.end();
  for(i=0;i<n/2;i++){
    ih->second = ih1->second;
    ih--;
  };
  for(;ih1!=herr.begin();ih1--,ih--) ih->second = ih1->second;

  for(ih=herr.begin();ih!=herr.end();ih++){
    if (ih->second > 1.) continue;
    fil1<<ih->first<<' '<<ih->second<<' '<<hist.size()<<'\n';
  };

}
fun_res splist::fun(string str){
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
    ostr << size();
    return ostr.str();
  };
  
  if (nam == "find_dW"){
    ostr << find_dw;
    return ostr.str();
  };
  
  if (nam == "find_eH"){
    ostr << find_eh;
    return ostr.str();
  };
  
  if (nam == "bounds"){
    if (!size()) return string("0 0 0 0");
    double vMax=0,uMin,uMax,du;
    uMin = line(0);
    uMax = line(size()-1);
    du = (uMax-uMin)/20;
    for(size_t i=0;i<size();i++)
      if (height(i) > vMax) vMax = height(i);
    if (vMax==0) vMax = 1.;
    ostr<<uMin-du<<' '<<uMax+du<<' '<<0<<' '<<vMax;
    return ostr.str();
  };
  
  return spbase::fun(str);			      
}

int splist::addLine(double w,double h,spectrum *sp){
  size_t j = size();
  for(size_t i=0;i<size();i++)
     if (w < line(i)) {
        j = i;
        break;
     };
  if (j == size()) addRow();
  else
    insertRow(j);
  setCurr(j);
  lparams().new_line(j,w,h,sp);
  renew(j,j);
  return j;
}

void splist::deleteLine(size_t i){
  size_t j=size();
  if (i>0 && line(i) == line(i-1)) j = i-1;
  else if (i<size()-1 && line(i) == line(i+1)) j = i+1;
  if (j < size()){
    height(j,height(j) + height(i));
  };
  erase(i,i);
  eraseRow(i);
}



//     --------------------------   GRAPHICS  --------------------------------


splist* splist::getConnected(){
  string conn_name = params["connect"];
  obj_list<splist> llist(owner,"splist");
  for(size_t i=0;i<llist.size();i++)
    if (llist[i].name == conn_name) return llist(i);
  return 0;
}

//bool splist::draw(canvas& c){
//  if (c.type == "graph"){
//   graphics& g = *(graphics*)(&c);
//   g.clip_clientRect();
//   double w,h;
//   size_t i0=0,i1=0;
//   for(size_t i=0;i<size();i++) {
//     w = line(i);
//     h = height(i);
//     ++i1;
//     if ( w < g.uMin() ) {
//       ++i0;
//       continue;
//     };
//     g.pen(color);
//     if ( w > g.uMax() ) break;
//     if (!allowed(i)) continue;
//     switch (style){
//       case FROM_ZERO: {
//               g.moveTo( g.X(w),g.Y(0.) );
//               g.lineTo( g.X(w),g.Y(h) );
//               break;
//             };
//       case FROM_TOP: {
//               if (h > g.vMax()) h = g.vMax()-(g.vMax()-g.vMin())/10;
//               g.moveTo( g.X(w),g.Y(g.vMax()) );
//               g.lineTo( g.X(w),g.Y(h) );
//               break;
//             };
//       case WHOLE: {
//               g.moveTo( g.X(w),g.Y(g.vMax()) );
//               g.lineTo( g.X(w),g.Y(g.vMin()) );
//               break;
//             };
//     };
//   };
//   i1 -= 2;
//
//   if ( g.has_focus() ){
//     if ( curr() < i0 ) setCurr(i0);
//     if ( curr() > i1 ) setCurr(i1);
//   };
//   w = lparams().line(curr());
//   h = lparams().height(curr());
//
//   g.pen(cur_color);
//     switch (style){
//       case FROM_ZERO: {
//               g.moveTo( g.X(w),g.Y(0.) );
//               g.lineTo( g.X(w),g.Y(h) );
//               g.moveTo( g.X(w),g.Y(g.vMax()) );
//               g.lineTo( g.X(w),g.Y(h)-5 );
//               break;
//             };
//       case NONE:
//       case FROM_TOP: {
//               if (h > g.vMax()) h = g.vMax()-(g.vMax()-g.vMin())/10;
//               g.moveTo( g.X(w),g.Y(g.vMax()) );
//               g.lineTo( g.X(w),g.Y(h) );
//               break;
//             };
//       case WHOLE: {
//               g.moveTo( g.X(w),g.Y(g.vMax()) );
//               g.lineTo( g.X(w),g.Y(g.vMin()) );
//               break;
//             };
//     };
//     for(size_t i=0;i<spectra.size();i++){
//        if (spectra[i].cname(g.name)){
//          spectra[i].draw(*(canvas*)&g);
//        };
//     };
//   if (focused()){// print line parameters and assignment
//     if (mouse_add_line){
//       g.pen(rgb(255,0,0));
//       g.text(5,0,"Add line");
//     };
//     g.clip(rect(0,g.client_height(),g.client_width(),g.client_height()+30));
//     ostringstream ostr;
//     ostr<<lparams().print(curr());
//     if (assigned()){
//        ostr<<' '<<q(curr())<<" - "<<q0(curr());
//     };
//     g.text(100,g.client_height(),ostr.str());
//     if (connected()){
//       splist* cll = getConnected();
//       if (cll && con()>=0) {
//         ostringstream ostr1;
//         ostr1<<cll->lparams().print(con());
//         if (cll->assigned()){
//            ostr1<<' '<<cll->q(con())<<" - "<<cll->q0(con());
//         };
//         g.text(100,g.client_height()+10,ostr1.str());
//       };
//     };
//   };
//   return true;
//  };
//  return spbase::draw(c);
//}
void splist::setParamType(string s){
  lineparams *new_lp = new_lineparams(s,*this);
  if (new_lp) {
    delete lp;
    lp = new_lp;
    params["par"] = s;
    return;
  }else{
    mio<<"params not created!\n";
  };
}

bool splist::setSpectrum(string sn){
  obj_list<spectrum> sps(owner,"spectrum");
  spectrum* spp = sps(sn);
  if (!spp){
    mio<<"Cannot find spectrum "<<sn<<'\n';
    return false;
  };
  spectrum *s = 0;
  for(size_t i=0;i<spectra.size();i++)
    if (spectra[i].exp_sp == spp){
      s = &spectra[i];
      break;
    };
  if (!s) s = &spectra.add();
  s->make(*spp);
  s->exp_sp = spp;
  s->owner = owner;
  renew();
  return true;
}

bool splist::delSpectrum(string sn){
  obj_list<spectrum> sps(owner,"spectrum");
  spectrum* spp = sps(sn);
  if (!spp){
    mio<<"Cannot find spectrum "<<sn<<'\n';
    return false;
  };
  for(size_t j=0;j<spectra.size();j++)
     if (spectra[j].exp_sp == spp) {
       spectra.erase(j);
       break;
     };
  return true;
}

bool splist::setSpectrum(){
  spectrum& s = spectra.add();
  if ( size() < 3 ) return false;
  s.make(2*lparams().line(0)-lparams().line(2),2*lparams().line(size()-1)-lparams().line(size()-3) ,0.01);
  s.owner = owner;
  renew();
  return true;
}

void splist::renew(){
  spectrum *s;
  for(size_t is = 0; is < spectra.size();is++){
      s = &spectra[is];
      if (!s->calcLines(lparams(),s->exp_sp)) continue;
      if (s->nop() != s->exp_sp->nop()){
        mio.error("Calculated and experimental("+s->exp_sp->name+
                ") spectra have different sizes!");
      }else{
        s->complex();
        for(size_t i=0;i<s->nop();i++)
           s->set(i,s->r[i],s->bline(s->w(i)) + s->exp_sp->weight(i)*(s->exp_sp->r[i] - s->r[i]));
      };
  };
}

void splist::erase(size_t i1,size_t i2){
  for(size_t is = 0; is < spectra.size();is++){
      spectra[is].calcLines(lparams(),i1,i2,spectra[is].exp_sp,-1);
  };
}

void splist::renew(size_t i1,size_t i2){
  spectrum *s;
  for(size_t is = 0; is < spectra.size();is++){
      s = &spectra[is];
      if (!s->calcLines(lparams(),i1,i2,s->exp_sp,1)) continue;
      s->complex();
      for(size_t i=0;i<s->nop();i++)
         s->set(i,s->r[i],s->bline(s->w(i)) + s->exp_sp->weight(i)*(s->exp_sp->r[i] - s->r[i]));
  };
}

double splist::fit(size_t i1,size_t i2,bool out){
//  ofstream tst_y("tst.y");
  p.clear();
  p0.clear();
  dp.clear();
  ip.clear();
  err.clear();
  names.clear();
  calc.clear();
  bl.clear();
  if (!spectra.size()) return 0.;
  //if (fit_vis){
  //   for(size_t i=0;i<spectra.size();i++){
  //       spectra[i].fit = spectra[i].cname(owner->plt.focname)?true:false;
  //       mio<<owner->plt.focname<<' '<<spectra[i].cname(owner->plt.focname)<<'\n';
  //   };
  //};

  if (out) {
    mio<<"Fitting lines "<<i1<<" - "<<i2<<'\n';
    mio<<(noise_f()?"Noise ":"Width ")<<"fit\n";
  };
  fit_i1 = i1;
  fit_i2 = i2;

  spectrum *s,*exps;

  int iw1,iw2;
  size_t ic;
  double w1,w2,dw;
  ic=0;
  ny = 0;
  for(size_t i=0;i<spectra.size();i++)
     if (spectra[i].exp_sp && spectra[i].fit) {
       exps = spectra[i].exp_sp;
       dw = spectra[i].delw();
       iw1 = exps->index(lparams().line(i1));
       w1 = exps->w(iw1);
       iw2 = exps->index(lparams().line(i2));
       w2 = exps->w(iw2);
       bool skip = false;
       for(int ii=i1;ii<=i2;ii++)
           if (exps->weight(ii) < 0){
             skip = true;
             break;
           };
       if (skip){
          if (out) mio<<"Skipping "<<exps->name<<' '<<w1<<' '<<w2<<'\n';
          continue;
       };
       for(;w1 > spectra[i].wstart() && iw1>0;w1 -= dw,iw1--) {
          if (noise_f()){if (spectra[i].exp_sp->val(lparams(),i1,i2,w1) < spectra[i].noise()) break;}
          else
            if (w1 < lparams().line(i1) - spectra[i].exp_sp->lwidth()) break;
       };
       for(;w2 < spectra[i].wstop();w2 += dw){
          if (noise_f()){if (spectra[i].exp_sp->val(lparams(),i1,i2,w2) < spectra[i].noise()) break;}
          else
            if (w2 > lparams().line(i2) + spectra[i].exp_sp->lwidth()) break;
       };
       if (w2 - w1 <= 0) continue;
       s = &calc.add();
       s->copy(*spectra[i].exp_sp,w1,w2);
       bl.push_back(&spectra[i].exp_sp->bline);
       s->exp_sp = spectra[i].exp_sp;
       s->fit = spectra[i].fit;
       if (s->afunQ()){
         s->mark(); 
         s->fft(); s->fft(-1);
         s->recenter();
       };
       if (out) mio<<"Spectrum "<<calc.size()-1<<" w1="<<w1<<" w2="<<w2<<" nop="<<s->nop()<<" type "<<s->type()<<'\n';
       //if (out) mio<<"wc="<<s->wavcorr()<<" dw="<<s->delw()<<' '<<dw<<'\n';
       ny += s->nop();
       ++ic;
     };

  ns = calc.size(); // number of spectra

  for(size_t i=i1;i<=i2;i++){
    for(size_t j=0;j<lparams().size();j++){
       if (!lparams().fixed(i,j)) {
         p.push_back(&lparams().par(i,j));
         p0.push_back(lparams().par(i,j));
         dp.push_back(lparams().dp(i,j));
         ip.push_back(i);
         if (lparams().error()) err.push_back(&lparams().err(i,j));
         names.push_back("Line "+strInt(i)+' '+lparams().names[j]);
       }else if (lparams().error()) lparams().err(i,j) = 0.;
    };
  };
  for(ic=0;ic<ns;ic++){
    if ((*bl[ic]).fixed()) continue;
    for(size_t i=0;i<(*bl[ic]).size();i++)
    if ((*bl[ic]).v[i].x >= calc[ic].wstart() && (*bl[ic]).v[i].x <= calc[ic].wstop()){
      for(size_t j=0;j<(*bl[ic]).psize();j++)
        if (!(*bl[ic]).fixed(i,j)){
          p.push_back(&((*bl[ic]).par(i,j)));
          p0.push_back((*bl[ic]).par(i,j));
          dp.push_back((*bl[ic]).dp(j));
          ip.push_back(-1);
        };
    };
  };


  np = p.size();
  y.resize(ny);
  f.resize(ny);
  weig.resize(ny);

  size_t iy = 0;
  ic = 0;
  spectrum s1;
  obj_list<spectrum>  sps(owner,"spectrum");
  for(ic=0;ic<calc.size();ic++){
     if (!sps.has(calc[ic].exp_sp)){
       mio<<"Cannot find exp. spectrum\n";
       return 0.;
     };
     mio<<calc[ic].exp_sp->name<<"  ok ";
     if (calc[ic].exp_sp && calc[ic].fit) {
       mio<<calc[ic].exp_sp->name<<"  OK"<<'\n';
       s1.make(calc[ic]);
       s1.calcLines(lparams(),0,i1-1,i2+1,size()-1,calc[ic].exp_sp);
       s1.sub(calc[ic].exp_sp->bline);
       iw1 = calc[ic].exp_sp->index(s1.wstart());
       for(size_t i=0;i<calc[ic].nop();i++,iy++) {
           y[iy] = calc[ic].rr(i) - s1.rr(i);
           weig[iy] = (iw1+i<calc[ic].exp_sp->nop())?calc[ic].exp_sp->weight(iw1+i):0.;
//           tst_y.precision(12);
//           tst_y<<calc[ic].w(i)<<' '<<s1.rr(i)<<'\n';
       };
     };
  };

  mio<<"ny="<<ny<<'\n';
  if (ny < np){
    mio<<"fit: Too many parameters\n";
    return 1e30;;
  };

  double ret=0.;
  switch(lparams().fit_t()){
    case lineparams::DER: ret = fit_der(i1,i2,out);break;
    case lineparams::DUD: ret = fit_dud(i1,i2,out);break;
    case lineparams::REG: ret = fit_reg(i1,i2,out);break;
  };
  if (ret >= 0.){
    vector<double> p_new(np);
    for(size_t i=0;i<np;i++) {
      p_new[i] = *p[i];
      *p[i] = p0[i];
    };
    //erase(i1,i2);
    for(size_t i=0;i<np;i++) {
      *p[i] = p_new[i];
    };
    //renew(i1,i2);
    ic = 0;
    size_t i0;
    for(size_t is=0;is<spectra.size();is++){
      s = &spectra[is];
      if (!s->exp_sp || !s->fit) continue;
      for(ic=0;ic<calc.size();ic++){
        if (s->exp_sp != calc[ic].exp_sp) continue;
        calc[ic].calcLines(lparams(),-1,-1,s->exp_sp);
        i0 = s->index(calc[ic].wstart());
        for(size_t i=i0+1;i<i0+calc[ic].nop()-1;i++){
          s->r[i] = calc[ic].rr(s->w(i));
          s->set(i,s->r[i],s->exp_sp->weight(i)*(s->exp_sp->rr(i)-s->r[i]));
        };
      };
    };
  };
  return ret;
}

double splist::fit_dud(size_t i1,size_t i2,bool out){
  int ngood = 0;

  matrix old_p(np,np);    // old parametrs
  for(size_t i=0;i<np;i++){
    for(size_t j=0;j<np;j++) old_p(i,j) = *p[j];
    old_p(i,i) += dp[i];
  };

  matrix old_f(np,ny);
  size_t iy = 0;

  size_t nn;
  nn = ny - np;

  for(size_t ip=0;ip<np;ip++){
    *p[ip] += dp[ip];
    iy = 0;
    for(size_t ic=0;ic<ns;ic++){
      if (!(*bl[ic]).fixed()) (*bl[ic]).refit();
      calc[ic].calcLines(lparams(),i1,i2);
      for(size_t i=0;i<calc[ic].nop();i++) {
        old_f(ip,iy++) = calc[ic].rr(i);
      };
    };
    *p[ip] -= dp[ip];
  };

  if (out) {
    mio<<"--------------------\n";
    for(size_t i=0;i<np;i++)
       mio<<"p("<<i<<")="<<*p[i]<<'\n';
  };
//  calc[0].saveAs("tst_calc");
//  matrix F(np,ny);
  matrix M(np,np);
  matrix IM(np,np);
  vec x(np),new_p(np);
  double sum,sig,sig0,dstep;
  size_t iold,istep;


  // calculate spectra with old params
  iy=0;
  sig0 = 0.;
  for(size_t ic=0;ic<ns;ic++){
    if (!(*bl[ic]).fixed()) (*bl[ic]).refit();
    calc[ic].calcLines(lparams(),i1,i2);
    for(size_t i=0;i<calc[ic].nop();i++,iy++) {
      f[iy] = calc[ic].rr(i);
//             tst_y.precision(12);
//             tst_y<<calc[ic].w(i)<<' '<<y[iy]<<'\n';
      sum = f[iy] - y[iy];
      sig0 += sum*sum;
    };
  };
  if (ny > np)  sig0 /= (ny-np);

  if (out) mio<<"sigma0="<<sig0<<" np="<<np<<" ns="<<ns<<'\n';

  for(size_t iter=0;iter<50;iter++){

     for(size_t k=0;k<np;k++){
        sum = 0;
        for(size_t i=0;i<ny;i++) sum += -(old_f(k,i)- f[i])*(f[i] - y[i])*weig[i];
        x(k) = sum;
        for(size_t j=0;j<np;j++){
           sum = 0;
           for(size_t i=0;i<ny;i++) sum += (old_f(k,i) - f[i])*(old_f(j,i) - f[i])*weig[i];
           M(k,j) = sum;
        };
     };

//     for(size_t k=0;k<np;k++){
//       mio.precision(12);
//       mio<<"M "<<old_p(k,k)<<"      "<<M(k,k)<<' '<<x[k]<<'\n';
//     };

     if (lparams().error()){
       M.inverse();
       for(size_t i=0;i<np;i++)
         *err[i] = sqrt(M(i,i));
       M.inverse();
     };


     M.solve(x);

     iold = iter - iter/np*np;
     istep = 0;
     dstep = 0.5;

     for(size_t k=0;k<np;k++){
       old_p(iold,k) = *p[k];
       *p[k] = new_p[k];
     };

     for(size_t i=i1;i<=i2;i++) lparams().limit(i);

     //  step shortening  --------------------------------------------
     shorten:

       // calculate spectra with new params
       iy=0;
       sig = 0.;
//       tst_f<<"-------------------\n";
       for(size_t ic=0;ic<ns;ic++){
         if (!(*bl[ic]).fixed()) (*bl[ic]).refit();
         calc[ic].calcLines(lparams(),i1,i2);
         for(size_t i=0;i<calc[ic].nop();i++,iy++) {
           f[iy] = calc[ic].rr(i);
//             tst_f.precision(12);
//             tst_f<<calc[ic].w(i)<<' '<<f[iy]<<'\n';
           sum = f[iy] - y[iy];
           sig += sum*sum;
         };
       };
       if (ny > np)  sig /= (ny-np);

     if (out) {
       mio<<"\niter "<<iter<<" istep="<<istep<<'\n';
       mio<<"sigma="<<sig<<' '<<sig0<<'\n';
     };
/*
     if (sig > sig0 && istep < 20){
       mio<<sig0<<' '<<sig<<' '<<istep<<'\n';
       for(size_t k=0;k<np;k++) *p[k] = dstep*new_p[k] + (1. - dstep)*old_p(iold,k);
       for(size_t i=i1;i<=i2;i++) lparams().limit(i);
       dstep *= -0.5;
       ++istep;
       goto shorten;
     };
*/      // -----------------------------------------------------------------------

     for(size_t i=0;i<ny;i++) old_f(iold,i) = f[i];

//     for(size_t k=0;k<np;k++){
//       mio.precision(12);
//       mio<<"old "<<old_p(iold,k)<<"     new "<<*p[k]<<"      "<<M(k,k)<<'\n';
//     };

     if (fabs(sig-sig0)/sig < 1e-3) {
       ngood++;
       if (ngood >= 5) break;
     }else if (ngood) ngood--;
     sig0 = sig;


  }; // --- end iter ---
  return sig;

}

double splist::fit_der(size_t i1,size_t i2,bool out){
  ofstream tst_f("tst.f");

  size_t ngood = 0;
  size_t iy = 0;
  size_t nn;
  nn = ny - np;

  if (out) {
    mio<<"--------------------\n";
    for(size_t i=0;i<np;i++) mio<<"p("<<i<<")="<<*p[i]<<" dp("<<i<<")="<<dp[i]<<'\n';
  };

  matrix F(ny,np); // derivatives
  matrix M(np,np); // normal system matrix
  matrix IM(np,np); // matrix inverse to M
  vec x(np),new_p(np); // right hand side vector & corrections to the parameters
  vec erro(np);
  vector<bool> failed(np);
  double sum,sig,sig0,sig00;
  data_container<spectrum> s1;
  for(size_t ic=0;ic<ns;ic++){
    s1.add().make(calc[ic]);
  };


  // calculate spectra with old params
  iy=0;
  sig0 = 0.;
  for(size_t ic=0;ic<ns;ic++){
    calc[ic].calcLines(lparams(),i1,i2,calc[ic].exp_sp);
    for(size_t i=0;i<calc[ic].nop();i++,iy++) {
      f[iy] = calc[ic].rr(i);
//             tst_y.precision(12);
//             tst_y<<calc[ic].w(i)<<' '<<y[iy]<<'\n';
      sum = f[iy] - y[iy];
      sig0 += sum*sum;
    };
  };
  if (ny > np)  sig0 /= (ny-np);
  sig00 = sig0;

  if (out) mio<<"sigma0="<<sig0<<" np="<<np<<" ns="<<ns<<'\n';

  for(size_t iter=0;iter<50;iter++){ // iterations

    again_when_failed:
    for(size_t jp=0;jp<np;jp++){
     if (failed[jp]) continue;
     *p[jp]+= dp[jp];// 0.0001;
     iy=0;
     for(size_t ic=0;ic<ns;ic++){
       if (!(*bl[ic]).fixed()) (*bl[ic]).refit();
       s1[ic].make(calc[ic]);
       s1[ic].calcLines(lparams(),i1,i2,calc[ic].exp_sp);
       for(size_t k=0;k<calc[ic].nop();k++,iy++) {
         F(iy,jp) = (s1[ic].r[k] - calc[ic].r[k])/dp[jp];
//         mio<<"F("<<iy<<','<<jp<<")="<<F(iy,jp)<<'\n';
       };
     };
     *p[jp]-= dp[jp];// 0.0001;
    };

     for(size_t k=0;k<np;k++){
        if (failed[k]){
          x(k) = 0.;
          M(k,k) = 1.;
          continue;
        };
        sum = 0;
        for(size_t i=0;i<ny;i++) if (weig[i]>0) sum += F(i,k)*(y[i] - f[i])*weig[i];
        x(k) = sum;
        for(size_t j=0;j<np;j++){
           sum = 0;
           for(size_t i=0;i<ny;i++) if (weig[i]>0) sum += F(i,k)*F(i,j)*weig[i];
           M(k,j) = sum;
        };
     };

/*     mio<<"MNS:\n";
     for(size_t i=0;i<np;i++){
       for(size_t j=0;j<np;j++)  mio<<M(i,j)<<' ';
       mio<<'\n';
     };
     mio<<'\n';
     for(size_t j=0;j<np;j++)  mio<<x(j)<<' ';
     mio<<'\n';
*/

//     for(size_t j=0;j<np;j++)  mio<<M(j,j)<<' '<<x(j)<<'\n';

     bool failure = false;
     for(size_t i=0;i<np;i++)
     if (!failed[i] && M(i,i) == 0.){
       failure = true;
       failed[i] = true;
       mio<<"Par "<<i<<" failed\n";
     };
     if (failure) goto again_when_failed;
     
     IM.copy(M);
     IM.inverse();
     for(size_t i=0;i<np;i++){
       erro[i] = sqrt(IM(i,i));
//       mio<<sqrt(sig0)*erro[i] <<' '<< dp[i]*1000<<'\n';
       if (sqrt(sig0)*erro[i] > dp[i]*10000){
         erro[i] = 0.;
         x[i] = 0;
         for(size_t j=0;j<np;j++)
           if (i == j) M(i,i) = 1.;
           else{
             M(i,j) = M(j,i) = 0.;
           };
       };
     };

     if (!M.solve(x)){
       mio<<"Fit failed\n";
       return -1.;
     };

//     mio<< "New params:"<<'\n';
     for(size_t k=0;k<np;k++){
//       mio<<*p[k];
       *p[k] += x[k];
//       mio<<' '<<*p[k]<<'\n';
     };

     for(size_t i=i1;i<=i2;i++) lparams().limit(i);

     // calculate spectra with new params
     iy=0;
     sig = 0.;
     for(size_t ic=0;ic<ns;ic++){
       if (!(*bl[ic]).fixed()) (*bl[ic]).refit();
       calc[ic].calcLines(lparams(),i1,i2,calc[ic].exp_sp);
       for(size_t i=0;i<calc[ic].nop();i++,iy++) {
         f[iy] = calc[ic].rr(i);
//                tst_f.precision(12);
//                tst_f<<calc[ic].w(i)<<' '<<f[iy]<<'\n';
         if (weig[iy] < 0.) continue;
         sum = f[iy] - y[iy];
         sig += sum*sum*weig[iy];
       };
     };
     if (ny > np)  sig /= (ny-np);

     double qq = sqrt(sig);
     for(size_t i=0;i<np;i++){
       erro[i] *= qq;
       if (lparams().error())  *err[i] = erro[i];
     };

     if (out) {
       mio<<"\niter "<<iter<<'\n';
       mio<<"sigma="<<sig<<' '<<sig0<<'\n';
     };

     if (sig/sig00 > 100.){
       unfit();
       mio<<"Fit failed\n";
       return -sig00;
     };

     if (fabs(sig-sig0)/sig < 1e-5) {
       ngood++;
       if (ngood >= 5) break;
     }else if (ngood) ngood--;
     sig0 = sig;


  }; // --- end iter ---
  if (out){
    for(size_t i=0;i<names.size();i++){
      mio<<names[i]<<' '<<p0[i]<<' '<<*p[i]<<' '<<erro[i];
      mio<<'\n';
    };
  };

  return sig;

}

double rho(vec& c2,vec& s,double g){
  double res=0.;
  double g2 = g*g;
  double d;
  if (g == 0.) return 0.;
  for(size_t i=0;i<s.size();i++){
    if (s[i] == 0.) continue;
    d = 1. + s[i]/g2;
    res += c2[i]/s[i]/d/d;
  };
  return res;
};

double rho_d(vec& c2,vec& s,double g){
  double res=0.;
  double g2 = g*g;
  double d;
  if (g == 0.) return 0.;
  for(size_t i=0;i<s.size();i++){
    if (s[i] == 0.) continue;
    d = s[i]/g + g;
    res += c2[i]/d/d/d;
  };
  return res;
};

double rho_d2(vec& c2,vec& s,double g){
  double res=0.;
  double g2 = g*g;
  double d;
  if (g == 0.) return 0.;
  for(size_t i=0;i<s.size();i++){
    if (s[i] == 0.) continue;
    d = s[i] + g2;
    d *= d;
    res += c2[i]/d*(s[i]-g2)/d;
  };
  return 12.*res*g2;
};

double eta(vec& c2,vec& s,double g){
  double res=0.;
  double g2 = g*g;
  double d;
  for(size_t i=0;i<s.size();i++){
    if (s[i] == 0.) continue;
    d = s[i] + g2;
    res += c2[i]/s[i]/d;
  };
  return res;
};

double eta_d(vec& c2,vec& s,double g){
  double res=0.;
  double g2 = g*g;
  double d;
  if (g == 0.) return 0.;
  for(size_t i=0;i<s.size();i++){
    if (s[i] == 0.) continue;
    d = s[i] + g2;
    res += c2[i]/s[i]*g/d/d;
  };
  return -2.*res;
};

double eta_d2(vec& c2,vec& s,double g){
  double res=0.;
  double g2 = g*g;
  double d;
  for(size_t i=0;i<s.size();i++){
    if (s[i] == 0.) continue;
    d = s[i] + g2;
    res += c2[i]/s[i]*(s[i]-3*g2)/d/d/d;
  };
  return -2.*res;
};

double kappa(vec& c2,vec& s,double g){
  if (g == 0.) return 0.;
  double r2 = rho_d2(c2,s,g);
  double r1 = rho_d(c2,s,g);
  double e2 = eta_d2(c2,s,g);
  double e1 = eta_d(c2,s,g);
  double g2 = g*g;
  double g4 = g2*g2;
  double d = sqrt(r1*r1+e1*e1);
//  return e*r/ed*(g2*ed*r + 2.*g*e*r + g4*e*ed)/d/d/d;
//  return fabs(r2*e1 - e2*r1)/d/d/d;
  return (r1*r2 + e1*e2)/sqrt((r1*r1 + e1*e1)*(r2*r2 + e2*e2));
}

double kappa1(vec& c2,vec& s,double g){
  if (g == 0.) return 0.;
  double rx1,ry1,rx2,ry2,rx3,ry3,co,dg = 0.0001;
  rx1 = eta(c2,s,g - dg);
  ry1 = rho(c2,s,g - dg);
  rx2 = eta(c2,s,g);
  ry2 = rho(c2,s,g);
  rx3 = eta(c2,s,g + dg);
  ry3 = rho(c2,s,g + dg);
  co = (  (rx1-rx2)*(rx3-rx2) + (ry1-ry2)*(ry3-ry2)  )/
        sqrt((rx1-rx2)*(rx1-rx2) + (ry1-ry2)*(ry1-ry2))/
        sqrt((rx3-rx2)*(rx3-rx2) + (ry3-ry2)*(ry3-ry2));
  co = acos(co);
  co = 1./tan(co/2);
  return co;
}

double splist::fit_reg(size_t i1,size_t i2,bool out){
  ofstream tst_y("tst.y");
  ofstream tst_f("tst.f");

  size_t ngood = 0;
  size_t iy = 0;
  size_t nn;
  nn = ny - np;

  if (out) {
    mio<<"----------- REG ------------\n";
    for(size_t i=0;i<np;i++) mio<<"p("<<i<<")="<<*p[i]<<" dp("<<i<<")="<<dp[i]<<'\n';
  };

  matrix K(ny,np);     // derivatives
  matrix M(np,np);     // normal system matrix
  vec x(np),new_p(np); // corrections to the parameters, new parameters
  vec b(np);           // right hand side vector
  spectrum s1,s2;
  double sum,sig,sig0,sig00;
  valarray<double> sigs(5);
  double g,g0,g1,g2,kap,kap1,kap0,kap2;            // gamma
  vec s(np);               // eigenvalues of M;
//  matrix V(np,np);  // eigenvectors of M
  vec c(np);               // c = b V
  vec c2(np);         // vector of squares of c
  vec pp(np);         //  scaling factors
  vec wp(np);         //

  // calculate spectra with old params
  iy=0;
  sig0 = 0.;
  for(size_t ic=0;ic<ns;ic++){
    calc[ic].calcLines(lparams(),i1,i2,calc[ic].exp_sp);
    for(size_t i=0;i<calc[ic].nop();i++,iy++) {
      f[iy] = calc[ic].rr(i);
//             tst_y.precision(12);
//             tst_y<<calc[ic].w(i)<<' '<<y[iy]<<'\n';
      sum = f[iy] - y[iy];
      sig0 += sum*sum;
    };
  };
  if (ny > np)  sig0 /= (ny-np);
  sig00 = sig0;
  sigs = sig00;

  if (out) mio<<"sigma0="<<sig0<<" np="<<np<<" ns="<<ns<<'\n';

  for(size_t iter=0;iter<50;iter++){ // iterations

     // calculate derivatives K
    for(size_t i=0;i<np;i++){
       if (ip[i]<0){
          *p[i]+= dp[i];// 0.0001;
          iy=0;
          for(size_t ic=0;ic<ns;ic++){
            if (!(*bl[ic]).fixed()) (*bl[ic]).refit();
            s1.make(calc[ic]);
            s1.calcLines(lparams(),i1,i2,calc[ic].exp_sp);
            for(size_t k=0;k<calc[ic].nop();k++,iy++) {
              K(iy,i) = (s1.r[k] - calc[ic].r[k])/dp[i];
     //         mio<<"K("<<iy<<','<<i<<")="<<K(iy,i)<<'\n';
            };
          };
          *p[i]-= dp[i];
       }else{
          iy=0;
          sum = 0.;
          for(size_t ic=0;ic<ns;ic++){
            s1.make(calc[ic]);
            s1.calcLines(lparams(),ip[i],ip[i],calc[ic].exp_sp);
            *p[i]+= dp[i];
            s2.make(calc[ic]);
            s2.calcLines(lparams(),ip[i],ip[i],calc[ic].exp_sp);
            *p[i]-= dp[i];
            for(size_t k=0;k<calc[ic].nop();k++,iy++) {
              K(iy,i) = (s2.r[k] - s1.r[k])/dp[i];
              g = (y[iy] - f[iy]);
              if (fabs(g/s1.r[k]) < 0.1)
                 sum += K(iy,i)*g*weig[iy];
            };
            b(i) = sum;
          };
       };
    };

      // calculate M and b
     for(size_t k=0;k<np;k++){
        for(size_t j=0;j<np;j++){
           sum = 0;
           for(size_t i=0;i<ny;i++) sum += K(i,k)*K(i,j)*weig[i];
           M(k,j) = sum;
        };
     };

// -----------------------------------------------------------------------------------------
//      find corrections to parameters
     for(size_t i=0;i<np;i++) {pp[i] = M(i,i)?1./sqrt(M(i,i)):0.; b[i]*=pp[i];};

     for(size_t i=0;i<np;i++)
     for(size_t j=i;j<np;j++){
       M(i,j) *= pp[i]*pp[j];
       if (i != j) M(j,i) = M(i,j);
     };

//     mio<<matr_show(M);


     if (M.dim1() == 1) {
       s[0] = 1.;
     }else
       M.diag(s);

     c.mul(M,b);
     for(size_t i=0;i<np;i++) c2[i] = c[i]*c[i];
//............. ......................................

//     mio<<"s="<<s;
//     ofstream fil("tmp.txt");
//     for(g=0.;g<5.;g+=.01) fil<<g<<' '<<kappa1(c2,s,g)<<'\n';

     //  looking for maximum of kappa
     g0 = 0.;
     g1 = 2.5;
     g2 = 5.;

     kap0 = kappa1(c2,s,g0);
     kap1 = kappa1(c2,s,g1);
     kap2 = kappa1(c2,s,g2);
     do{
       g = (g1 + g0)/2;
       kap =  kappa1(c2,s,g);
       if (kap > kap1){
          kap2 = kap1;
          g2 = g1;
          kap1 = kap;
          g1 = g;
       }else{
          kap0 = kap;
          g0 = g;
       };
       g = (g2 + g1)/2;
       kap =  kappa1(c2,s,g);
       if (kap > kap1){
          kap0 = kap1;
          g0 = g1;
          kap1 = kap;
          g1 = g;
       }else{
          kap2 = kap;
          g2 = g;
       };
     }while(fabs(1. - 2*kap/(kap0+kap1)) > 1e-5);
     g2 = g1*g1; // gamma^2

//....................................................


     for(size_t i=0;i<np;i++)
       if (s[i] != 0) c[i] /= (s[i] + g2);
       else c[i] = 0.;

     x.mul(c,M);
//     mio<<"s="<<s;
//     mio<<"b="<<b;
//     mio<<"x="<<x;

     for(size_t i=0;i<np;i++) {
       x[i] *= pp[i];
       //if (fabs(x[i]/(*p[i])) < 0.7)
       (*p[i]) += x[i];
     };
//     mio<<"x="<<x;
//-------------------------------------------------------------------------------------------
     if (lparams().error()){ // if fit errors are needed
       M.inverse();
       for(size_t i=0;i<err.size();i++)
         *err[i] = sqrt(M(i,i));
       M.inverse();
     };

     // calculate spectra with new params
     iy=0;
     sig = 0.;
     for(size_t ic=0;ic<ns;ic++){
       if (!(*bl[ic]).fixed()) (*bl[ic]).refit();
       calc[ic].calcLines(lparams(),i1,i2,calc[ic].exp_sp);
       for(size_t i=0;i<calc[ic].nop();i++,iy++) {
         f[iy] = calc[ic].rr(i);
//                tst_y.precision(12);
//                tst_y<<calc[ic].w(i)<<' '<<y[iy]<<'\n';
//                tst_f.precision(12);
//                tst_f<<calc[ic].w(i)<<' '<<f[iy]<<'\n';
         sum = f[iy] - y[iy];
         sig += sum*sum;
       };
     };
     if (ny > np)  sig /= (ny-np);
     sigs[iter % 5] = sig;

     if (lparams().error()){
       double qq = sqrt(sig);
       for(size_t i=0;i<err.size();i++)
         *err[i] *= qq;
     };

     if (out) {
       mio<<"\niter "<<iter<<'\n';
       mio<<"sigma="<<sig<<' '<<sig00<<' '<<g1<<'\n';
     };

     if (sig/sig00 > 100.){
       unfit();
       mio<<"Fit failed\n";
       return -sig00;
     };

     if (iter < 5) continue;
     sig = 0.;
     sig0 = sigs.sum()/5;
     for(size_t i=0;i<5;i++)  sig += sigs[i] - sig0;
     if (fabs(sig/sig0) < 1e-7) break;

  }; // --- end iter ---

  if (sig > sig00){
    unfit();
    mio<<"Fit failed\n";
    return -sig00;
  }else if (out){
    for(size_t i=0;i<names.size();i++){
      mio<<names[i]<<' '<<p0[i]<<' '<<*p[i];
      if (lparams().error()) mio<<' '<<*err[i];
      mio<<'\n';
    };
  };

  return sig;

}

void splist::unfit(){
  if (fit_i1 >= 0 || fit_i2 >= fit_i1) erase(fit_i1,fit_i2);
  if (p.size() == p0.size()){
    for(size_t i=0;i<p.size();i++) *(p[i]) = p0[i];
  };
  for(size_t i=0;i<spectra.size();i++)
    if (spectra[i].exp_sp && (!spectra[i].exp_sp->bline.fixed()))  spectra[i].exp_sp->bline.refit();
  if (fit_i1 >= 0 || fit_i2 >= fit_i1) renew(fit_i1,fit_i2);
  else
    renew();
}

//cmd_res splist::mouseClick(canvas* c,int x,int y,int shft){
//  cmd_res bad(false),repaint,ok;
//  repaint.repaint(true);
//  if (c->type != "graph") return bad;
//  graphics& g = *(graphics*)c;
//  double w = g.U(x);
//  double h = g.V(y);
//  if (mouse_add_line){
//    mouse_add_line = false;
//    for(size_t i=0;i<spectra.size();i++)
//      if (spectra[i].cname(owner->plt.focname)) {
//        ostringstream ostr;
//        ostr<<"add ";
//        ostr.precision(12);
//        ostr<<w<<' '<<h<<' '<<i;
//        return cmd(ostr.str());
//      };
//    addLine(w,h);
//    return repaint;
//  };
//  return ok;
//}
//
//cmd_res splist::mouseDoubleClick(canvas* c,int x,int y,int shft){
//  cmd_res bad(false),repaint;
//  repaint.repaint(true);
//  if (c->type != "graph") return bad;
//  graphics& g = *(graphics*)c;
//  double w = g.U(x);
//  size_t j = curr();
//  for(size_t i=0;i<size();i++){
//    if (line(i) < w) continue;
//    j = i;
//    if (j && fabs(line(j-1)-w) < fabs(line(j)-w)) j--;
//    setCurr(j);
//    break;
//  };
//  return repaint;
//}

} // Goblin

