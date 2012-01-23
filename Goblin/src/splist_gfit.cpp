#include <fstream>
#include <valarray>
#include <stdlib.h>
#include <algorithm>
#include <map>

#include "Goblin/lineparams.h"
#include "Goblin/splist.h"
#include "Goblin/enlist.h"
#include "Goblin/matrix.h"
#include "Goblin/mio.h"
#include "Goblin/gdata.h"

namespace Goblin
{

using namespace std;

double splist::fitq(double h1,double h2,enlist* pen){
  enlist& en = pen?*pen:*(new enlist);
  make_ener(en,"e0");
  vector<double>* lin = getDouble("line");
  vector<double>* e0 = getDouble("e0");
  mio<<"ener "<<en.size()<<'\n';
  en.save(owner->dir()+"en000.lev3");
  map<VJKG,pair<double,int> > ee;
  for(size_t i=0;i<en.size();i++)
    ee[en.q(i)] = make_pair(en.ener(i),0);
  double h;
  for(size_t i=0;i<size();i++){
    h = height(i);
    if (h > h1 && h < h2) ee[q(i)].second = 1;
  };
  cmd("flt new");
  for(size_t i=0;i<size();i++)
    if (q(i).assigned() && ee[q(i)].second){
       (*lin)[i] = ee[q(i)].first - (*e0)[i];
       flt_add(i);
    }else
      flt_rm(i);
  cmd("renew");
  if (!pen) delete &en;
  return 0.;
}

void splist::efit(enlist& en,vector<int>& ind,bool int_fit,bool out){
  ofstream tst_y("tst.y");
  ofstream tst_f("tst.f");
  p.clear();
  p0.clear();
  dp.clear();
  ip.clear();
  err.clear();
  names.clear();
  calc.clear();
  bl.clear();
  if (!spectra.size()) {
    mio<<"There is no spectra to fit to\n";
    return;
  };
  //if (fit_vis){
  //   for(size_t i=0;i<spectra.size();i++){
  //       spectra[i].fit = spectra[i].cname(owner->plt.focname)?true:false;
  //       if (out) mio<<owner->plt.focname<<' '<<spectra[i].cname(owner->plt.focname)<<'\n';
  //   };
  //};

  if (out) {
    mio<<"eFitting"<<'\n';
    mio<<"Always width fit \n";
  };
  fit_i1 = -1;
  fit_i2 = -1;

  spectrum *s,*exps;

  size_t iw2,ic;
  double w1,w2,dw;
  ic=0;
  ny = 0;
  int il;

  vector<double>* lin = getDouble("line");
  vector<double>* hi = getDouble("intens");
  vector<double>* e0 = getDouble("e0");
  if (!e0){
    mio<<"No e0 in the linelist\n";
    return;
  };
  
  obj_list<spectrum>  sps(owner,"spectrum");
  for(size_t i=0;i<spectra.size();i++)
     if (spectra[i].exp_sp && spectra[i].fit) {
       if (!sps.has(spectra[i].exp_sp)){
         mio<<"Cannot find exp. spectrum\n";
         return;
       };
       exps = spectra[i].exp_sp;
       dw = exps->delw();
       il = -1;
       while (true){
         il++;
         w1 = line(ind[il]) - exps->lwidth();
         while( il < ind.size()-1 && line(ind[il]) + exps->lwidth() >= 
                                     line(ind[il+1]) - exps->lwidth() ) il++;
         
         w2 = line(ind[il]) + exps->lwidth();
         if (w1 < exps->wstart()) w1 = exps->wstart();
         if (w2 > exps->wstop()) w2 = exps->wstop();
         if (w2 - w1 <= 0){
           if (il == ind.size()-1) break;
           continue;
         };
         int i1,i2;
         i1 = exps->index(w1);
         i2 = exps->index(w2);
         bool skip = false;
         for(int ii=i1;ii<=i2;ii++)
           if (exps->weight(ii) < 0){
             skip = true;
             break;
           };
         if (skip){
             if (out) mio<<"Skipping "<<exps->name<<' '<<w1<<' '<<w2<<'\n';
             if (il == ind.size()-1) break;
             continue;
         };
         w1 = exps->w(i1);
         w2 = exps->w(i2);
         s = &calc.add();
         s->copy(*exps,w1,w2);
         bl.push_back(&exps->bline);
         s->exp_sp = exps;
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
         if (il == ind.size()-1) break;
       };
     };

  ns = calc.size(); // number of spectra
  map< VJKG,int > efix,ifix;
  for(size_t i=0;i<ind.size();i++){
    if (!lparams().fixed(i,0))  efix[q(ind[i])] = 1;  //  0 must be line center
    if (!lparams().fixed(i,1))  ifix[q(ind[i])] = 1;  //  1 must be intensity
  };
  
  npe = 0;  //  number of energies
  npi = 0;  //  number of intensities
  for(map< VJKG,int >::iterator ff=efix.begin();ff!=efix.end();ff++)
  if (ff->second){
    il = en.index(ff->first);
    ip.push_back(il);
    p.push_back(&en.ener(il));
    p0.push_back(en.ener(il));
    dp.push_back(lparams().dp(0,0));
    names.push_back("Ener "+en.q(il).str());
    npe++;
  };

  vector<int> eind(ind.size(),-1);
  vector<int> iind(ind.size(),-1);
  for(size_t i=0;i<ind.size();i++){
    il = ind[i];
    for(size_t j=0;j<npe;j++)
      if (q(il) == en.q(ip[j])) eind[i] = j;
  };
  
  if (int_fit && out) mio<<"Int.fit\n";
  vector<double>* intf = int_fit?en.getDouble("intf"):0;  //  intensity factor
  vector<double> cint;  //  calculated intensities;
  
  if (!connected()) {
    intf = 0;
    mio<<"Line list is not connected\n";
  }else{
    splist* cll = getConnected();
    if (!cll){
      mio<<"Cannot find connected line-list!\n";
      return;
    };
    vector<double>* cintens = cll->getDouble("intens");
    if (!cintens){
      mio<<"No field d:intens in line-list "<<cll->name<<'\n';
      return;
    };
    for(size_t i=0;i<ind.size();i++)
      if (con(ind[i])>=0) cint.push_back((*cintens)[con(ind[i])]);
      else{
        cint.push_back(0.);
        lparams().fix(ind[i],1);
      };
  };

  if (intf){
    for(map< VJKG,int >::iterator f=ifix.begin();f!=ifix.end();f++)
    if (f->second){
      il = en.index(f->first);
      ip.push_back(il);
      p.push_back(&(*intf)[il]);
      p0.push_back((*intf)[il]);
      dp.push_back(0.0001);
      names.push_back("Intf "+en.q(il).str());
      npi++;
    };
    for(size_t i=0;i<ind.size();i++){
      il = ind[i];
      for(size_t j=npe;j<npe+npi;j++)
        if (q(il) == en.q(ip[j])) iind[i] = j;
    };  
  };

  for(size_t i=0;i<ind.size();i++){
    il = ind[i];
    if (!lparams().fixed(il,0)) 
      (*lin)[il] = (*p[eind[i]]) - (*e0)[il];
    for(size_t j=1;j<lparams().size();j++){
       if (!lparams().fixed(il,j)) {
         if (j == 1 && intf){
           (*hi)[il] = (*p[iind[i]])*cint[i];
           continue;
         };
         p.push_back(&lparams().par(il,j));
         p0.push_back(lparams().par(il,j));
         dp.push_back(lparams().dp(il,j));
         ip.push_back(il);
         if (lparams().error()) err.push_back(&lparams().err(il,j));
         names.push_back("Line "+strInt(il)+' '+lparams().names[j]);
       }else if (lparams().error()) lparams().err(il,j) = 0.;
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

  //  Saving dependent parameters for infit
  for(size_t i=0;i<ind.size();i++){
    il = ind[i];
    p.push_back(&(*lin)[il]);
    p0.push_back((*lin)[il]);
    if (!intf) {
      p.push_back(&(*hi)[il]);
      p0.push_back((*hi)[il]);
    };
  };

  y.resize(ny);
  f.resize(ny);
  weig.resize(ny);

  size_t iy = 0;
  ic = 0;
  spectrum s1;
  int iw1;
  for(size_t ic=0;ic<calc.size();ic++){
     s1.make(calc[ic]);
     s1.calcLines(lparams(),ind,calc[ic].exp_sp,-1);// calc lines not in ind
     s1.sub(calc[ic].exp_sp->bline);
     iw1 = calc[ic].exp_sp->index(s1.wstart());
       for(size_t i=0;i<calc[ic].nop();i++,iy++) {
           y[iy] = calc[ic].rr(i) - s1.rr(i);
           weig[iy] = (iw1+i<calc[ic].exp_sp->nop())?calc[ic].exp_sp->weight(iw1+i):0.;
       };
  };

  if (ny < np){
    mio<<"fit: Too many parameters\n";
    return;
  };

  double ret=0.;
//-----------------------------------------------------------------------------
  set4efit_data dt;
  dt.ind = &ind;
  dt.eind = &eind;
  dt.iind = &iind;
  dt.lin = lin;
  dt.hi = hi;
  dt.e0 = e0;
  dt.intf = intf;
  dt.cint = &cint;
  
  int ngood = 0;
  size_t jp;
  iy = 0;
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
//  spectrum s1;
  double sum,sig,sig0,sig00;


  mio<<"f.size="<<f.size()<<' '<<y.size()<<'\n';
  // calculate spectra with old params
  iy=0;
  sig0 = 0.;
  for(size_t ic=0;ic<ns;ic++){
    calc[ic].calcLines(lparams(),ind,calc[ic].exp_sp);
    for(size_t i=0;i<calc[ic].nop();i++,iy++) {
      if (iy >= f.size()){
        mio<<"Oops...\n";
        return;
      };
      f[iy] = calc[ic].rr(i);
//             tst_y.precision(12);
//             tst_y<<calc[ic].w(i)<<' '<<y[iy]<<'\n';
      sum = f[iy] - y[iy];
      sig0 += sum*sum;
    };
  };
  if (ny > np)  sig0 /= (ny-np);
  sig00 = sig0;

  if (out) mio<<"sigma0="<<sig0<<" np="<<np<<" ns="<<ns<<" npe="<<npe<<" npi="<<npi<<'\n';

  for(size_t iter=0;iter<50;iter++){ // iterations
     
    again_when_failed:
    for(jp=0;jp<np;jp++){
     if (failed[jp]) continue;
     *p[jp]+= dp[jp];// 0.0001;
     set4efit(jp,dt);
     iy=0;
     for(size_t ic=0;ic<ns;ic++){
       if (!(*bl[ic]).fixed()) (*bl[ic]).refit();
       s1.make(calc[ic]);
       s1.calcLines(lparams(),ind,calc[ic].exp_sp);
       for(size_t k=0;k<calc[ic].nop();k++,iy++) {
         F(iy,jp) = (s1.r[k] - calc[ic].r[k])/dp[jp];
//         mio<<"F("<<iy<<','<<jp<<")="<<F(iy,jp)<<'\n';
       };
     };
     *p[jp]-= dp[jp];// 0.0001;
     set4efit(jp,dt);
    };
     for(size_t k=0;k<np;k++){
        if (failed[k]){
          x(k) = 0.;
          M(k,k) = 1.;
          continue;
        };
        sum = 0;
        for(size_t i=0;i<ny;i++) sum += F(i,k)*(y[i] - f[i])*weig[i];
        x(k) = sum;
        for(size_t j=0;j<np;j++){
           sum = 0;
           for(size_t i=0;i<ny;i++) {
              sum += F(i,k)*F(i,j)*weig[i];
              //if (k == j) mio<<k<<' '<<i<<' '<<F(i,k)*F(i,j)<<' '<<weig[i]<<'\n';
           };
           M(k,j) = sum;
        };
     };

     bool failure = false;
     for(size_t i=0;i<np;i++)
     if (!failed[i] && M(i,i) == 0.){
       failure = true;
       failed[i] = true;
       if (out) mio<<"Par "<<i<<" failed\n";
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

     if (!M.solve(x)){
       mio<<"Fit failed\n";
       return;
     };

//     mio<< "New params:"<<'\n';
     for(size_t jp=0;jp<np;jp++){
//       mio<<*p[jp];
       *p[jp] += x[jp];
       set4efit(jp,dt);
//       mio<<' '<<*p[jp]<<'\n';
     };

//     for(size_t i=i1;i<=i2;i++) lparams().limit(i);

     // calculate spectra with new params
     iy=0;
     sig = 0.;
     for(size_t ic=0;ic<ns;ic++){
       if (!(*bl[ic]).fixed()) (*bl[ic]).refit();
       calc[ic].calcLines(lparams(),ind,calc[ic].exp_sp);
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
       mio<<"Fit failed\n";
       unfit();
       return;
     };

     if (fabs(sig-sig0)/sig < 1e-5) {
       ngood++;
       if (ngood >= 5) break;
     }else if (ngood) ngood--;
     sig0 = sig;


  }; // --- end iter ---
//  if (out){
    for(size_t i=0;i<names.size();i++){ 
      mio<<names[i]<<' ';
      mio<<p0[i]<<' ';
      mio<<*p[i]<<' ';
      mio<<erro[i];
      mio<<'\n';
    };

  ret = sig;

//-----------------------------------------------------------------------------
  if (ret >= 0.){
    vector<double> p_new(np);
    for(size_t i=0;i<np;i++) {
      p_new[i] = *p[i];
      *p[i] = p0[i];
    };
//    erase(i1,i2);
    for(size_t i=0;i<np;i++) {
      *p[i] = p_new[i];
    };
    ic = 0;
    size_t i0;
    for(size_t is=0;is<spectra.size();is++){
      s = &spectra[is];
      if (!s->exp_sp || !s->fit) continue;
      for(ic=0;ic<calc.size();ic++){
        if (s->exp_sp != calc[ic].exp_sp) continue;
        calc[ic].calcLines(lparams(),-1,-1,s->exp_sp);
        i0 = s->index(calc[ic].wstart());
        for(size_t i=i0;i<i0+calc[ic].nop();i++){
          s->r[i] = calc[ic].rr(s->w(i));
          s->set(i,s->r[i],s->exp_sp->weight(i)*(s->exp_sp->rr(i)-s->r[i]));
        };
      };
    };
  };
  return;
}

void splist::set4efit(int jp,set4efit_data& dt){
     int il;
     if (jp < npe){
        for(size_t i=0;i<dt.ind->size();i++){
          il = (*dt.ind)[i];
          if (!lparams().fixed(il,0) && (*dt.eind)[i] == jp) 
            (*dt.lin)[il] = (*p[jp]) - (*dt.e0)[il];
        };
     }else if (dt.intf && jp < npe+npi){
        for(size_t i=0;i<dt.ind->size();i++){
          il = (*dt.ind)[i];
          if (!lparams().fixed(il,1) && (*dt.iind)[i] == jp) 
            (*dt.hi)[il] = (*p[jp])*(*dt.cint)[i];
        };
     };
};


void splist::prep4efit(){
  enlist en;
  make_ener(en,"e0");
  vector<int>& nl = *en.getInt("n");
  vector<double>& mint = *en.getDoubleNew("mint");
  int i,j,i1,i2;
  for(i=0;i<size();i++){
    j = en.index(q(i));
    if (height(i) > mint[j]) mint[j] = height(i);
  };
  en.sort_double("mint");
  en.flt_make();
  en.flt_enable();
  vector<int> ind;
  spectrum *s;
  bool big;
  float hh;
  string fn = owner->dir()+"efit_cmd.txt";
  ofstream fil(fn.c_str());
  fil<<"com "<<name<<'\n';
  fil<<".fixall 0011111111111\n";
  for(size_t is=0;is<spectra.size();is++)
    for(i=0;i<spectra[is].nop();i++) spectra[is].exp_sp->weight(i,1.);
  vector< pair<int,int> > weig0;
  vector<string> sn0;
  vector<int> resq;
  vector< vector<int> > enf(en.size());
  bool unfix_self;
  
  for(j=0;j<en.size();j++){
    en.flt_rm(j);
    //if (nl[j] < 2) continue;
    ind.clear();
    weig0.clear();
    sn0.clear();
    resq.clear();
    for(i=0;i<size();i++)
      if (q(i) == en.q(j)) {
        ind.push_back(i);
      };
    for(vector<int>::iterator k=ind.begin();k!=ind.end();k++){
      unfix_self = true;
    for(size_t is=0;is<spectra.size();is++){
      s = &spectra[is];
      if (!s->fit) continue;
      i1 = s->index(line(*k) - s->lwidth());
      i2 = s->index(line(*k) + s->lwidth());
      if (i1 < 0) i1 = 0;
      if (i2 >= s->nop()-1) i2 = s->nop()-1;
      if (i1 >= s->nop()-1 || i2 <= 0) continue;
      big = false;
      for(size_t ii=i1;ii<=i2;ii++){
        if (s->rr(ii) > s->hmax()) {big = true;break;};
      };
      lparams().init(s);
      lparams().init_line(*k,s);
      hh = lparams().value(*k,line(*k));
      double w0;
      if (hh < s->hmax() && hh > s->hmax()*0.1){
        en.flt_add(j);
        w0 = line(*k);
        if (is == 0 && *k != 0) for(int kk=*k-1;kk>=0;kk--)
          if (w0 - line(kk) < 0.04){
            size_t jq = en.index(q(kk));
            if (jq < en.size()) resq.push_back(jq);
            w0 = line(kk);
            unfix_self = false;
          };
        if (is == 0 && *k < size()-1) for(int kk=*k+1;kk<size();kk++)
          if (line(kk) - w0 < 0.04){
            size_t jq = en.index(q(kk));
            if (jq < en.size()) resq.push_back(jq);
            w0 = line(kk);
            unfix_self = false;
          };
        if (unfix_self && lparams().fixed(*k,2)) 
        if (s->rr(line(*k)-0.2)/hh <0.5 && s->rr(line(*k)+0.2)/hh <0.5)
           lparams().unfix(*k,2); //  ???? 
      };
      //  this treatment of 'big' lines is good when fitting to
      //  all spectra simultaneously
      if (big) {
        for(i=i1;i<=i2;i++) s->exp_sp->weight(i,-0.0001);
        weig0.push_back(make_pair(i1,i2));
        sn0.push_back(s->exp_sp->name);
      };
      //  if I want to fit one spectrum at a time I must do like this:
      //  if (big) {en.flt_rm(j); k=ind.end();break;};
    }};
    if (!en.allowed(j)) continue;
    enf[j].push_back(j);
    fil<<"#"<<en.q(j)<<"\n";
    for(size_t i=0;i<resq.size();i++){
      fil<<"# "<<en.q(resq[i])<<'\n';
      enf[j].push_back(resq[i]);
    };
    for(size_t is=0;is<weig0.size();is++){
      fil<<sn0[is]<<".weight "<<weig0[is].first<<' '<<weig0[is].second<<" -0.0001\n";
    };
    for(size_t i=0;i<ind.size();i++){
      if (!lparams().fixed(ind[i],2)) fil<<".unfix "<<ind[i]<<" 2\n";
    }
  };
  en.save(owner->dir()+"tmp.lev3");
  
  map<int,int> done;
  for(j=0;j<enf.size();j++){
    if (!enf[j].size() || done[j]) continue;
    ind.clear();
    for(vector<int>::iterator e=enf[j].begin();e!=enf[j].end();e++){
      if (!en.allowed(*e)) continue;
      fil<<"#"<<en.q(*e)<<"\n";
      done[*e]+=1;
      for(i=0;i<size();i++)
        if (q(i) == en.q(*e)) {
          ind.push_back(i);
        };
    };
    if (!ind.size()) continue;
    fil<<".efitl efit int slnt ";
    for(vector<int>::iterator k=ind.begin();k!=ind.end();k++)
       fil<<*k<<' ';
    fil<<'\n';
    fil<<"out\n";
  };
}

void splist::prep_q4efit(enlist& en,VJKG& qq,bool info){
  vector<int> ind;
  spectrum *s;
  bool big,doit=false;
  float hh;
  int i,j,i1,i2;
  for(size_t is=0;is<spectra.size();is++)
    for(i=0;i<spectra[is].nop();i++) spectra[is].exp_sp->weight(i,1.);
  vector< pair<int,int> > weig0;
  vector<string> sn0;
  vector<int> resq;
  
    if (info) mio<<'\n'<<qq<<'\n';
    ind.clear();
    weig0.clear();
    sn0.clear();
    resq.clear();
    j = en.index(qq);
    for(i=0;i<size();i++)
      if (q(i) == qq) {
        ind.push_back(i);
      };
    if (j>=en.size()){
      if (!ind.size()){
        mio<<"Cannot find state "<<qq<<" in the line-list "<<name<<'\n';
        return;
      };
      vector<double> *e0 = getDouble("e0");
      if (!e0){
        mio<<"Cannot evaluate energy of state "<<qq<<'\n';
        return;
      };
      double ee = line(ind[0])+(*e0)[ind[0]];
      double hi = height(ind[0]);
      for(vector<int>::iterator i=ind.begin();i!=ind.end();i++)
        if (height(*i) > hi){
          ee = line(*i)+(*e0)[*i];
          hi = height(*i);
        };
      en.add(qq,ee);
      en.sort_q();
      j = en.index(qq);
    };
    
    for(vector<int>::iterator k=ind.begin();k!=ind.end();k++)
    for(size_t is=0;is<spectra.size();is++){
      s = &spectra[is];
      if (!s->fit) continue;
      i1 = s->index(line(*k) - s->lwidth());
      i2 = s->index(line(*k) + s->lwidth());
      if (i1 < 0) i1 = 0;
      if (i2 >= s->nop()-1) i2 = s->nop()-1;
      if (i1 >= s->nop()-1 || i2 <= 0) continue;
      big = false;
      for(size_t ii=i1;ii<=i2;ii++){
        if (s->rr(ii) > s->hmax()) {big = true;break;};
      };
      lparams().init(s);
      lparams().init_line(*k,s);
      hh = lparams().value(*k,line(*k));
      double w0;
      if (hh < s->hmax() && hh > s->hmax()*0.1){
        if (lparams().fixed(*k,2)) 
        if (s->rr(line(*k)-0.2)/hh <0.5 && s->rr(line(*k)+0.2)/hh <0.5)
           lparams().unfix(*k,2); //  ???? 
        doit = true;
        w0 = line(*k);
        if (is == 0 && *k != 0) for(int kk=*k-1;kk>=0;kk--)
          if (w0 - line(kk) < 0.04){
            resq.push_back(kk);
            w0 = line(kk);
          }else break;
        if (is == 0 && *k < size()-1) for(int kk=*k+1;kk<size();kk++)
          if (line(kk) - w0 < 0.04){
            resq.push_back(kk);
            w0 = line(kk);
          }else break;
      };
      //  this treatment of 'big' lines is good when fitting to
      //  all spectra simultaneously
      if (big) {
        if (info) mio<<"Too big line "<<line(*k)<<' '<<height(*k)<<" for "<<s->exp_sp->name<<'\n';
        else{
          s->exp_sp->weight(s->index(line(*k)),-0.0001);
          weig0.push_back(make_pair(i1,i2));
          sn0.push_back(s->exp_sp->name);
        };
      };
      //  if I want to fit one spectrum at a time I must do like this:
      //  if (big) {en.flt_rm(j); k=ind.end();break;};
    };

//    if (!ind.size()) return;

    if (info){
      if (doit)  mio<<"Can fit\n";
      else
        mio<<"Cannot fit\n";
      for(vector<int>::iterator rk=resq.begin();rk!=resq.end();rk++)
        mio<<q(*rk)<<'-'<<q0(*rk)<<' '<<line(*rk)<<'\n';
      mio<<'\n';
    }else if (doit){    
      ostringstream fil;
      fil<<"efitl "<<en.name<<" int ";
      for(vector<int>::iterator k=ind.begin();k!=ind.end();k++)
         fil<<*k<<' ';
      cmd(fil.str());
    }else
      mio<<"Cannot fit\n";
}

} // Goblin
