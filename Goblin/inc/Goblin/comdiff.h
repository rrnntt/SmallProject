#ifndef COMDIFF_H
#define COMDIFF_H

#include "Goblin/DllExport.h"
#include "Goblin/vjkg.h"

#include <vector>
#include <string>
#include <iostream>

namespace Goblin
{

using namespace std;

struct level{
  VJKG q;
  double e;
  int i1,i2;
};

struct foundItem{
  int low;   // index in lower
  int line;  // index in sp
};

struct elevel{
  VJKG q;
  double e; // mean value
  int n;    // numb. of lines
  int ng;   // numb. of good lines
  double e1,e2;// spread
  double err; // exper. error
};

class splist;
class enlist;

class comdiff {
public: 
  comdiff();
  ~comdiff();
  double found_e(int i)const;
  bool set(splist&,enlist& low_en,string vstr,double up_ener, VJKG& up_q);
  bool set(double up_ener, VJKG& up_q);
  bool selRules(VJKG& q1,VJKG& q,int dj,int dk);
  void find();
  void check();
  void setUpper(enlist& up_en){up_ener = &up_en;}
  void assign(VJKG qq);
  void assign(){assign(q);}
  void get(size_t i);
  void del(size_t i);
  double e0;
  double dw,dW;
  int dk_max;
  VJKG q;
  vector<level> lower;
  vector< vector<foundItem> > found;
  splist *sp;
  enlist *low_ener;
  enlist *up_ener;
  VJKG q1;
};

ostream& operator << (ostream& ostr,const comdiff& cd);

} // Goblin

#endif

