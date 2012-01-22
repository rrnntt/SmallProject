#include "Goblin/enlist.h"
#include "Goblin/splist.h"
#include "Goblin/comdiff.h"
#include "Goblin/mio.h"
#include "Kernel/Logger.h"

#include <iostream>
#include <iomanip>

//#include "../Troll1/mio.h"

namespace Goblin
{

using namespace std;

/*ostream& operator << (ostream& ostr,const comdiff& cd){
  size_t ie,il;
  ostr<<cd.q<<' ';
  ostr.precision(11);
  ostr<<cd.e0<<'\n';
  ostr<<"---------"<<cd.found.size()<<"-----------"<<'\n';
  for(size_t i=0;i<cd.found.size();i++){
    ostr<<i<<' '
        <<setprecision(11)<<cd.found_e(i)<<'\n';
    for(int j=0;j<cd.found[i].size();j++){
      ie = cd.found[i][j].low;
      il = cd.found[i][j].line;
      ostr<<j<<") "<<cd.lower[ie].q<<' '<<setprecision(8)<<cd.sp->line(il)<<' '
      <<setprecision(3)<<cd.sp->height(il)<<' ';
      ostr<<setprecision(11)<<cd.sp->line(il)+cd.lower[ie].e<<' '<<cd.sp->q(il)<<'\n';
    };
    ostr<<'\n';
  };
  return ostr;
}
*/

ostream& operator << (ostream& ostr,const comdiff& cd){
  size_t ie,il;
  mio <<cd.q<<' ';
  //mio.operator<<(cd.q);
  mio <<cd.e0<<'\n';
  mio <<"---------"<<cd.found.size()<<"-----------"<<'\n';
  for(size_t i=0;i<cd.found.size();i++){
    mio <<i<<' '
        <<setprecision(11)<<cd.found_e(i)<<'\n';
    for(int j=0;j<cd.found[i].size();j++){
      ie = cd.found[i][j].low;
      il = cd.found[i][j].line;
      mio <<j<<") "<<cd.lower[ie].q<<' '<<cd.sp->line(il)<<' '
      <<cd.sp->height(il)<<' ';
      mio <<cd.sp->line(il)+cd.lower[ie].e<<' '<<cd.sp->q(il)<<'\n';
    };
    mio <<'\n';
  };
  return ostr;
}

comdiff::comdiff(){
  sp = 0;
  dw = 0.01; // accuracy
  dW = 1.;   // interval
  dk_max = 5;
}
comdiff::~comdiff(){
}

double comdiff::found_e(int i)const{
  double res = 0.;
  if (!sp || !found[i].size()) return 0.;
  for(int j=0;j<found[i].size();j++)
    res += sp->line(found[i][j].line) + lower[found[i][j].low].e;
  return res/found[i].size();
}

bool comdiff::set(double up_ener, VJKG& up_q){

  e0 = up_ener;
  q = up_q;
  if (e0 == 0.) return true; // setting only q for finding lines to this state

  int dj,dk;
  level lvl;

  lower.clear();
  found.clear();
  if (!low_ener || !sp) return false;

  enlist& en = *low_ener;
  splist& ll = *sp;

  size_t symm = q.tsymm();
  VJKG q0;
  for(dj=-1;dj<=1;dj++)
  for(dk=-dk_max;dk<=dk_max;dk++)
    for(int i=0;i<en.size();i++){
      q0 = en.q(i);
      if (q.iso() == q0.iso() && q0.j == q.j+dj && q0.k == q.k+dk && q0.symm() == symm) {
         lvl.q = q0;
         lvl.e = en.ener(i);
         lvl.i1 = lvl.i2 = -1;
         double lin = e0 - lvl.e;
         for(int j=0;j<ll.size();j++){
           if (lvl.i1 < 0 && ll.line(j) >= lin - dW) lvl.i1 = j;
           if (lvl.i2 < 0 && ll.line(j) >= lin + dW) {
             if (j > 0) lvl.i2 = j - 1;
             else lvl.i2 = j;
             break;
           };
         };
         if (lvl.i1 >=0 && lvl.i2 < 0) lvl.i2 = ll.size()-1;
         lower.push_back(lvl);
         //break;
      };
    };
    return true;
}

/** only for X2Y molecules!!! */
bool comdiff::selRules(VJKG& q1,VJKG& q,int dj,int dk){
  int j1 = q.j + dj;
  if (j1 < 0) return false;
  int v3 = q1.v(3) + q.v(3) + dk;
  if (v3/2*2 == v3) return false;
  int k1 = q.k + dk;
  if (k1 > j1) return false;
  int g1 = j1-k1;
  if ((g1+q.g)/2*2 == g1+q.g) g1++;
  q1.j = j1;
  q1.k = k1;
  q1.g = g1;
  return true;
}

void comdiff::find(){
  vector<foundItem> f1;
  double lin1,lin2,e1,e2;
  if (!sp) return;
  splist& ll = *sp;

  foundItem fi;
  found.clear();
  for(int i=0;i<lower.size();i++)
  for(int ii=lower[i].i1;ii<=lower[i].i2;ii++) {
    f1.clear();
    lin1 = ll.line(ii);
    e1 = lower[i].e;
    for(int j=i+1;j<lower.size();j++)
    for(int jj=lower[j].i1;jj<=lower[j].i2;jj++) {
      lin2 = ll.line(jj);
      e2 = lower[j].e;
      if (fabs(lin1+e1-lin2-e2) <= dw){
        if (!f1.size()) {
          fi.low = i;
          fi.line = ii;
          f1.push_back(fi);
        };
        fi.low = j;
        fi.line = jj;
        f1.push_back(fi);
      }else if (j > i+1){
        for(int k=0;k<f1.size();k++){
          double lin,en;
          lin = ll.line(f1[k].line);
          en = lower[f1[k].low].e;
          if (fabs(lin+en-lin2-e2) <= dw){
            fi.low = j;
            fi.line = jj;
            f1.push_back(fi);
            break;
          };
        };
      };
    };
    bool ok;
    if (!f1.size()) continue;
    ok = false;
    for(int m=0;m<found.size();m++){
      for(int k=0;k<f1.size();k++){
        ok = false;
        for(int n=0;n<found[m].size();n++)
          if (f1[k].low==found[m][n].low && f1[k].line==found[m][n].line) {ok = true; break;};
        if (!ok) break;
      };
      if (ok) break;
    };
    // ok == true means that all transitions in f1 have already been found
    if (!ok) found.push_back(f1);
  };
}


bool comdiff::set(splist& spd,enlist& low_en,string vstr,double up_ener, VJKG& up_q){
  sp = &spd;
  q1.setV(vstr);
  low_ener = &low_en;
  return set(up_ener,up_q);
}

void comdiff::check(){
  vector<foundItem> f1;
  if (!sp) {
     mio <<"Linelist is not set\n";
     return;
  };
  if (!low_ener) {
    mio <<"Ground state energies are not set\n";
    return;
  };
  splist& ll = *sp;
  enlist& en = *low_ener;

  foundItem fi;
  level lvl;
  found.clear();
  lower.clear();
  if (!ll.assigned()) return;
  size_t j;
  for(size_t i=0;i<ll.size();i++)
    if (ll.q(i) == q){
      fi.line = i;
      j = en.index(ll.q0(i));
      if (j >= en.size()) continue;
      lvl.q = ll.q0(i);
      lvl.e = en.ener(j);
      lower.push_back(lvl);
      fi.low = lower.size()-1;
      f1.push_back(fi);
    };
  found.push_back(f1);
}

void comdiff::assign(VJKG qq){
  if (!found.size()) return;
  if (!sp->assigned()) sp->make_quanta();
  vector<foundItem>::iterator f;
  for(f=found[0].begin();f!=found[0].end();f++){
    sp->q(f->line) = qq;
    sp->q0(f->line) = lower[f->low].q;
  };
}

void comdiff::get(size_t i){
  if (i >= found.size()) return;
  vector< vector<foundItem> >::iterator f;
  if (found.size() > 1){
    f = found.insert(found.begin(),found[i]);
    f++;
    found.erase(f,found.end());
  };
}

void comdiff::del(size_t i){
  if (!found.size()) return;
  if (i >= found[0].size()) return;
  vector<foundItem>::iterator f;
  size_t j = 0;
  for(f=found[0].begin();f!=found[0].end();f++,j++)
  if (j == i){
    found[0].erase(f);
    break;
  };
}

} // Goblin
