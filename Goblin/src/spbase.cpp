#include <sstream>
#include <fstream>
#include "Goblin/spbase.h"
#include "Goblin/utils.h"
#include "Goblin/mio.h"
#include "Goblin/gdata.h"
#include "DataObjects/NumericColumn.h"

#include <algorithm>

namespace Goblin
{

using namespace std;

bool spbase::created = false;
double spbase::bad_double;
double spbase::bad_float;
double spbase::bad_int;
double spbase::bad_string;
VJKG   spbase::bad_q;

void assign_vjkg_member(void *q,const string mbr,const string val){
  if (mbr.empty())  {
    ((VJKG*)q)->assign(val);
    VJKG::default_Iso(((VJKG*)q)->Iso());
    return;
  };
  if (mbr=="j")  ((VJKG*)q)->j = atoi(val.c_str());
  if (mbr=="k")  ((VJKG*)q)->k = atoi(val.c_str());
  if (mbr=="g")  ((VJKG*)q)->g = atoi(val.c_str());
  if (mbr=="iso")  {
     ((VJKG*)q)->setIso(atoi(val.c_str()));
     VJKG::default_Iso(((VJKG*)q)->Iso());
  };
  if (mbr=="v")  {
    ((VJKG*)q)->setV(val.c_str());
    return;
  };
  if (mbr[0] == 'v'){
    int i = atoi(mbr.c_str()+1);
    ((VJKG*)q)->setV(i-1,atoi(val.c_str()));
  };
  if (mbr=="vv2")  {  //  convert from vers2 format
    string v;
    int n = ((VJKG*)q)->nv();
    int nval = val.size()-1;
    for(size_t i=0;i<n;i++)
      if (i <= nval) v += val[nval-i];
      else
         v += '0';
    ((VJKG*)q)->setV(v.c_str());
    return;
  };
}

spbase::spbase():object()   //,color(0,0,255)
{
  if (!created){
    db_field_type_register::register_type("vjkg",allocate_field<VJKG>,assign_vjkg_member,print_field<VJKG>);
    created = true;
  };
  //write(false);
  flt_ = 0;
  flt_en_ = false;
  out_type = "def";
  in_type = "def";
  line_style = 1;
  pnt_size = 5;
}
spbase::~spbase(){
}

/**
 * Return pointer to a vector of doubles in column nam.
 * If column with this name dosn't exist or it is not of type double
 * returns nullptr.
 * @param nam :: The name of the column.
 */
vector<double>* spbase::getDouble(const string nam)
{
  try
  {
    DataObjects::Column_ptr column = getColumn(nam);
    DataObjects::NumericColumn* num = column->asNumeric();
    if (num)
    {
      return num->getDoubleVector();
    }
    return nullptr;
  }
  catch(...)
  {}
  return nullptr;
}

/**
 * Return pointer to a vector of doubles in column nam.
 * If column with this name dosn't exist there will be an attemt to create it.
 * If a column with this name already exists but has a different type
 * returns nullptr.
 * @param nam :: The name of the column.
 */
vector<double>* spbase::getDoubleNew(const string nam)
{
  DataObjects::Column_ptr column;
  try
  {
    column = getColumn(nam);
  }
  catch(ColumnNotFound& e)
  {
    addColumn("double",nam);
    try
    {
      column = getColumn(nam);
    }
    catch(...)
    {
      return nullptr;
    }
  }
  DataObjects::NumericColumn* num = column->asNumeric();
  if (num)
  {
    return num->getDoubleVector();
  }
  return nullptr;
}

vector<float>* spbase::getFloat(const string nam){
  try
  {
    DataObjects::Column_ptr column = getColumn(nam);
    DataObjects::NumericColumn* num = column->asNumeric();
    if (num)
    {
      return num->getFloatVector();
    }
    return nullptr;
  }
  catch(...)
  {}
  return nullptr;
  //db_field<float>* f = getField<float>("f",nam);
  //return f?&(f->data):0;
}

vector<float>* spbase::getFloatNew(const string nam){
  DataObjects::Column_ptr column;
  try
  {
    column = getColumn(nam);
  }
  catch(ColumnNotFound& e)
  {
    addColumn("double",nam);
    try
    {
      column = getColumn(nam);
    }
    catch(...)
    {
      return nullptr;
    }
  }
  DataObjects::NumericColumn* num = column->asNumeric();
  if (num)
  {
    return num->getFloatVector();
  }
  return nullptr;
  //db_field<float>* f = getFieldNew<float>("f",nam);
  //return f?&(f->data):0;
}

vector<int>* spbase::getInt(const string nam){
  try
  {
    DataObjects::Column_ptr column = getColumn(nam);
    DataObjects::NumericColumn* num = column->asNumeric();
    if (num)
    {
      return num->getIntVector();
    }
    return nullptr;
  }
  catch(...)
  {}
  return nullptr;
  //db_field<int>* f = getField<int>("i",nam);
  //return f?&(f->data):0;
}

vector<int>* spbase::getIntNew(const string nam){
  DataObjects::Column_ptr column;
  try
  {
    column = getColumn(nam);
  }
  catch(ColumnNotFound& e)
  {
    addColumn("double",nam);
    try
    {
      column = getColumn(nam);
    }
    catch(...)
    {
      return nullptr;
    }
  }
  DataObjects::NumericColumn* num = column->asNumeric();
  if (num)
  {
    return num->getIntVector();
  }
  return nullptr;
  //db_field<int>* f = getFieldNew<int>("i",nam);
  //return f?&(f->data):0;
}

vector<string>* spbase::getString(const string nam){
  try
  {
    DataObjects::ColumnVector<std::string> vec = getColumn(nam);
    if (vec)
    {
      return vec.getStdVector();
    }
    return nullptr;
  }
  catch(...)
  {}
  return nullptr;
  //db_field<string>* f = getField<string>("s",nam);
  //return f?&(f->data):0;
}

vector<string>* spbase::getStringNew(const string nam){
  try
  {
    DataObjects::ColumnVector<std::string> vec = getColumn(nam);
    if (vec)
    {
      return vec.getStdVector();
    }
    return nullptr;
  }
  catch(...)
  {
    addColumn("string",nam);
    DataObjects::ColumnVector<std::string> vec = getColumn(nam);
    return vec.getStdVector();
  }
  return nullptr;
  //db_field<string>* f = getFieldNew<string>("s",nam);
  //return f?&(f->data):0;
}

bool spbase::load(string fn){
  //dbase::loadFromFile(fn);
  loadAscii(fn);
  return true;
}

void spbase::apply_params(){
}

cmd_res spbase::cmd(string str){
  cmd_res ok(true),bad(false),repaint;
  repaint.repaint(true);
  istringstream istr(str);
  string cmd_name,rest;
  istr >> cmd_name;
  rest = str.substr(str.find(cmd_name)+cmd_name.size());

  if (cmd_name == "param"){
    string pname,pvalue;
    istr >> pname>>pvalue;
    params[pname] = pvalue;
    return ok;
  };
  
  if (cmd_name=="apply_params"){
    this->apply_params();
    return ok;
  };
  
  if (cmd_name == "txt"){
    string fn,fmt;
    istr >> fn;
    fmt = str.substr(str.find(fn)+fn.size());
    save_txt(fn,fmt);
    return ok;
  };
  
  if (cmd_name == "sort_d"){
    string fn;
    istr >> fn;
    sort_double(fn);
    return repaint;
  };
  
  if (cmd_name == "xfield"){
    istr >> xfield;
    return repaint;
  };
  
  if (cmd_name == "yfield"){
    istr >> yfield;
    return repaint;
  };
  
  if (cmd_name == "out_type"){
    istr >> out_type;
    return repaint;
  };
  
  if (cmd_name == "in_type"){
    istr >> in_type;
    return ok;
  };
  
  if (cmd_name == "new"){
    string typnam,typ,nam;
    istr >> typnam;
    type_name(typnam,typ,nam);
    //newField(typ,nam);
    addColumn(typ,nam);
    ok.repaint(true);
    return ok;
  };

  if (cmd_name == "double"){ // converts int and float fields to double
    string typnam,typ,nam;
    istr >> typnam;
    type_name(typnam,typ,nam);
    //newField("d",nam);
    addColumn("double",nam);
    vector<double>* d = getDouble(nam);
    if (!d) return 0;
    if (typ == "float"){
      vector<float>* f = getFloat(nam);
      if (!f) return 0;
      for(size_t i=0;i<size();i++) (*d)[i] = double((*f)[i]);
    }else if (typ == "int"){
      vector<int>* f = getInt(nam);
      if (!f) return 0;
      for(size_t i=0;i<size();i++) (*d)[i] = double((*f)[i]);
    };
    return ok;
  };

  if (cmd_name == "sqrt"){
    string cd1;
    istr >> cd1;
    vector<double>* d1 = getDouble(cd1);
    if (!d1) {
      mio<<"Cannot find field d:"<<cd1<<'\n';
      return bad;
    };
    for(size_t i=0;i<size();i++) if (allowed(i)) (*d1)[i] = sqrt((*d1)[i]);
    return ok;
  };

  if (cmd_name == "^2"){
    string cd1;
    istr >> cd1;
    vector<double>* d1 = getDouble(cd1);
    if (!d1) {
      mio<<"Cannot find field d:"<<cd1<<'\n';
      return bad;
    };
    for(size_t i=0;i<size();i++) if (allowed(i)) (*d1)[i] *= (*d1)[i];
    return ok;
  };

  if (cmd_name == "="){
    string cd1,cd2;
    istr >> cd1 >> cd2;
    vector<double>* d1 = getDouble(cd1);
    vector<double>* d2 = getDouble(cd2);
    if (!d1) return bad;
    if (d2){  for(size_t i=0;i<size();i++) (*d1)[i] = (*d2)[i];
    }else{
      double dd = atof(cd2.c_str());
      for(size_t i=0;i<size();i++) if (allowed(i)) (*d1)[i] = dd;
    };
    return ok;
  };

  if (cmd_name == "+"){
    string cd1,cd2;
    istr >> cd1 >> cd2;
    vector<double>* d1 = getDouble(cd1);
    vector<double>* d2 = getDouble(cd2);
    if (!d1) return 0;
    if (d2){  for(size_t i=0;i<size();i++) if (allowed(i)) (*d1)[i] += (*d2)[i];
    }else{
      double dd = atof(cd2.c_str());
      for(size_t i=0;i<size();i++) if (allowed(i)) (*d1)[i] += dd;
    };
    return ok;
  };

  if (cmd_name == "--"){
    string cd1,cd2;
    istr >> cd1 >> cd2;
    vector<double>* d1 = getDouble(cd1);
    vector<double>* d2 = getDouble(cd2);
    if (!d1) return 0;
    if (d2) { for(size_t i=0;i<size();i++) if (allowed(i)) (*d1)[i] -= (*d2)[i];
    }else{
      double dd = atof(cd2.c_str());
      for(size_t i=0;i<size();i++) if (allowed(i)) (*d1)[i] -= dd;
    };
    return ok;
  };

  if (cmd_name == "*"){
    string cd1,cd2;
    istr >> cd1 >> cd2;
    vector<double>* d1 = getDouble(cd1);
    vector<double>* d2 = getDouble(cd2);
    if (!d1) return 0;
    if (d2) { for(size_t i=0;i<size();i++) if (allowed(i)) (*d1)[i] *= (*d2)[i];
    }else{
      double dd = atof(cd2.c_str());
      for(size_t i=0;i<size();i++) {if (allowed(i)) (*d1)[i] *= dd;};
    };
    return ok;
  };

  if (cmd_name == "/"){
    string cd1,cd2;
    istr >> cd1 >> cd2;
    vector<double>* d1 = getDouble(cd1);
    vector<double>* d2 = getDouble(cd2);
    double dd = atof(cd2.c_str());
    if (!d1) return 0;
    if (d2)
      for(size_t i=0;i<size();i++) {
        if (!allowed(i)) continue;
        dd = (*d2)[i];
        if (dd == 0.) dd = 1e-30;
        (*d1)[i] /= dd;
      }
    else{
      dd = atof(cd2.c_str());
      if (dd == 0.) dd = 1e-30;
      for(size_t i=0;i<size();i++) if (allowed(i)) (*d1)[i] /= dd;
    };
    return ok;
  };

  if (cmd_name == "out"){
    string cd,fn;
    vector<vector<double>*> d;
    vector<double> *d1;
    int pr = 12;
    istr >> fn;
    ofstream fil(fn.c_str());
    if (!fil) return 0;
    while(istr>>cd){
      if (cd[0] == '~') pr = atoi(cd.c_str()+1);
      else{
        d1 = getDouble(cd);
        if (d1) {
          d.push_back(d1);
          cerr<<"out: "<<cd<<' '<<'\n';
        };
      };
    };
    if (d.size()){
      for(size_t i=0;i<size();i++){
        for(size_t j=0;j<d.size();j++) {
          if (!allowed(i)) continue;
          fil.precision(pr);
          fil << (*(d[j]))[i] <<' ';
        };
        fil<<'\n';
      };
    };
    return ok;
  };
  if (cmd_name == "save"){
    save();
    return ok;
  };

  if (cmd_name == "saveAs"){
    string fn;
    istr >> fn;
    save(fn);
    return ok;
  };

  //if (cmd_name == "mvf"){ // move field form position i to j
  //  size_t i,j;
  //  istr>> i >> j;
  //  moveField(i,j);
  //  return ok;
  //};

  if (cmd_name == "defiso"){
    int i;
    istr>> i;
    VJKG::default_Iso(i);
    return ok;
  };

  if (cmd_name=="flt"){
    string com;
    istr>>com;

    if (com=="new"){
      flt_make();
      return ok;
    };

    if (com=="en"){
      flt_enable();
      ok.repaint(true);
      return ok;
    };

    if (com=="dis"){
      flt_disable();
      ok.repaint(true);
      return ok;
    };

    if (com=="not"){
      flt_not();
      ok.repaint(true);
      return ok;
    };

    if (com=="erase"){
      flt_erase();
      ok.repaint(true);
      return ok;
    };

    if (com=="add"){
      string typnam,typ,nam;
      string val,cmp;
      char op=' ';
      istr >> typnam >> cmp >> val >> op;
      type_name(typnam,typ,nam);
      if (typ == "d") flt_addDouble(nam,cmp,atof(val.c_str()),op);
      else if (typ == "f") flt_addFloat(nam,cmp,atof(val.c_str()),op);
      else if (typ == "i") flt_addInt(nam,cmp,atoi(val.c_str()),op);
      else if (typ == "s") flt_addString(nam,cmp,val,op);
      return ok;
    };

    return ok;
  };

  return object::cmd(str);
}

fun_res spbase::fun(string str){
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

  if (nam == "nfields"){
    ostr << columnCount();
    return ostr.str();
  };

  if (nam == "curr"){
    ostr << curr();
    return ostr.str();
  };

  if (nam == "defiso"){
    ostr << VJKG::d_Iso;
    return ostr.str();
  };

  if (nam == "sum"){
    string typnam;
    istr>>typnam;
    if (typnam.empty()) return fun_res("0");
    string typ,nam;
    type_name(typnam,typ,nam);
    if (typ == "d"){
      vector<double>* v = getDouble(nam);
      if (!v) {
        mio<<"No field "<<typnam<<'\n';
        return fun_res("0");
      };
      double sum = 0.;
      for(size_t i=0;i<size();i++) sum += (*v)[i];
      ostr << sum;
    } else if (typ == "f"){
      vector<float>* v = getFloat(nam);
      if (!v) {
        mio<<"No field "<<typnam<<'\n';
        return fun_res("0");
      };
      float sum = 0.;
      for(size_t i=0;i<size();i++) sum += (*v)[i];
      ostr << sum;
    } else if (typ == "i"){
      vector<int>* v = getInt(nam);
      if (!v) {
        mio<<"No field "<<typnam<<'\n';
        return fun_res("0");
      };
      int sum = 0;
      for(size_t i=0;i<size();i++) sum += (*v)[i];
      ostr << sum;
    } else 
       ostr << '0';
    return ostr.str();
  };

  return object::fun(str);
}


//void spbase::copy(spbase& sp,filter *flt){
//  deleteFields();
//  vector<db_basic_field*>::iterator iif;
//  db_basic_field* f;
//  string typ,nam;
//  vector<bool> ok;
//  if (flt){
//    ok.resize(sp.size());
//    for(size_t i=0;i<sp.size();i++)
//      ok[i] = flt->allowed(i);
//  };
//  for(iif=sp.fields.begin();iif!=sp.fields.end();iif++){
//    typ = (*iif)->type;
//    nam = (*iif)->name;
//    newField(typ,nam);
//    f = fields.back();
//    if (typ=="d"){
//      vector<double>& d = ((db_field<double>*)(f))->data;
//      vector<double>& d1 = ((db_field<double>*)(*iif))->data;
//      if (!flt){
//        d.resize(d1.size());
//        d = d1;
//      }else{
//        for(size_t i=0;i<sp.size();i++)
//          if (ok[i]) d.push_back(d1[i]);
//      };
//    };
//    if (typ=="f"){
//      vector<float>& d = ((db_field<float>*)(f))->data;
//      vector<float>& d1 = ((db_field<float>*)(*iif))->data;
//      if (!flt){
//        d.resize(d1.size());
//        d = d1;
//      }else{
//        for(size_t i=0;i<sp.size();i++)
//          if (ok[i]) d.push_back(d1[i]);
//      };
//    };
//    if (typ=="i"){
//      vector<int>& d = ((db_field<int>*)(f))->data;
//      vector<int>& d1 = ((db_field<int>*)(*iif))->data;
//      if (!flt){
//        d.resize(d1.size());
//        d = d1;
//      }else{
//        for(size_t i=0;i<sp.size();i++)
//          if (ok[i]) d.push_back(d1[i]);
//      };
//    };
//    if (typ=="vjkg"){
//      vector<VJKG>& d = ((db_field<VJKG>*)(f))->data;
//      vector<VJKG>& d1 = ((db_field<VJKG>*)(*iif))->data;
//      if (!flt){
//        d.resize(d1.size());
//        d = d1;
//      }else{
//        for(size_t i=0;i<sp.size();i++)
//          if (ok[i]) d.push_back(d1[i]);
//      };
//    };
//  };
//  params = sp.params;
//  fname = owner->dir()+"copy_of_"+sp.name+'.'+sp.ext;
//
//}

void spbase::savePlot(string fn){
  ofstream fil(fn.c_str());
  if (!fil){
    mio<<"Cannot open file "<<fn<<'\n';
    return;
  };
  for(size_t i=0;i<plot.size();i++)
    fil<<plot[i].first<<' '<<plot[i].second<<'\n';
}

void spbase::flt_make(){
  flt_ = getIntNew("flt");
  for(size_t i=0;i<size();i++) (*flt_)[i] = 1;
}

void spbase::flt_delete(){
  removeColumn("flt");
  flt_ = 0;
}

void spbase::flt_addDouble(string nam,string cmp,double val,char op){
  vector<double> *d_= getDouble(nam);
  if (!d_) return;
  vector<double> &d = *d_;
  bool ok;
  for(size_t i=0;i<size();i++){
    if (cmp=="=") ok = (d[i] == val);
    else if (cmp=="<") ok = (d[i] < val);
    else if (cmp=="<=") ok = (d[i] <= val);
    else if (cmp==">") ok = (d[i] > val);
    else if (cmp==">=") ok = (d[i] >= val);
    if (op == '&') ok = ok && allowed1(i);
    else if (op == '|') ok = ok || allowed1(i);
    else if (op == '^') ok = ok && (!allowed1(i));
    (*flt_)[i] = ok?1:0;
  };
}

void spbase::flt_addFloat(string nam,string cmp,float val,char op){
  vector<float> *d_= getFloat(nam);
  if (!d_) return;
  vector<float> &d = *d_;
  bool ok;
  for(size_t i=0;i<size();i++){
    if (cmp=="=") ok = (d[i] == val);
    else if (cmp=="<") ok = (d[i] < val);
    else if (cmp=="<=") ok = (d[i] <= val);
    else if (cmp==">") ok = (d[i] > val);
    else if (cmp==">=") ok = (d[i] >= val);
    if (op == '&') ok = ok && allowed1(i);
    else if (op == '|') ok = ok || allowed1(i);
    else if (op == '^') ok = ok && (!allowed1(i));
    (*flt_)[i] = ok?1:0;
  };
}

void spbase::flt_addInt(string nam,string cmp,int val,char op){
  vector<int> *d_= getInt(nam);
  if (!d_) return;
  vector<int> &d = *d_;
  bool ok;
  for(size_t i=0;i<size();i++){
    if (cmp=="=") ok = (d[i] == val);
    else if (cmp=="<") ok = (d[i] < val);
    else if (cmp=="<=") ok = (d[i] <= val);
    else if (cmp==">") ok = (d[i] > val);
    else if (cmp==">=") ok = (d[i] >= val);
    if (op == '&') ok = ok && allowed1(i);
    else if (op == '|') ok = ok || allowed1(i);
    else if (op == '^') ok = ok && (!allowed1(i));
    (*flt_)[i] = ok?1:0;
  };
}

void spbase::flt_addString(string nam,string cmp,string val,char op){
  //vector<string> *d_= getString(nam);
  DataObjects::Column_ptr d = getColumn(nam);
  if (!d) return;
  //vector<string> &d = *d_;
  bool ok;
  for(size_t i=0;i<size();i++){
    if (cmp=="=") ok = (d->asString(i) == val);
    else if (cmp=="has") ok = (d->asString(i).find(val) != string::npos);
    if (op == '&') ok = ok && allowed1(i);
    else if (op == '|') ok = ok || allowed1(i);
    else if (op == '^') ok = ok && (!allowed1(i));
    (*flt_)[i] = ok?1:0;
  };
}

void spbase::flt_not(){
  if (!flt_) return;
  for(size_t i=0;i<size();i++)
    if (allowed1(i)) flt_rm(i);
    else
      flt_add(i);
}

//void spbase::flt_erase(){
//  if (!flt_) return;
//  if (!flt_enabled()) return;
//  vector<int> indx(size());
//  for(size_t i=0;i<size();i++)
//    indx[i] = (*flt_)[i];
//  eraseRows(indx);
////  flt_delete();
//}

bool spbase::save(){
  if (write()){
    save(fname);
    return true;
  };
  return false;
}

bool spbase::save(string fn){
//  vector<int> *f = 0;
//  if (flt() && flt_enabled()) f = flt_;
//  dbase::saveToFile(fn.c_str(),f);
//  return true;
  return false;
}

void spbase::flt_add(size_t i,bool v,char op){
 if (!flt_ || i>=size()) return;
    if (op=='&') (*flt_)[i] = (*flt_)[i] && v;
    else if (op=='|') (*flt_)[i] = (*flt_)[i] || v;
    else if (op=='^') (*flt_)[i] = (*flt_)[i] && (!v);
    else (*flt_)[i] = (*flt_)[i] = v;
}

//bool spbase::draw(canvas& c){
//  if (c.type == "graph" && c.out_type == "def"){
//    if (xfield.empty() || yfield.empty()) return false;
//    vector<double> *xf = getDouble(xfield);
//    vector<double> *yf = getDouble(yfield);
//    if (!xf || !yf) return false;
//    graphics& g = *(graphics*)(&c);
//    g.clip_clientRect();
//    double x,y;
//    int xx,yy,p2=pnt_size/2;
//    g.pen(color);
//    for(size_t i=0;i<size();i++){
//       if (!allowed(i)) continue;
//       x = (*xf)[i];
//       y = (*yf)[i];
//       xx = g.X(x);
//       yy = g.Y(y);
//       if (x > g.uMin() && x < g.uMax() && y > g.vMin() && y < g.vMax())
//         g.drawRect(rect(xx-p2,yy-p2,pnt_size,pnt_size));
//    };
//    return true;
//  };
//  return false;
//}
//
//cmd_res spbase::mouseClick(canvas* c,int x,int y,int shft){
//  cmd_res bad(false),repaint,ok;
//  repaint.repaint(true);
//  if (c->type != "graph") return bad;
//  graphics& g = *(graphics*)c;
//  double w = g.U(x);
//  double h = g.V(y);
//  return bad;
//}
//
//cmd_res spbase::mouseDoubleClick(canvas* c,int x,int y,int shft){
//  cmd_res bad(false),repaint,ok;
//  repaint.repaint(true);
//  if (c->type != "graph") return bad;
//  graphics& g = *(graphics*)c;
//  double w = g.U(x);
//  size_t j = curr();
//  return bad;
//}

class comp_double{
  vector<double> *d;
  bool des;
public:
  comp_double(spbase& s,string fn,bool de=false):des(de){d = s.getDouble(fn);}
  int operator()(const size_t i1,const size_t i2){
    if (des) return d?(*d)[i1]>(*d)[i2]:i1>i2;
    return d?(*d)[i1]<(*d)[i2]:i1<i2;
  }
};

//void spbase::sort_double(string fn,bool des){
//  vector<size_t> ind;
//  ind.resize(size());
//  for(size_t i=0;i<ind.size();i++)
//    ind[i] = i;
//  sort(ind.begin(),ind.end(),comp_double(*this,fn,des));
//  for(size_t i=0;i<fields.size();i++) {
//    fields[i]->sort(ind);
//  };
//}

struct fmt_rec{
  string type;
  void* data;
  string fill,fill1;
  vector<size_t> sizes;
  fmt_rec():data(){}
};

//void spbase::save_txt(string fn,string fmt){
//  istringstream istr(fmt);
//  string f,typnam,typ,nam;
//  size_t prec,wdt,i;
//  fmt_rec fld;
//  vector<fmt_rec> flds;
//  vector<double>* dp;
//  vector<float>* fp;
//  vector<int>* ip;
//  VJKG* qq;
//  while(istr>>f){
//    fld.sizes.clear();
//    dp = 0; fp = 0; ip = 0;
//    i = f.find('{');
//    if (i != string::npos){
//      typnam = f.substr(0,i);
//      f.erase(0,i+1);
//      i = f.find('}');
//      if (i != string::npos) f[i] = ' ';
//      for(size_t j=0;j<f.size();j++)
//        if (!isdigit(f[j])) f[j] = ' ';
//      istringstream is(f);
//      while(is>>i)  fld.sizes.push_back(i);
//    }else{
//      typnam = f;
//    };
//    type_name(typnam,typ,nam);
//    if (typ.empty()) typ = typnam;
//    if (typ == "d") {
//      dp = getDouble(nam);
//      if (!dp) {
//        mio<<"No field d:"<<nam<<'\n';
//	       continue;
//      };
//      fld.data = (void*) dp;
//      switch (fld.sizes.size()){
//        case 0: fld.sizes.push_back(6);  // precision
//	       case 1: fld.sizes.push_back(12); // width
//       	case 2: fld.sizes.push_back(0);  // 0 - fixed, 1 - scientific
//      };
//    }else if (typ == "f"){
//      fp = getFloat(nam);
//      if (!fp) {
//        mio<<"No field f:"<<nam<<'\n';
//	continue;
//      };
//      fld.data = (void*) fp;
//      switch (fld.sizes.size()){
//        case 0: fld.sizes.push_back(6);  // precision
//	       case 1: fld.sizes.push_back(12); // width
//       	case 2: fld.sizes.push_back(0);  // 0 - fixed, 1 - scientific
//      };
//    }else if (typ == "i"){
//      ip = getInt(nam);
//      if (!ip) {
//        mio<<"No field i:"<<nam<<'\n';
//        continue;
//      };
//      fld.data = (void*) ip;
//      if (!fld.sizes.size()) fld.sizes.push_back(5);  //  width
//    }else if (typ == "vjkg"){
//      //db_field<VJKG>* qf = getField<VJKG>(typ,nam);
//      DataObjects::ColumnVector<VJKG> qf = getColumn(nam);
//      if (!qf) {
//        mio<<"No field vjkg:"<<nam<<'\n';
//       	continue;
//      };
//      fld.data = (void*) (&qf->data);
//      switch (fld.sizes.size()){
//        case 0: fld.sizes.push_back(2);  // vib quanta format
//	       case 1: fld.sizes.push_back(2);  // separation between vib and rot quanta
//        case 2: fld.sizes.push_back(3);  // rot quanta format
//        case 3: fld.sizes.push_back(0);  // print iso, if nonzero - separation between iso and vib
//                      //  by default iso is not printed
//      };
//      fld.fill = string(fld.sizes[1],' ');
//      fld.fill1 = string(fld.sizes[3],' ');
//    }else if (typ == "x"){
//      fld.data = 0;
//      switch (fld.sizes.size()){
//        case 0: fld.sizes.push_back(1);  // left width
//	       case 1: fld.sizes.push_back(0);  // right width
//      };
//      if (nam.empty()){
//        fld.fill = string(fld.sizes[0],' ');
//        mio<<fld.fill.size()<<'\n';
//      }else{
//        fld.fill = string(fld.sizes[0],' ') + nam + string(fld.sizes[1],' ');
//      };
//    }else {
//      mio<<"Unknown type "<<typ<<'\n';
//      continue;
//    };
//    fld.type = typ;
//    flds.push_back(fld);
//  };
////  for(vector<fmt_rec>::iterator ff=flds.begin();ff!=flds.end();ff++){
////    mio<<ff->type<<' '<<ff->data<<' '<<ff->sizes.size()<<'\n';
////  };
//  
//  ofstream fil(fn.c_str());
//  if (!fil){
//    mio<<"Cannot open file "<<fn<<'\n';
//    return;
//  };
//  for(i=0;i<size();i++){
//    if (!allowed(i)) continue;
//    for(vector<fmt_rec>::iterator ff=flds.begin();ff!=flds.end();ff++){
//      if (ff->type == "d") {
//        if (ff->sizes[2])  fil<<strDoubleE( (* (vector<double>*)ff->data)[i],ff->sizes[0],ff->sizes[1]);
//	else
//	  fil<<strDouble( (* (vector<double>*)ff->data)[i],ff->sizes[0],ff->sizes[1]);
//      }else if (ff->type == "f"){
//        if (ff->sizes[2])  fil<<strDoubleE( (double)(* (vector<float>*)ff->data)[i],ff->sizes[0],ff->sizes[1]);
//	else
//	  fil<<strDouble( (double)(* (vector<float>*)ff->data)[i],ff->sizes[0],ff->sizes[1]);
//      }else if (ff->type == "i"){
//        fil<<strInt( (* (vector<int>*)ff->data)[i],ff->sizes[0]);
//      }else if (ff->type == "vjkg"){
//        qq = &(* (vector<VJKG>*)ff->data)[i];
//        if (ff->sizes[3]) fil<<qq->iso()<<ff->fill1;
//	for(size_t j=0;j<qq->v.n();j++) fil<<strInt(qq->v(j),ff->sizes[0]);
//	fil<<ff->fill<<strInt(qq->j,ff->sizes[2])<<strInt(qq->k,ff->sizes[2])<<strInt(qq->g,ff->sizes[2]);
//      }else {//  ff->type == "x"
//         fil<<ff->fill;
//      };
//    };
//    fil<<endl;
//  };
//}

/** divides typnam into type and name of a field */
void spbase::type_name(const string typnam,string& typ,string& nam){
  string str = typnam;
  if (str[0] == '%') str.erase(0,1);
  size_t i;
  i = str.find(':');
  if (i == string::npos || i == 0){
    typ = "";
    nam = "";
    return;
  };
  typ = str.substr(0,i);
  nam = str.substr(i+1);
}



//--------------------------------------------------------------------------------
//       filters

bool doubleFilter::set(spbase& sp,string nam,double lo,double up){
    data = sp.getColumn(nam);
    if (!data) return false;
    lower = lo;
    upper = up;
    return true;
}

bool intFilter::set(spbase& sp,string nam,int lo,int up){
    data = sp.getColumn(nam);
    if (!data) return false;
    lower = lo;
    upper = up;
    return true;
}

bool qFilter::set(spbase& sp,string nam,string value){
  data = sp.getColumn(nam);
  if (!data) return false;
  string str = value;
  string vvv = brakets(str,"()");
  if (!vvv.empty()){
    str.erase(str.find("("+vvv+")"),vvv.size()+2);
    qq.setV(vvv);
  };
  return true;
}

bool qFilter::allowed(size_t i){
  if (!data) return false;
  bool ok;
  VJKG &q = data[i];
  ok = match_vib?(q.v == qq.v):true;
  ok = ok && (qq.iso()!=-100)?(q.iso()==qq.iso()):true;
  ok = ok && (qq.j!=-100)?(q.j==qq.j):true;
  ok = ok && (qq.k!=-100)?(q.k==qq.k):true;
  ok = ok && (qq.g!=-100)?(q.g==qq.g):true;
  return ok;
}

//-----------------------------------------------------------
//ostream& operator << (ostream& ostr,const spbase& sp){
//  vector<db_basic_field*>::const_iterator f;
//  void* d;
//  size_t i;
//  i = sp.curr();
//  for(f=sp.fields.begin();f!=sp.fields.end();f++){
//    d = (**f).data_addr(i);
//    if ((**f).print && d) (**f).print(ostr,d);
//  };
//  return ostr;
//}

//------------------------------------------------------------

type_filter::~type_filter(){
}

bool type_filter::allowed(size_t i){
  return true;
}

bool filter::allowed(size_t i){
  return true;
//  if (data.size()<2) return
//  for(size_t i=0;i<data.size();i++)
}

} // Goblin 
