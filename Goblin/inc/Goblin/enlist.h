#ifndef ENLIST_H
#define ENLIST_H

#include "Goblin/spbase.h"
#include "Goblin/vjkg.h"
//#include "../Troll1/def.h"

#include <string>
#include <vector>

namespace Goblin
{

using namespace std;

class enlist:public spbase {
public:
  vector<double> *ener_p;
  vector<VJKG> *q_p;
  enlist();
  ~enlist();
  size_t add(VJKG& q,double ee);
  double& ener(size_t i){return (*ener_p)[i];}
  double& ener(VJKG& q);
  size_t index(VJKG& q);
  size_t index(const VJKG& q);
  VJKG& q(size_t i){return (*q_p)[i];}
  bool load(string fn);
  void sort_symm();
  void sort_q();
  cmd_res cmd(string str);
  fun_res fun(string str);
  void swapq(VJKG& q1,VJKG& q2);
  void diff(enlist& en);
  void table(enlist& en,string fn_and_states);
  void cmp_q(enlist& en,int Jmax=100);
//  bool draw(canvas& c);
//  cmd_res mouseClick(canvas* c,int x,int y,int shft=0);
//  cmd_res mouseDoubleClick(canvas* c,int x,int y,int shft=0);
};

} // Goblin

#endif
