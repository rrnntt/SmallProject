#ifndef SPBASE_H
#define SPBASE_H

#include "Goblin/dbase.h"
#include "Goblin/vjkg.h"
#include "Goblin/object.h"
#include "DataObjects/TableWorkspace.h"
#include "DataObjects/ColumnVector.h"

#include <iostream>
#include <vector>
#include <string>
#include <utility>
//#include "../Troll1/utils.h"
//#include "../Troll1/object.h"
//#include "../Troll1/graphics.h"

namespace Goblin
{

using namespace std;

class filter;

class GOBLIN_EXPORT spbase : public DataObjects::TableWorkspace,public object  {
protected:
  //rgb color;
  string out_type,in_type;
  string xfield,yfield;
  int line_style; //  0 - no lines, 1 - solid line
  int pnt_size;
  size_t m_currentRow;
  map<string,string> params;
public:
  static bool created;
  static double bad_double;
  static double bad_float;
  static double bad_int;
  static double bad_string;
  static VJKG bad_q;
  vector<int> *flt_;
  bool flt_en_;
//  file_name fname;
  vector< pair<double,double> > plot;
	spbase();
	~spbase();
  vector<double>* getDouble(const string nam);
  vector<double>* getDoubleNew(const string nam);
  vector<float>* getFloat(const string nam);
  vector<float>* getFloatNew(const string nam);
  vector<int>* getInt(const string nam);
  vector<int>* getIntNew(const string nam);
  //vector<string>* getString(const string nam);
  //vector<string>* getStringNew(const string nam);
  cmd_res cmd(string str);
  fun_res fun(string str);
  size_t size() const {return rowCount();}
  size_t curr() const {return m_currentRow;}
  void setCurr(size_t i) {if (i < rowCount()) m_currentRow = i;}
	size_t next(){return m_currentRow<size()-1?++m_currentRow:m_currentRow;}
	size_t prev(){return m_currentRow>0?--m_currentRow:m_currentRow;}
  void type_name(const string typnam,string& typ,string& nam);
  //void copy(spbase& spb,filter *flt=0);
  bool load(string fn);
  void apply_params();
  bool save();
  bool save(string fn);
  void savePlot(string fn);
  bool flt(){return (flt_!=0);}
  bool flt_enabled(){return flt_en_;}
  void flt_enable(){flt_en_=true;}
  void flt_disable(){flt_en_=false;}
  bool allowed(size_t i){return (!flt_ || !flt_enabled() || (i< rowCount() && (*flt_)[i]))?true:false;}
  bool allowed(){return allowed(curr());}
  bool allowed1(size_t i){return ((*flt_)[i])?true:false; }
  void flt_add(size_t i){if (flt_ && i<rowCount()) (*flt_)[i] = 1;}
  void flt_rm(size_t i){if (flt_ && i<rowCount()) (*flt_)[i] = 0;}
  void flt_make();
  void flt_delete();
  void flt_addDouble(string nam,string cmp,double val,char op=' ');
  void flt_addFloat(string nam,string cmp,float val,char op=' ');
  void flt_addInt(string nam,string cmp,int val,char op=' ');
  void flt_addString(string nam,string cmp,string val,char op=' ');
  void flt_not();
  void flt_erase();
  void flt_and(size_t i,bool v){if (flt_) (*flt_)[i] = (*flt_)[i] && v;}
  void flt_or(size_t i,bool v){if (flt_) (*flt_)[i] = (*flt_)[i] || v;}
  void flt_xor(size_t i,bool v){if (flt_) (*flt_)[i] = (!(*flt_)[i]) && v;}
  void flt_add(size_t i,bool v,char op=' ');
  //bool draw(canvas& c);
  //cmd_res mouseClick(canvas* c,int x,int y,int shft=0);
  //cmd_res mouseDoubleClick(canvas* c,int x,int y,int shft=0);
  void sort_double(string fn,bool des=true);
  void save_txt(string fn,string fmt);
};

ostream& operator << (ostream& ostr,const spbase& sp);

class GOBLIN_EXPORT type_filter{
public:
  char op;
  type_filter():op('&'){}
  virtual ~type_filter();
  virtual bool allowed(size_t i);
};

class GOBLIN_EXPORT filter{
public:
  bool allowed(size_t i);
};

class GOBLIN_EXPORT doubleFilter:public type_filter{
  double lower,upper;
  DataObjects::ColumnVector<double> data;
public:
  doubleFilter():data(){}
  bool set(spbase& sp,string nam,double lo,double up);
  bool allowed(size_t i){return data?(data[i]>lower && data[i]<upper):false;}
};

class GOBLIN_EXPORT intFilter:public type_filter{
  int lower,upper;
  DataObjects::ColumnVector<int> data;
public:
  intFilter():data(){}
  bool set(spbase& sp,string nam,int lo,int up);
  bool allowed(size_t i){return data?(data[i]>lower && data[i]<upper):false;}
};

class GOBLIN_EXPORT qFilter:public type_filter{
  DataObjects::ColumnVector<VJKG> data;
  VJKG qq;
  bool match_vib;
public:
  qFilter():match_vib(false){qq.assign("[-100] (0) -100 -100 -100");}
  bool set(spbase& sp,string nam,string value);
  bool allowed(size_t i);
};

} // Goblin

#endif
