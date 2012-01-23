#include "Goblin/spectrum.h"
#include "Goblin/lineparams.h"
#include "Goblin/ifun.h"
#include "Goblin/matrix.h"
#include "Goblin/mio.h"
#include "Goblin/gdata.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <ctime>

namespace Goblin
{

spectrum::spectrum():object(){
  wstart_ = wstop_ = 0.;
  wavcorr_ = 0;
  shift_ = 0;
  hminimum = 0.0001;
  hmaximum = 1000.;
  exp_sp = 0;
  fit = true;
  type(unknown);
  noise_ = hminimum;
  fun_ = 0;
  fft_rr0 = 0.;
  fft_ir0 = 0.;
  center_ = 0.;
  lwidth_ = 0.1;
  //color_ = rgb(0,0,255);
  //im_color_ = rgb(255,0,255);
  in_type(in_spectrum);
  wr_dat = false;
}
spectrum::~spectrum(){
  if (fun_) delete fun_;
}

fun_res spectrum::fun(string str){
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
  
  if (nam == "wstart"){
    ostr.precision(9);
    ostr << wstart();
    return ostr.str();
  };
  
  if (nam == "wstop"){
    ostr.precision(9);
    ostr << wstop();
    return ostr.str();
  };
  
  if (nam == "wsize"){
    ostr << weight_.size();
    return ostr.str();
  };
  
  if (nam == "weight") {
    double ww1;
    istr >> ww1;
    ostr<<weight(index(ww1));
    return ostr.str();
  };

  if (nam == "size" || nam == "nop"){
    ostr << nop();
    return ostr.str();
  };
  
  if (nam == "int") {
    double ww1,ww2;
    istr >> ww1>>ww2;
    ostr<<integrate(ww1,ww2);
    return ostr.str();
  };

  if (nam == "bounds") {
    if (!nop()) return string("0 0 0 0");
    double vMin=1e30,vMax=-1e30,uMin,uMax,du;
    uMin = wstart();
    uMax = wstop();
    du = (uMax - uMin)/20;
    for(size_t i=0;i<nop();i++){
      if (rr(i)>vMax) vMax = rr(i);
      if (iscomplex() && ir(i)>vMax) vMax = ir(i);
      if (rr(i)<vMin) vMin = rr(i);
      if (iscomplex() && ir(i)<vMin) vMin = ir(i);
    };
    ostr<<uMin-du<<' '<<uMax+du<<' '<<vMin<<' '<<vMax;
    return ostr.str();
  };

  return object::fun(str);			      
}

cmd_res spectrum::cmd(string str){
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
    if (who == "bl") return bline.cmd(itscom+' '+rest);
    return bad;
  };
  
  if (com=="wr_dat"){
    wr_dat = true;
    return ok;
  };
  
  if (com=="ro_dat"){
    wr_dat = false;
    return ok;
  };
  
//  if (com=="new_plot"){
//    string nam;
//    istr>>nam;
//    owner->cmd("plot.new graph spec spec spec."+nam);
//    ostringstream ostr;
//    canvas* c = owner->plt.foc_canv();
//    if (!c || c->type != "graph") return bad;
//    graphics& g = *(graphics*)c;
//    ostr<<"plot."<<nam<<".uv "<<g.uMin()<<' '<<g.uMax()<<' '<<g.vMin()<<' '<<g.vMax();
//    owner->cmd(ostr.str());
//    owner->cmd("plot.focus "+nam);
//    return repaint;
//  };

  //if (com=="color"){
  //  int r,g,b;
  //  istr>>r>>g>>b;
  //  color(rgb(r,g,b));
  //  return repaint;
  //};

  //if (com=="icolor"){
  //  int r,g,b;
  //  istr>>r>>g>>b;
  //  icolor(rgb(r,g,b));
  //  return repaint;
  //};

  if (com=="hweight"){
    weight_.resize(0);
    return repaint;
  };
  
  if (com=="weight"){
    int i1,i2;
    float wt,rmin,rmax;
    string srmin,srmax;
    istr>>i1>>i2>>wt>>srmin>>srmax;
    if (!srmin.empty() && !srmax.empty()){
      rmin = atof(srmin.c_str());
      rmax = atof(srmax.c_str());
    }else{
      rmin = rmax = 0.;
    };
    if (i1 > i2 || i1 > nop()-1 || i2 < 0) return bad;
    mio<<"weight "<<rmin<<' '<<rmax<<'\n';
    for(size_t i=size_t(i1);i<=size_t(i2);i++)
      if (rmin == rmax  || (r[i]>=rmin && r[i]<rmax)) {
        weight(i,wt);
      };
    return ok;
  };
  
  if (com=="weightw"){
    double ww1,ww2;
    int i1,i2;
    string wt,srmin,srmax;
    istr>>ww1>>ww2>>wt>>srmin>>srmax;
    i1 = index(ww1);
    i2 = index(ww2);
    ostringstream ostr;
    ostr<<"weight "<<i1<<' '<<i2<<' '<<wt;
    if (!srmax.empty()) ostr<<' '<<srmin<<' '<<srmax;
    return cmd(ostr.str());
  };
  
  if (com=="save"){
    if (save()) return ok;
    return bad;
  };

  if (com == "saveAs"){
    string fn;
    istr >> fn;
    save(fn);
    return ok;
  };

  if (com == "make"){
    string n;
    double ww0,ww1;
    istr >> ww0 >> ww1 >> n;
    if (n.find('.') != string::npos) make(ww0,ww1,atof(n.c_str()));
    else
      make(ww0,ww1,(size_t)atoi(n.c_str()));
    return ok;
  };
  
  if (com == "info"){
    mio<<"Spectrum "<<name<<'\n';
    mio<<strDouble(wstart(),4,12)<<' '<<strDouble(wstop(),4,12)<<' '<<nop()<<'\n';
    return ok;
  };

  if (com == "lines") {
    string lname;
    istr>>lname;
    obj_list<splist> lines(owner,"splist");
    splist* lp = lines(lname);
    if (!lp){
      mio<<"No line list "<<lname<<'\n';
      return bad;
    };
    calcLines(lp->lparams());
    return repaint;
  };

  if (com == "mix") {
    addMix(rest);
    return repaint;
  };
  
  if (com == "temper") {
    double tt;
    istr>>tt;
    exp_cond.T(tt);
    return repaint;
  };
  
  if (com == "length") {
    double ll;
    istr>>ll;
    exp_cond.L(ll);
    return repaint;
  };
  
  if (com == "type") {
    string typ;
    istr>>typ;
    if (typ == "ab") type(spectrum::absorbance);
    if (typ == "tr") type(spectrum::transmittance);
    if (typ == "em") type(spectrum::emission);
    if (typ == "un") type(spectrum::unknown);
    return repaint;
  };

  if (com == "in_type") {
    string typ;
    istr>>typ;
    if (typ == "sp") in_type(in_spectrum);
    if (typ == "bl") in_type(in_bline);
    return ok;
  };

  if (com == "fft") {
    fft();
    return repaint;
  };

  if (com == "ifft") {
    fft(-1);
    return repaint;
  };

  if (com == "conv") {
    mio<<str<<'\n';
    conv(rest);
    ir_.resize(0);
    return repaint;
  };

  if (com == "deconv") {
    conv(rest,'d');
    ir_.resize(0);
    return repaint;
  };

  if (com == "afun") {
    afun(rest);
    return repaint;
  };

  if (com == "scale" || com == "*") {
    float a;
    istr>>a;
    for(size_t i=0;i<nop();i++)
      if (iscomplex()) set(i,rr(i)*a,ir(i)*a);
      else
        r[i] *= a;
    return repaint;
  };

  if (com == "+") {
    float a,b = 0.;
    istr >>a >> b;
    for(size_t i=0;i<nop();i++)
      if (iscomplex()) set(i,rr(i)+a,ir(i)+b);
      else
        r[i] += a;
    return repaint;
  };

  if (com == "hlim") {
    float hlim;
    istr>>hlim;
    for(size_t i=0;i<nop();i++)
      if (abs(r[i]) > hlim) r[i] = hlim;
    return repaint;
  };

  if (com == "real") {
    ir_.resize(0);
    return repaint;
  };

  if (com == "cut") {
    double ww1,ww2;
    istr >> ww1>>ww2;
    cut(ww1,ww2);
    return repaint;
  };

  if (com == "ex"){
    double rr0=0.,ir0=0.;
    istr >> rr0 >> ir0;
    extend(1,rr0,ir0);
    return repaint;
  };

  if (com == "shift") {
     double dww;
     istr >> dww;
     mio<<"shifting spectrum "<<name<<" by "<<dww<<'\n';
     shift(dww);
     return repaint;
  };
  if (com == "noise") {
     float h;
     istr >> h;
     noise(h);
     return repaint;
  };
  if (com == "addnoise") {
     float h;
     istr >> h;
     add_noise(h);
     return repaint;
  };

  if (com == "wave_corr"){
    wave_corr();
    return ok;
  };

  if (com == "hmax") {
     float h;
     istr >> h;
     set_hmax(h);
     return repaint;
  };
  if (com == "hmin") {
     float h;
     istr >> h;
     set_hmin(h);
     return repaint;
  };

  if (com == "lwidth"){
    double lw;
    istr>>lw;
    lwidth(lw);
    return ok;
  };

  if (com == "cp1"){
    double w,dw;
    istr>>w>>dw;
    cp1_w = w;
    cp1_dw = dw;
    return ok;
  };

  if (com == "cp2"){
    double w,dw;
    istr>>w>>dw;
    cp2_w = w;
    cp2_dw = dw;
    return ok;
  };

  if (com == "bline") {
    string value;
    istr>>value;
    if (value == "erase"){
      size_t j;
      istr >> j;
      bline.erase(j);
      return repaint;
    };
    if (value == "insert"){
      double x,y,d;
      istr >> x;
      y = bline(x);
      d = bline.der(x);
      bline.insert(x,y,d);
      return repaint;
    };
    if (value == "add"){
      add(bline);
      return repaint;
    };
    if (value == "sub"){
      sub(bline);
      return repaint;
    };
    if (value == "clear"){
      bline.clear();
      return repaint;
    };
    if (value == "fix"){
      bline.fix();
      return repaint;
    };
    if (value == "unfix"){
      bline.unfix();
      return repaint;
    };
  };

  if (com == "div") {
    string sname;
    istr>>sname;
    obj_list<spectrum> sps(owner,"spectrum");
    spectrum* sp = sps(sname);
    if (!sp){
      mio<<"No spectrum "<<sname<<'\n';
      return bad;
    };
    spectrum &s = *sp;
    double x0,y0,x1,y1,r1;
    for(size_t i=0;i<nop();i++){
      x1 = s.rr(w(i));
      y1 = s.irr(w(i));
      r1 = x1*x1 + y1*y1;
      if (r1 == 0.) set(i,1e30,1e30);
      else{
        x0 = rr(i);
        y0 = ir(i);
        set(i,(x0*x1+y0*y1)/r1,(y0*x1-x0*y1)/r1);
      };
    };
    return repaint;
  };

  if (com == "out"){
    string fn,w1,w2;
    istr>>fn>>w1>>w2;
    if (fn.empty()) return bad;
    ofstream fil(fn.c_str());
    if (!fil) {
      mio<<"Cannot open file "<<fn<<'\n';
      return bad;
    };
    int i1,i2;
    if (w1.empty()) i1 = 0;
    else
       i1=index(atof(w1.c_str()));
    if (w2.empty()) i2 = nop()-1;
    else
       i2=index(atof(w2.c_str()));
    if (i2 <= i1) return bad;
    for(size_t i=i1;i<=i2;i++) {
      fil.precision(12);
      fil<<w(i)<<' '<<rr(i);
      if (iscomplex()) fil<<' '<<ir(i);
      fil<<'\n';
    };
    return ok;
  };

  if (com == "mark"){
    mark();
    return ok;
  };

  if (com == "rec"){
    recenter();
    return repaint;
  };

  if (com == "zero"){
    string ww1,ww2;
    istr>>ww1>>ww2;
    if (ww2.empty()) zero();
    else{
      int i1 = index(atof(ww1.c_str()));
      int i2 = index(atof(ww2.c_str()));
      if (i1 < 0) i1 = 0;
      if (i2 >=nop()) i2 = nop()-1;
      if (i2 < i1) return bad;
      for(int i=i1;i<=i2;i++)
        r[i] = 0.;
    };
    return repaint;
  };

  if (com == "mirror"){
    mirror();
    return repaint;
  };

  if (com == "fff") { 
    for(size_t i=0;i<nop();i++){
      r[i] = fabs(r[i]);
    };
    return repaint;
  };

  if (com == "ggg") { 
    for(size_t i=0;i<nop();i++){
      r[i] *= (i/2*2==i)?1.:-1.;
    };
    return repaint;
  };

  if (com == "cshift") { 
    double d;
    size_t n2 = nop()/2;
    for(size_t i=0;i<n2;i++){
      d = r[i];
      r[i] = r[n2+i];
      r[n2+i] = d;
    };
    return repaint;
  };

  if (com=="keypress"){
    cmd_res res;
    if (in_type() == in_bline) {
       res = bline.cmd(com+' '+rest);
       if (res.repaint()) send("renew",true);
       return res;
    };
//    int key;
//    string in_typ;
//    char sh;
//    istr>>key>>in_typ>>sh;
//    if (in_typ == "in"){
//      switch (key){
//        case key_K: return cmd("nextl");
//        case key_J: return cmd("prevl");
//        case key_Tab: return cmd("next");
//        case key_Slash: return cmd("center");
//        case key_Delete: {erase(curr()); return repaint;};
//      };
//      return bad;
//    };
    return bad;
  };
  

/*
  if (par == "repl") {
    size_t j;
    double h;
    if (value.empty()) return 0;
    j = atoi(value.c_str());
    if (j >= spectra.size()) return 0;
    istr>>h;
    spectrum &s0 = spectra[i];
    spectrum &s1 = spectra[j];
    if (s0.type() != spectrum::absorbance || s1.type() != spectrum::absorbance){
      mio<<"Must be absorbace spectra\n";
      return 0;
    };
    double dw = s0.delw();
    double x0,y0,x1,y1;
    for(size_t i=0;i<s0.nop();i++){
      if (s0.rr(i) > h) s0.r[i] = s1.rr(s0.w(i));
    };
    return 1;
  };

  if (par == "deriv"){
    s.deriv();
    return 1;
  };

  if (par == "tst"){
  //  simulating coadded scans
    int n = atoi(value.c_str());
    string nse; // noise
    istr>>nse;
    if (llists.size() < 1){
      mio<<"A linelist is needed\n";
      return 0;
    };
    if (spectra.size() < 2){
      cmd("new spectrum 0 10 10");
    };
    cmd("spectrum 0 lines 0");
    cmd("spectrum 0 addnoise "+nse);
    cmd("spectrum 1 copy 0 0 1000000,");
    mio<<"Noise "<<nse<<endl;
    for(size_t j=2;j<=n;j++){
      cmd("spectrum 1 lines 0");
//      cmd("spectrum 1 zero");
      cmd("spectrum 1 addnoise "+nse);
      cmd("spectrum 0 add 1");
      mio<<j<<endl;
    };
    cmd("spectrum 0 scale "+strDouble(1./double(n)));
    return 1;
  };

  if (par == "gal"){
    double w,aw,dw=s.delw(),f,pi=2*acos(0.);
    double y,z;
    istr >> y>>z;
    mio<<"gal: "<<y<<z<<endl;
    for(size_t i=0;i<s.nop();i++){
      w = s.w(i);
      aw = abs(w);
      f = exp(-aw+(1.-z*aw-exp(-z*aw))/(2*z*z));
      s.set(i,f*cos(pi*w/dw),f*sin(pi*w/dw));
      //s.r[i] = f;
    };
    return 1;
  };

  if (par == "spconv"){
    size_t j;
    j = atoi(value.c_str());
    if (j >= spectra.size()) return 0;
    s.conv(spectra[j]);
    return 1;
  };

  if (par == "symm"){
    s.symm();
    return 1;
  };

  if (par == "copy") {
    if (value.empty()) return 0;
    size_t j = atoi(value.c_str());
    double w1,w2;
    size_t k;
    istr >> w1 >> w2 >> value;
    if (value.empty()) k = 1;
    else
      k = atoi(value.c_str());
    if (k == 0 || k >= s.nop()) k = 1;
    s.copy(spectra[j],w1,w2,k);
    return 1;
  };

  if (par == "curl") {
    size_t j;
    if (value.empty()) return 0;
    j = atoi(value.c_str());
    if (j >= spectra.size()) return 0;
    spectrum &s0 = spectra[i];
    spectrum &s1 = spectra[j];
    double dw = s0.delw();
    double x0,y0,x1,y1,curl=0.;
    double w0,w1;
    w0 = plt.gcurr().uMin();
    w1 = plt.gcurr().uMax();
    size_t i0,i1;
    i0 = s0.index(w0);
    i1 = s0.index(w1);
    string fn = dir()+"tmp.txt";
    ofstream fil(fn.c_str());
    if (!fil){
      mio<<"Cannot open file "<<fn<<endl;
      return 0;
    };
    x0 = y0 = 0.;
    w0 = 0.;
    for(size_t i=i0;i<=i1;i++){
      x1 = s0.rr(i);
      y1 = s1.rr(s0.w(i));

      y1 -= x1;
      curl += (x1-x0)*(y1+y0)/2;
      fil<<x1<<' '<<y1<<endl;

//      w1 = s0.w(i);
//      if (x1 > 0.4 && x1 < 1.2){
//        w0 = exp(-4*y1)-exp(-4*x1);
//        fil<<w1<<' '<<w0<<endl;
//      }else
//        fil<<w1<<' '<<-w0<<endl;

      x0 = x1;
      y0 = y1;
    };
    mio<<"C="<<curl<<endl;
    return 0;
  };

  if (par == "mul") {
    size_t j;
    if (value.empty()) return 0;
    j = atoi(value.c_str());
    if (j >= spectra.size()) return 0;
    spectrum &s0 = spectra[i];
    spectrum &s1 = spectra[j];
    double dw = s0.delw();
    double x0,y0,x1,y1;
    bool intrp = false;
    if ((abs(s0.wstart()-s1.wstart()) > dw/10) ||
         (abs(s0.wstop()-s1.wstop()) > dw/10)) intrp = true;
    for(size_t i=0;i<s0.nop();i++){
      if (intrp){
        x1 = s1.rr(s0.w(i));
        y1 = s1.irr(s0.w(i));
      }else{
        x1 = s1.rr(i);
        y1 = s1.ir(i);
      };
      x0 = s0.rr(i);
      y0 = s0.ir(i);
      s0.set(i,(x0*x1-y0*y1),(y0*x1+x0*y1));
    };
    return 1;
  };

  if (par == "add") {
    size_t j;
    if (value.empty()) return 0;
    j = atoi(value.c_str());
    if (j >= spectra.size()) return 0;
    spectrum &s0 = spectra[i];
    spectrum &s1 = spectra[j];
    double dw = s0.delw();
    double x0,y0,x1,y1;
    bool intrp = false;
    if ((abs(s0.wstart()-s1.wstart()) > dw/10) ||
         (abs(s0.wstop()-s1.wstop()) > dw/10)) intrp = true;
    for(size_t i=0;i<s0.nop();i++){
      if (intrp){
        x1 = s1.rr(s0.w(i));
        y1 = s1.irr(s0.w(i));
      }else{
        x1 = s1.rr(i);
        y1 = s1.ir(i);
      };
      x0 = s0.rr(i);
      y0 = s0.ir(i);
      s0.set(i,x0+x1,y0+y1);
    };
    return 1;
  };

  if (par == "sub") {
    size_t j;
    if (value.empty()) return 0;
    j = atoi(value.c_str());
    if (j >= spectra.size()) return 0;
    spectrum &s0 = spectra[i];
    spectrum &s1 = spectra[j];
    double dw = s0.delw();
    double x0,y0,x1,y1;
    bool intrp = false;
    if ((abs(s0.wstart()-s1.wstart()) > dw/10) ||
         (abs(s0.wstop()-s1.wstop()) > dw/10)) intrp = true;
    for(size_t i=0;i<s0.nop();i++){
      if (intrp){
        x1 = s1.rr(s0.w(i));
        y1 = s1.irr(s0.w(i));
      }else{
        x1 = s1.rr(i);
        y1 = s1.ir(i);
      };
      x0 = s0.rr(i);
      y0 = s0.ir(i);
      s0.set(i,x0-x1,y0-y1);
    };
    return 1;
  };

  if (par == "smul") {
    size_t j,k;
    if (value.empty()) return 0;
    j = atoi(value.c_str());
    spline *sre=0,*sim=0;
    if (j >= spl.size()) return 0;
    sre = &spl[j];
    string sk;
    istr >> sk;
    if (sk.empty()) sim = 0;
    else {
      k = atoi(sk.c_str());
      if (k < spl.size()) sim = &spl[k];
      else
        sim = 0;
    };
    double x0,y0,x1,y1,x,dw=s.delw();
    x = s.wstart();
    for(size_t i=0;i<s.nop();i++,x+=dw){
      x1 = sre->val(x);
      y1 = sim?sim->val(x):0.;
      x0 = s.rr(i);
      y0 = s.ir(i);
      s.set(i,x0*x1-y0*y1,y0*x1+x0*y1);
    };
    return 1;
  };

  if (par == "sdiv") {
    size_t j,k;
    if (value.empty()) return 0;
    j = atoi(value.c_str());
    spline *sre=0,*sim=0;
    if (j >= spl.size()) return 0;
    sre = &spl[j];
    string sk;
    istr >> sk;
    if (sk.empty()) sim = 0;
    else {
      k = atoi(sk.c_str());
      if (k < spl.size()) sim = &spl[k];
      else
        sim = 0;
    };
    double x0,y0,x1,y1,r1,x,dw=s.delw();
    x = s.wstart();
    for(size_t i=0;i<s.nop();i++,x+=dw){
      x1 = sre->val(x);
      y1 = sim?sim->val(x):0.;
      r1 = x1*x1 + y1*y1;
      if (r1 == 0.) s.set(i,1e30,1e30);
      else{
        x0 = s.rr(i);
        y0 = s.ir(i);
        s.set(i,(x0*x1+y0*y1)/r1,(y0*x1-x0*y1)/r1);
      };
    };
    return 1;
  };

  if (par == "rr") {
    size_t j;
    if (value.empty()) j = spectra[i].nop()/2;
    else
      j = atoi(value.c_str());
    mio<<"r["<<j<<"]("<<spectra[i].w(j)<<")="<<spectra[i].rr(j)<<endl;
  };

  if (par == "rrot") {
     spectra[i].rrot();
     return 1;
  };

  if (par == "imadd") {
     spectra[i].imadd();
     return 1;
  };

  if (par == "ro") {
    spectra[i].readOnly(true);
    return 0;
  };

  if (par == "wr") {
    spectra[i].readOnly(false);
    return 0;
  };

  if (par == "wcorr") {
    double wc;
    wc = atof(value.c_str());
    spectra[i].wavcorr(wc);
    return 1;
  };

  if (par == "imul") {
    spectra[i].imul(rest);
    return 1;
  };

  if (par == "idiv") {
    spectra[i].imul(rest,'d');
    return 1;
  };
*/  
  return object::cmd(str); 
};

bool spectrum::make(double w1,double w2,size_t n,spectrum *sp){
  if (w1 >= w2 || n <= 0) return false;
  wstart_ = w1;
  wstop_ = w2;
  shift_ = 0.;
  set_wavcorr(0);
  cp1_w = wstart();
  cp1_dw = 0.;
  cp2_w = wstop();
  cp2_dw = 0.;
  r.resize(n);
//  bline.clear();
  if (bline.size()<2){
    bline.insert(wstart_,0,0);
    bline.insert(wstop_,0,0);
  };
  type(unknown);
  if (sp){
    set_hmin(sp->hmin());
    set_hmax(sp->hmax());
    type(sp->type());
    exp_cond.copy(sp->exp_cond);
    bline.copy(sp->bline);
    if (sp->afunQ()) {
      afun(sp->afun().input());
    };
    owner = sp->owner;
  };
  return true;
}
bool spectrum::make(double w1,double w2,double dw,spectrum *sp){
  if (w1 >= w2 || dw <= 0) return false;
  wstart_ = w1;
  r.resize(size_t((w2-w1)/dw));
  wstop_ = w1 + (nop()-1)*dw ;
  shift_ = 0.;
  wavcorr(0);
  cp1_w = wstart();
  cp1_dw = 0.;
  cp2_w = wstop();
  cp2_dw = 0.;
  type(unknown);
//  bline.clear();
  if (bline.size()<2){
    bline.insert(wstart_,0,0);
    bline.insert(wstop_,0,0);
  };
  if (sp){
    set_hmin(sp->hmin());
    set_hmax(sp->hmax());
    type(sp->type());
    exp_cond.copy(sp->exp_cond);
    bline.copy(sp->bline);
    if (sp->afunQ()) {
      afun(sp->afun().input());
    };
    owner = sp->owner;
  };
  return true;
}

bool spectrum::make(spectrum& sp){
  return make(sp.wstart(),sp.wstop(),sp.nop(),&sp);
}

size_t spectrum::index(double ww){
   double dw = delw();
   if (ww < wstart()) return 0;
   else if (ww > wstop()) return nop()-1;
   else
     return  dw?size_t((ww-wstart())/dw):0;
}

bool spectrum::load(string fn){
  size_t i = fn.find_last_not_of(' ');
  if (i!=string::npos && i>3 && fn.substr(i-3,4) == ".txt"){
    return loadASCII(fn);
  }else
    return loadGremlin(fn);
  return false;
}

bool spectrum::loadGremlin(const string fn){
  ifstream hdr(fn.c_str());
  char str[300],*c;
  double w1=0,w2=0,dw=0,wc = 0;
  double temper=0.,P,ZSum=0.,L=1.,sh=0.;
  size_t n,niso;
  int iso;
  int bocode;
  string si,sii;
  while(hdr.getline(str,299)){

     if (strstr(str,"END") || strstr(str,"end")) break;
     c = strchr(str,'=');
     if (!c) continue;
     *(c++) = '\0';

     if (strstr(str,"inftype") || strstr(str,"INFTYPE")){
       int n;
       sscanf(c,"%i",&n);
       if (n != 6){
         hdr.close();
         return false;
       };
     };


//     if (strstr(str,"id") || strstr(str,"ID")) strcpy(id,c);
     if (strstr(str,"wstart") || strstr(str,"WSTART")) sscanf(c,"%lf",&w1);
     if (strstr(str,"wstop") || strstr(str,"WSTOP")) sscanf(c,"%lf",&w2);
     if (strstr(str,"npo") || strstr(str,"NPO")) sscanf(c,"%li",&n);
     if (strstr(str,"delw") || strstr(str,"DELW")) sscanf(c,"%lf",&dw);
//     if (strstr(str,"resolutn") || strstr(str,"RESOLUTN")) sscanf(c,"%lf",&resolution);
//     if (strstr(str,"xaxis_is") || strstr(str,"XAXIS_IS")) strncpy(units,c,20);
     if (strstr(str,"wavcorr") || strstr(str,"WAVCORR")) sscanf(c,"%lf",&wc);
//     if (strstr(str,"pspect") || strstr(str,"PSPECT")) sscanf(c,"%lf",&pressure);
     if (strstr(str,"temper") || strstr(str,"TEMPER")) sscanf(c,"%lf",&temper);
     if (strstr(str,"bocode") || strstr(str,"BOCODE")) sscanf(c,"%i",&bocode);
     if (strstr(str,"length") || strstr(str,"LENGTH")) sscanf(c,"%lf",&L);
     if (strstr(str,"shift") || strstr(str,"SHIFT")) sscanf(c,"%lf",&sh);
     if (strstr(str,"afun") || strstr(str,"AFUN")) afun(string(c));
     if (strstr(str,"mix") || strstr(str,"MIX")) { // example: mix= 210   11:1340 12:1220
       addMix(string(c));
     };

  };
  hdr.close();

  char fn2[300];
  strcpy(fn2,fn.c_str());

//  if (make(w1+w1*wc,w2+w2*wc,n)){
  if (make(w1,w2,n)){
    set_wavcorr(wc);
    shift(sh);
    c = strchr(fn2,'.');
    if (c) strcpy(c+1,"DAT");
    else
      strcat(fn2,".DAT");
    mio<< fn2 <<"\n";
    ifstream dat(fn2,ios_base::binary);
    if (!dat) return false;
    // check for existance & try with "dat"
    // check if size of file >= sizeof(float)*nop
    float *rrr = new float[nop()],a;               //  beeeee
    char *ia,*ib;
    dat.read((char*)rrr,sizeof(float)*nop());           //  beeeee
    for(size_t i=0;i<nop();i++) {
      a = rrr[i];
      if (bocode == 1){
        ia = (char*) &a;
        ib = (char*) &rrr[i];
        *ib = *(ia+3);
        *(ib+1) = *(ia+2);
        *(ib+2) = *(ia+1);
        *(ib+3) = *(ia+0);
      };
      a = rrr[i];
      if (abs(a) <1e60)  r[i] = a;                              //  beeeee
      else
        if (a>0) r[i] = 1e60;
        else
          r[i] = -1e60;
    };
    delete[] rrr;                                //  beeeee
    dat.close();
    if (temper == 0.) temper = 296.;
    exp_cond.T(temper);
    exp_cond.L(L);
    mio<<"T="<<exp_cond.T()<<", L="<<L<<'\n';
  };

  cp1_w = wstart();
  cp1_dw = 0.;
  cp2_w = wstop();
  cp2_dw = 0.;

  c = strchr(fn2,'.');
  if (c) strcpy(c+1,"BLN");
  else
    strcat(fn2,".BLN");

  bline.load(string(fn2));
  return true;
}


//bool spectrum::draw(canvas& c){
//  if (c.type == "graph" && c.out_type == "def"){
//    if (iscomplex()) draw(*(graphics*)(&c),im_color_,true);
//    draw(*(graphics*)(&c),color_);
//    if (bline.cname(c.name)) bline.draw(c);
//  };
//  return true;
//}

//void spectrum::draw(graphics& g,rgb color,bool im){
//  g.clip_clientRect();
//  size_t i0 = index(g.uMin());
//  size_t i1 = index(g.uMax());

//  if (i0 >= nop()-1) return;
//  if (i1 <= 0) return;
//  if (i0 < 0) i0 = 0;
//  if (i1 >= nop()) i1 = nop()-1;
//  if (i0 > 0) --i0;
//  if (i1 < nop()-1) ++i1;
//  valarray<float> *ar;

//  if (im) ar = &ir_;
//  else
//    ar = &r;

//  size_t dn = (i1 - i0)/g.width();
//  if (  dn < 10 ) {
//     g.pen(color);
//     g.moveTo(g.X(w(i0)),g.Y(r[i0]));
//     for(size_t i=i0;i<=i1;i++) {
//       g.lineTo(g.X(w(i)),g.Y(double((*ar)[i])));
//     };
//  }else{
//     float hmin,hmax;
//     size_t i00;
//     int j0,j1;
//     j0 = (wstart() <= g.uMin())?0:int( (wstart()-g.uMin())/(g.uMax()-g.uMin())*g.width() );
//     j1 = (wstop() >= g.uMax())?g.width():int( (wstop()-g.uMin())/(g.uMax()-g.uMin())*g.width() );
//     if (j1 <= j0) return;
//     dn = (i1 - i0)/(j1-j0);

//     int iy1,iy2;
//     g.pen(color);
//     for(int j=j0;j<j1;j++){
//       hmin=1e32; hmax=-1e32;
//       i00 =i0 + (i1 - i0)*(j-j0)/(j1-j0);
//       for(size_t i=i00;i<=i00+dn;i++) {
//         if ((*ar)[i] < hmin) hmin = (*ar)[i];
//         if ((*ar)[i] > hmax) hmax = (*ar)[i];
//       };
//       //mio<<hmin<<' '<<hmax<<'\n';
//       iy1 = g.Y(double(hmin));
//       iy2 = g.Y(double(hmax));
//       if (iy1 == iy2) iy2++;
//       g.moveTo(j,iy1);
//       g.lineTo(j,iy2);
//     };
//  };

//}

//void spectrum::drawRes(graphics& g,spectrum& sp,rgb color){
///*  if ( wstart() != sp.wstart() || wstop() != sp.wstop() || nop() != sp.nop() ) return;
//  size_t i0 = index(g.uMin());
//  size_t i1 = index(g.uMax());
//  if (i0 >= nop()-1) return;
//  if (i1 <= 0) return;
//  if (i0 < 0) i0 = 0;
//  if (i1 >= nop()) i1 = nop()-1;
//  if (i0 > 0) --i0;
//  if (i1 < nop()-1) ++i1;

//  float h,h0=0;
//  size_t dn = (i1 - i0)/g.width();
//  if (  dn < 10 ) {
//     shape& s = g.addShape(shape::MLINE);
//     s.color = color;
//     s.moveTo(g.X(w(i0)),g.Y(r[i0]));
//     for(size_t i=i0;i<=i1;i++) {
////       h = sp.r[si0+i] - r[i0+i];
//       h = (sp.r[i]<=sp.hmax())?(sp.bline(w(i)) + sp.r[i] - r[i]):(sp.bline(w(i))+(1.-exp(-sp.r[i]+r[i]))*exp(-r[i]));
//       //h = (sp.r[i]<=sp.hmax())?(sp.bline(w(i)) + sp.r[i] - r[i]):(sp.bline(w(i)) + (sp.r[i] - r[i])/(exp(r[i])-1.));
//       if (fabs(h) > sp.hmax()) h /= sp.hmax();
//       h0 = h;
//       s.lineTo(g.X(w(i)),g.Y(double(h)));
//     };
//  }else{
//     float hhmin,hhmax;
//     size_t i00,si;
//     int j0,j1,sj0,sj1,sdn;
//     j0 = (wstart() <= g.uMin())?0:int( (wstart()-g.uMin())/(g.uMax()-g.uMin())*g.width() );
//     j1 = (wstop() >= g.uMax())?g.width():int( (wstop()-g.uMin())/(g.uMax()-g.uMin())*g.width() );
//     if (j1 <= j0) return;
//     dn = (i1 - i0)/(j1-j0);

//     shape& s = g.addShape(shape::LINES);
//     s.color = color;
//     for(int j=j0;j<j1;j++){
//       hhmin=1e32; hhmax=-1e32;
//       i00 =i0+(i1 - i0)*(j-j0)/(j1-j0);
//       for(size_t i=i00;i<=i00+dn;i++) {
//         h = (sp.r[i]<=sp.hmax())?(sp.bline(w(i)) + sp.r[i] - r[i]):(sp.bline(w(i))+(1.-exp(-sp.r[i]+r[i]))*exp(-r[i]));
//         //h = (sp.r[i]<=sp.hmax())?(sp.bline(w(i)) + sp.r[i] - r[i]):(sp.bline(w(i)) + (sp.r[i] - r[i])/(exp(r[i])-1.));
//         if (fabs(h) > sp.hmax()) h /= sp.hmax();
//         h0 = h;
//         if (h < hhmin) hhmin = h;
//         if (h > hhmax) hhmax = h;
//       };
//       s.moveTo(j,g.Y(double(hhmin)));
//       s.lineTo(j,g.Y(double(hhmax)));
//     };
//  };
//*/
//}


//void spectrum::drawRes(graphics& g,rgb color){
//  if (exp_sp) drawRes(g,*exp_sp,color);
//}

bool spectrum::save(){
  mio<<"Calling :save "<<fname<<' '<<write()<<'\n';
  if (fname.empty()) return false;
  if (!wr_dat) {
    string fn2(fname);
    size_t i = fn2.rfind('.');
    if (i != string::npos) fn2.erase(i);
    fn2 += ".BLN";
    bline.save(fn2.c_str());
    mio<<"Saving "<<fn2<<"...\n";
  }else{
    mio<<"Saving "<<name<<"...\n";
    return save(fname);
  };
  return true;
}

bool spectrum::save(string fn){
  char fn2[300];
  strcpy(fn2,fn.c_str());
  char *c = strchr(fn2,'.');
  int Iso;
  if (c) strcpy(c+1,"HDR");
  else
    strcat(fn2,".HDR");

  ofstream hdr(fn.c_str());
  hdr.precision(14);
  hdr << "wstart="<<wstart_<<'\n';
  hdr << "wstop="<< wstop_<<'\n';
  hdr << "npo="<< nop()<<'\n';
  hdr << "delw="<< delw()<<'\n';
  hdr << "shift="<< shift()<<'\n';
  hdr << "wavcorr="<< wavcorr()<<'\n';
  hdr << "BOCODE="<< 0 <<'\n';
  hdr << "temper="<< exp_cond.T() << '\n';
  hdr << "length="<< exp_cond.L() << '\n';
  for(size_t i=0;i<exp_cond.nmix();i++){
    hdr << "MIX=" << exp_cond.P(exp_cond.Iso(i,0)) << ' ';
    for(size_t j=0;j<exp_cond.niso(i);j++)
      hdr << exp_cond.Iso(i,j)<<':'<<exp_cond.Z(i,j) << ' ';
    hdr << '\n';
  };
  if (afunQ()) hdr << "afun=" << afun().input()<<'\n';
  hdr.close();



  strcpy(fn2,fn.c_str());
  c = strchr(fn2,'.');
  if (c) strcpy(c+1,"DAT");
  else
    strcat(fn2,".DAT");

  ofstream dat(fn2,ios_base::binary);
  float *rrr = new float[nop()];           //  beeeee
  for(size_t i=0;i<nop();i++) rrr[i] = r[i]; //  beeeee
  dat.write((char*)rrr,sizeof(float)*nop());                      //  beeeee
  delete[] rrr;                             //  beeeee
  dat.close();

  c = strchr(fn2,'.');
  if (c) strcpy(c+1,"BLN");
  else
    strcat(fn2,".BLN");

  bline.save(fn2);
  return true;
}

void spectrum::add(spline& bl){
  double w = wstart();
  double b;
  if (type() == absorbance)
    for(size_t i=0;i<nop();i++,w+=delw())
       r[i] += bl(w);
  if (type() == transmittance)
    for(size_t i=0;i<nop();i++,w+=delw()){
       b = bl(w);
       r[i] *= b>0?b:1.;
    };
}

void spectrum::sub(spline& bl){
  double w = wstart();
  double b;
  if (type() == absorbance)
    for(size_t i=0;i<nop();i++,w+=delw())
       r[i] -= bl(w);
  if (type() == transmittance)
    for(size_t i=0;i<nop();i++,w+=delw()){
       b = bl(w);
       r[i] /= b>0?b:1.;
    };
}

float spectrum::hweight(size_t i){
  if (r[i] <= hmax()) return 1.;
  else{
    return exp(-fabs(3*r[i]));
  };
}

void spectrum::weight(size_t i,float we){
  if (weight_.size()!=nop()) {
    weight_.resize(nop());
    for(size_t i=0;i<nop();i++) weight_[i]=hweight(i);
  };
  weight_[i]=we;
}

void spectrum::type(spectrum_type tp){
  float rr;
  double y;
  if (type() == transmittance && tp == absorbance){
     for(size_t i=0;i<nop();i++){
        rr = r[i];
        r[i] = -(rr>0?log(rr):log(hmin()));
     };
     if (bline.size()>1){
       for(size_t i=0;i<bline.size();i++){
         y = bline.v[i].y;
         if (y <= 0.) y = 1e-30;
         bline.v[i].y = -log(y);
         bline.v[i].d = -bline.v[i].d/y;
         bline.refit(i);
       };
     };
  };
  if (type() == absorbance && tp == transmittance){
     for(size_t i=0;i<nop();i++){
        rr = r[i];
        r[i] = exp(-rr);
     };
     if (bline.size()>1){
       for(size_t i=0;i<bline.size();i++){
         bline.v[i].y = exp(-bline.v[i].y);
         bline.v[i].d = -bline.v[i].y*bline.v[i].d;
         bline.refit(i);
       };
     };
  };
  type_ = tp;
  fft_rr0 = 0.;
  fft_ir0 = 0.;
}

/*
      way = 0  remove old data and replace with calculated with lines i1 - i2
      way = 1  add lines i1 - i2 to existing data
      way = -1 remove (subtract) lines i1-i2
*/
bool spectrum::calcLines(lineparams& lp,int i1,int i2,spectrum *sp,int way){
  spectrum_type this_type = type();
  obj_list<spectrum> sps(owner,"spectrum");
  if (sp && !sps.has(sp)){
    mio<<"Couldnt find spectrum\n";
    return false;
  };
  spectrum& s = sp?(*sp):(*this);
  if (way == 0) {
     zero();
     if (this_type == transmittance) {
        type(unknown);
        type(absorbance);
        add(lp,i1,i2);
        type(transmittance);
     }else
       add(lp,i1,i2);
     conv();
     add(bline);
  }else{
     if (this_type == transmittance){
       sub(bline);
       type(absorbance);
       add(lp,i1,i2,way);
       type(transmittance);
       add(bline);
     }else
       add(lp,i1,i2,way);
  };

  return true;
}

void spectrum::calcLines(lineparams& lp,int i1,int i2,int ii1,int ii2,spectrum *sp){
  obj_list<spectrum> sps(owner,"spectrum");
  if (sp && !sps.has(sp)){
    mio<<"Couldnt find spectrum\n";
    return;
  };
  spectrum_type this_type = type();
  spectrum& s = sp?(*sp):(*this);
  zero();
  if (this_type == transmittance) type(absorbance);
  add(lp,i1,i2);
  add(lp,ii1,ii2);
  if (this_type == transmittance) type(transmittance);
  conv();
  add(bline);
}

/*
     way = 1  calculate lines with indeces which are in ind vector
     way =-1  calculate lines with indeces which are not in ind vector
*/

bool spectrum::calcLines(lineparams& lp,vector<int>& ind,spectrum *sp,int way){
  spectrum_type this_type = type();
  obj_list<spectrum> sps(owner,"spectrum");
  if (sp && !sps.has(sp)){
    mio<<"Couldnt find spectrum\n";
    return false;
  };
  spectrum& s = sp?(*sp):(*this);
  zero();
  if (this_type == transmittance) {
        type(unknown);
        type(absorbance);
  };
  if (way == 1)
  for (size_t i=0;i<ind.size();i++){
      if (ind[i] < 0 || ind[i] >= lp.nol()) continue;
      add(lp,ind[i],ind[i]);
  }else{
    map<int,char> no;
    for(size_t i=0;i<ind.size();i++) no[ind[i]] = '!';
    for (size_t i=0;i<lp.nol();i++){
      if (no.find(i) == no.end()) add(lp,i,i);
    };
  };
  if (this_type == transmittance) type(transmittance);
  conv();
  add(bline);
  return true;
}

void spectrum::shift(double dw){
  shift_ += dw;
  w0 = wstart_ + shift_ + wavcorr_ * (wstart_ + shift_);
  w1 = wstop_ + shift_ + wavcorr_ * (wstop_ + shift_);
}

void spectrum::fft(spectrum& sp, int isign){
//void four1(float *data, unsigned long nn, int isign){
  size_t nn=sp.nop(),n,mmax,m,j,istep,i,ii,jj;
  double wtemp,wr,wpr,wpi,wi,theta;
  float tempr,tempi;
  double X;

  for(i=0;i<100;i++)
    if (!nn){
      if (i == 0) return;
      j = i - 1;
//      mio<<"fft: j="<<j<<'\n';
      break;
    }else {
//      mio<<i<<' '<<nn<<'\n';
      nn >>= 1;
    };
  nn = 1 << j;

  if (nn < sp.nop()) nn <<= 1;
  X = 1./(sp.delw()?sp.delw():0.000001);

  double new_dw = X/nn;
  make(-X/2,X/2-new_dw,nn,&sp);

  ir_.resize(nn);
  ir_ = 0.;
//  mio<<"fft: "<<wstart()<<' '<<wstop()<<' '<<delw()<<' '<<nop()<<'\n';

  if (type()==transmittance) {
    fft_rr0 = (fft_rr0 ==0.)?1.:0;
  }else{
    fft_rr0 = 0.;
  };


  size_t np,nm,nop2,nn2;
  nop2 = sp.nop()/2;
  nn2 = nn/2;
  if (isign > 0){ // direct fourier transform
    np = nn2;
    nm = nn2;
    r[0] = (nn == sp.nop())?sp.r[0]:fft_rr0;
    ir_[0] = (nn == sp.nop())?sp.ir(0):fft_ir0;
    r[nn2] = sp.r[nop2];
    ir_[nn2] = sp.ir(nop2);
  }else{         // inverse fourier transform
    np = 0;
    nm = nn;
    r[nn2] = (nn == sp.nop())?sp.r[0]:fft_rr0;
    ir_[nn2] = (nn == sp.nop())?sp.ir(0):fft_ir0;
    r[0] = sp.r[nop2];
    ir_[0] = sp.ir(nop2);
  };


  for(i=1;i<nn2;i++){
    j = nop2 + i; // for positive frequencies in sp
    ii = np + i;
    if (j < sp.nop()) {
      r[ii] = sp.r[j];
      ir_[ii] = sp.ir(j);
    }else{
      r[ii] = fft_rr0;
      ir_[ii] = fft_ir0;
    };
    j = nop2 - i; // for negative frequencies in sp
    ii = nm - i;
    if (j < sp.nop()) {
      r[ii] = sp.r[j];
      ir_[ii] = sp.ir(j);
    }else{
      r[ii] = fft_rr0;
      ir_[ii] = fft_ir0;
    };
  };

  n = nn << 1;
  j = 1;

  for(i=1;i<n;i+=2){
    ii = i >> 1;
    if (j > i){
      jj = j >> 1;
      tempr = r[ii];
      r[ii] = r[jj];
      r[jj] = tempr;
      tempr = ir_[ii];
      ir_[ii] = ir_[jj];
      ir_[jj] = tempr;
    };
    m = n >> 1;
    while(m >= 2 && j > m){
      j -= m;
      m >>= 1;
    }
    j += m;
  }

  mmax = 2;


  while(n > mmax){
    istep = mmax << 1;
    theta = isign*(6.28318530717959/mmax);
    wtemp = sin(0.5*theta);
    wpr = -2.0*wtemp*wtemp;
    wpi = sin(theta);
    wr = 1.;
    wi = 0.;
    for(m=1;m<mmax;m+=2){
       for(i=m;i<=n;i+=istep){
          j = i + mmax;
          ii = i >> 1;
          jj = j >> 1;
          tempr = wr*r[jj]-wi*ir_[jj];
          tempi = wr*ir_[jj]+wi*r[jj];
          r[jj]=r[ii]-tempr;
          ir_[jj]=ir_[ii]-tempi;
          r[ii] += tempr;
          ir_[ii] += tempi;
       }
       wr = (wtemp=wr)*wpr-wi*wpi+wr;
       wi = wi*wpr+wtemp*wpi+wi;
    }
    mmax = istep;
  }


  if ( isign < 0 ) {
    r /= nn;
    ir_ /= nn;
  };


  if (isign > 0){
    n = nn/2;
    valarray<float> tmp(n-1);
    tmp[0] = r[n];
    r[n] = r[0];
    r[0] = tmp[0];
    for(i=0;i<tmp.size();i++) {
      tmp[i] = r[n+1+i];
      r[n+1+i] = r[i+1];
    };
    for(i=0;i<tmp.size();i++) {
      r[i+1] = tmp[i];
    };

    tmp[0] = ir_[n];
    ir_[n] = ir_[0];
    ir_[0] = tmp[0];
    for(i=0;i<tmp.size();i++) {
      tmp[i] = ir_[n+1+i];
      ir_[n+1+i] = ir_[i+1];
    };
    for(i=0;i<tmp.size();i++) {
      ir_[i+1] = tmp[i];
    };
  };


}

void spectrum::fft(int isign){
  spectrum sp;
  sp.make(*this);
  sp.r = r;
  if (iscomplex()){
    sp.ir_.resize(nop());
    sp.ir_ = ir_;
  };
  fft(sp,isign);
}

bool spectrum::loadASCII(const string fn){
  ifstream fil(fn.c_str());
  if (!fil) return false;
  size_t n,i,j,nc=0;
  double w1,w2,w,w0,dw;
  float b,bb;
  string str;

  getline(fil,str);
  istringstream istr(str);
  istr >> w1 >> w2 >> n >> nc;
  mio<<w1<<' '<<w2<<' '<<nc<<'\n';
  if (w2 <= w1 || nc > 3 || !nc) {
    mio.error("Spectrum input: Wrong file format");
    return false;
  };

  w = w0 = w1;
  i = 0;
  if (nc <= 1){
    make(w1,w2,n);
    while(getline(fil,str)){
      if (i >= n) {
        mio.error("Spectrum input: Inconsistent data 1");
        return false;
      };
      r[i++] = atof(str.c_str());
    };
  }else if (nc == 2){
    make(w1,w2,n);
//    while(getline(fil,str)){
    while(fil>> w >> b){
      if (i >= n) {
        mio.error("Spectrum input: Inconsistent data 2");
        return false;
      };
//      istr.str(str);
//      istr >> w >> b;
//      mio<<w<<' '<<b<<' '<<str<<'\n';
      r[i++] = b;
    };
//    cerr<<"n="<<n<<'\n';
  }else if (nc == 3){
    make(w1,w2,n);
    complex();
    while(fil>> w >> b >> bb){
      if (i >= n) {
        mio.error("Spectrum input: Inconsistent data 3");
        mio<<i<<' '<<n<<'\n';
        return false;
      };
      r[i]   = b;
      ir_[i++] = bb;
    };
  };
  if (i < n) {
     mio.error("Spectrum input: Inconsistent data");
     return false;
  };
  return true;
}

void spectrum::conv(const string fn,char way){
  size_t i = fn.find_first_not_of(' ');
  if (i == string::npos){
    if (!afunQ()) return;
    conv(afun(),way);
    return;
  };
  ifun *fun = 0;
  fun = new_ifun(fn);
  if (fun) {
    conv(*fun,way);
    delete fun;
  };
}

void spectrum::conv(ifun& fun,char way){
  double a,ww0=w0,ww1=w1;
  size_t nn = nop();
  spectrum_type this_type = type();
  
  if (this_type == absorbance) type(transmittance);
  fft();


  if (way == 'c'){
//    mio<<"conv...";
//    mio<<fun.tr(0);
    for(size_t i=0;i<nop();i++){
      a = fun.tr(w(i));
      r[i] *= a;
      ir_[i] *= a;
    };
//    mio<<"ok\n";
  }else{
    for(size_t i=0;i<nop();i++){
      a = fun.tr(w(i));
      if (abs(a) > 0.3){
        r[i] /= a;
        ir_[i] /= a;
      };
    };
  };

  fft(-1);
  if (this_type == absorbance) type(absorbance);
  double dw = delw();
  wstart_ = ww0 - dw*(nop()/2 - nn/2);
  wstop_  = ww1 + dw*(nop() - nn - (nop()/2 - nn/2));
  shift_ = 0.;
  wavcorr(0.);
}

void spectrum::conv(spectrum& sp){
  spectrum sf;
  sf.fft(sp);
  double ww0=w0,ww1=w1,ww;
  size_t nn = nop();
  fft();
  float x,y,a,b;
  for(size_t i=0;i<nop();i++){
    ww = w(i);
    x = r[i];
    y = iscomplex()?ir_[i]:0.;
    a = sf.rr(ww);
    b = sf.iscomplex()?sf.irr(ww):0.;
//    set(i,x*a-y*b,x*b+y*a);
    set(i,x*a+y*b,-x*b+y*a);
  };
  fft(-1);
  double dw = delw();
  wstart_ = ww0 - dw*(nop()/2 - nn/2);
  wstop_  = ww1 + dw*(nop() - nn - (nop()/2 - nn/2));
  shift_ = 0.;
  wavcorr(0.);
}

void spectrum::imul(const string fn,char way){
  ifun *fun = 0;
  istringstream istr(fn);
  string name;
  istr >> name;
  if (name == "box") {
    double wdt;
    istr >> wdt;
    if (wdt <= 0.) wdt = 0.00001;
    fun = new boxcar(wdt);
    goto go;
  };

 go:
  if (fun) {
    if (fun == 0) return;
    double a;

    if (way == 'c'){
      for(size_t i=0;i<nop();i++){
        a = fun->tr(w(i));
        r[i] *= a;
        ir_[i] *= a;
      };
    }else{
      for(size_t i=0;i<nop();i++){
        a = fun->tr(w(i));
        if (abs(a) > 0.01){
          r[i] /= a;
          ir_[i] /= a;
        };
      };
    };
    delete fun;
  };
}

void spectrum::afun(const string fn){
  if (fun_) delete fun_;
  fun_ = new_ifun(fn);
}

double spectrum::integrate(double ww1,double ww2){
  if (ww1 >= wstop()) return 0.;
  if (ww2 <= wstart()) return 0.;
  if (ww1 >= ww2) return 0.;
  size_t i1 = index(ww1);
  size_t i2 = index(ww2);
  double h = 0.;
//  for(size_t i=i1;i<=i2;i+=2) {
//      h += (r[i] + 4*r[i+1] + r[i+2])/3;
//  };
  for(size_t i=i1;i<=i2;i++) {
      h += r[i];
  };
  return delw()*h;
}

double spectrum::integrate_im(double ww1,double ww2){
  if (!iscomplex()) return 0.;
  if (ww1 >= wstop()) return 0.;
  if (ww2 <= wstart()) return 0.;
  if (ww1 >= ww2) return 0.;
  size_t i1 = index(ww1);
  size_t i2 = index(ww2);
  double h = 0.;
//  for(size_t i=i1;i<=i2;i+=2) {
//      h += (r[i] + 4*r[i+1] + r[i+2])/3;
//  };
  for(size_t i=i1;i<=i2;i++) {
      h += ir_[i];
  };
  return delw()*h;
}

void spectrum::add_noise(float nse){
  noise(nse);
  float a = nse/RAND_MAX;
  if (clock() == clock_t(-1)){
    mio<<"No clock()\n";
  }else
    srand(clock());
  mio<<rand()<<'\n';
  for(size_t i=0;i<nop();i++){
    r[i] += rand()*a - nse/2;
  };
}

void spectrum::cut(double xx1,double xx2){
  valarray<float> tmp;
  size_t i1,i2,n;
  double x1,x2;
  bool cmpx = iscomplex();
  if (xx2 <= xx1) return;
  x1 = xx1>wstart()?xx1:wstart();
  x2 = xx2<wstop()?xx2:wstop();
  i1 = index(x1);
  i2 = index(x2);
  x1 = w(i1);
  x2 = w(i2);
  n = i2 - i1 + 1;
  tmp.resize(n);
  for(size_t i=0;i<n;i++)  tmp[i] = r[i1 + i];
  r.resize(n);
  r = tmp;
  if (cmpx) {
    tmp.resize(n);
    for(size_t i=0;i<n;i++)  tmp[i] = ir_[i1 + i];
    ir_.resize(n);
    ir_ = tmp;
  }
  w0 = wstart_ = x1;
  w1 = wstop_ = x2;
  shift_ = 0.;
}

void spectrum::copy(spectrum& sp,double xx1,double xx2,size_t k){
  size_t i1,i2,n;
  double x1,x2;
  bool cmpx = sp.iscomplex();
  if (xx2 <= xx1) return;
  x1 = xx1>sp.wstart()?xx1:sp.wstart();
  x2 = xx2<sp.wstop()?xx2:sp.wstop();
  i1 = sp.index(x1);
  i2 = sp.index(x2);
  n = (i2 - i1)/k + 1;
  i2 = i1 + (n-1)*k;
  x1 = sp.w(i1);
  x2 = sp.w(i2);
  make(x1,x2,n,&sp);
  for(size_t i=0;i<n;i++)  r[i] = sp.r[i1 + k*i];
  if (cmpx) {
    complex();
    for(size_t i=0;i<n;i++)  ir_[i] = sp.ir_[i1 + k*i];
  }
}

void spectrum::tst(){
  complex();
  size_t n2 = nop()/2;
  w0 = -0.05*n2;
  w1 =  0.05*(n2-1);
  shift_ = 0.;
  double pi = 2*acos(0.)/delw()*1;
  double x,f,y,z,x1;
  y = 1.;
  z = 0.0001;
  for(size_t i=0;i<nop();i++){
     x = w(i);
     x1 = abs(x);
     f = 100*exp(-y*x1*x1/*+(1.-z*x1-exp(-z*x1))/(2*z*z)*/);
     r[i] = f*cos(pi*x);
     ir_[i] = f*sin(pi*x);
  };
}


void spectrum::set(size_t i,double x,double y){
  if (!iscomplex()) complex();
  if (i<nop()) {
    r[i] = x;
    ir_[i] = y;
  };
}


void spectrum::add(lineparams& lp,size_t ii1,size_t ii2,int way){

  lp.init(this); // initialize variables of lp common for all lines

  size_t j0,j;
  float hh;
  double ww,ddw,dlw=delw();
  bool leftStop=false, rightStop=false;
  size_t i,i1,i2,ni=lp.nol();
  if (!ni) return;
  i1 = ii1<ni?ii1:0;
  i2 = ii2<ni?ii2:(ii1<ni?i1:ni-1);
  for(size_t i=i1;i<=i2;i++){

     if (!lp.allowed(i)) continue;
     lp.init_line(i,this); // initialize variables specific for this line

     ww = lp.line(i);
     j0 = index(ww);
     ww = w(j0);
     leftStop=false;
     rightStop=false;
     for(j=0;;j++){
       if (j0 + j >= nop()) rightStop = true;
       if (int(j0) - int(j) < 0) leftStop = true;
       if (leftStop && rightStop) break;

       hh = lp.value(i,w(j0+j));
//  if (i>730 && i< 734) mio<<"calc "<<w(j0+j)<<' '<<hh<<"\n";
       if (fabs(hh) < hmin()) break;
       if (!rightStop) r[j0+j] += (way<0)?-hh:hh;

       hh = lp.value(i,w(j0-j));
       if (!leftStop && j)  r[j0-j] += (way<0)?-hh:hh;
     };
  };
}

float spectrum::val(lineparams& lp,size_t ii1,size_t ii2,double ww){

  lp.init(this); // initialize variables of lp common for all lines

  float hh=0.;
  size_t i1,i2,ni=lp.nol();
  if (!ni) return 0.;
  i1 = ii1<ni?ii1:0;
  i2 = ii2<ni?ii2:(ii1<ni?i1:ni-1);
  for(size_t i=i1;i<=i2;i++){
     lp.init_line(i,this); // initialize variables specific for this line
     hh += (float)lp.value(i,ww);
  };
  return hh;
}


void spectrum::rrot(){
  if (!iscomplex()) return;
  float re,im,a;
  for(size_t i=0;i<nop();i++){
    re = rr(i);
    im = ir(i);
    a = re?atan(im/re):1.5707963;
    r[i] = re*cos(a)+im*sin(a);
    ir_[i]=0.;
  };
}

void spectrum::imadd(){
  if (!iscomplex()) return;
  for(size_t i=0;i<nop();i++){
    r[i] += ir_[i];
    ir_[i]=0.;
  };
}


string spectrum::print(){
  stringstream ostr;
  if (name.empty()) ostr << "Spectrum ";
  else
     ostr<<name<<' ';
  ostr<<wstart()<<' '<<wstop()<<' '<<nop()<<" bl["<<bline.curr()<<']';
  if (bline.fixed()) ostr<<"* ";
  ostr<<' '<<bline.currX(); if (bline.fixed(bline.curr(),0)) ostr<<'*';
  ostr<<' '<<bline.currY(); if (bline.fixed(bline.curr(),1)) ostr<<'*';
  ostr<<' '<<bline.currD(); if (bline.fixed(bline.curr(),2)) ostr<<'*';
  return ostr.str();
}


// k is extension factor, rr0 & ir0 are filling values
void spectrum::extend(int k,float rr0,float ir0){
  size_t i,j,nn=nop(),nl,nr,dn;
  for(i=0;i<100;i++)
    if (!nn){
      if (i == 0) return;
      j = i - 1;
      break;
    }else {
      nn >>= 1;
    };
  nn = 1 << (j + k);
  mio<<"ex: nn="<<nn<<(iscomplex()?" complex ":" real ")<<rr0<<' '<<ir0<<'\n';
  dn = nn - nop();
  nr = dn / 2;
  nl = nr + (2*nr!=dn?1:0);
  valarray<float> newr(nn);
  valarray<float> newi(iscomplex()?nn:0);
  for(i=0;i<nl;i++){
    newr[i] = rr0;
    if (iscomplex()) newi[i] = ir0;
  };
  for(i=0;i<nop();i++){
    newr[nl+i] = r[i];
    if (iscomplex()) newi[nl+i] = ir(i);
  };
  for(i=nl+nop();i<nn;i++){
    newr[i] = rr0;
    if (iscomplex()) newi[i] = ir0;
  };
  double neww1,neww2;
  neww1 = wstart() - nl*delw();
  neww2 = wstop() + nr*delw();
  shift_ = 0.;
  wstart_ = neww1;
  wstop_ = neww2;
  wavcorr(0.);
  r.resize(nn);
  r = newr;
  if (newi.size()){
    ir_.resize(nn);
    ir_ = newi;
  };
}

void spectrum::mark(){
  size_t nn = nop()/2;
  center_ = wstart() + delw()*nn;
}

void spectrum::recenter(){
  double width = wstop() - wstart();
  double dw = delw();
  size_t nn = nop()/2;
  wstart_ = center_ - dw*nn;
  wstop_ = wstart_ + width;
  shift_ = 0.;
  wavcorr(0.);
}

void spectrum::symm(){
  if (iscomplex()) ir_.resize(0);
  size_t nn = 2*nop(),n=nop();
  double dw = delw();
  valarray<float> nr(nn);
  for(size_t i=1;i<n;i++){
    nr[i] = nr[nn-i] = r[n-i];
  };
  nr[0] = 0.;
  nr[n] = r[0];
  r.resize(nn);
  r = nr;
  double ww0 = wstart(),ww1 = wstop();
  wstart_ = 2*ww0 - ww1 - dw;
  shift_ = 0.;
  wavcorr(0.);
}

void spectrum::mirror(){
  size_t n = nop();
  valarray<float> r1(n),ir1;
  r1 = r;
  if (iscomplex()){
    ir1.resize(n);
    ir1 = ir_;
    ir_.resize(2*n);
  }else
    ir_.resize(0);
  r.resize(2*n);
  r[n] = r1[0];
  for(size_t i=1;i<n;i++){
    r[n + i] = r[n - i] = r1[i];
    if (iscomplex()) {
      ir_[n + i] = ir1[i];
      ir_[n - i] = -ir1[i];
    };
  };
}

void spectrum::wave_corr(){
  double d,D;
  d = (cp2_dw - cp1_dw)/(cp2_w - cp1_w);
  D = (cp1_dw - d*cp1_w)/(1. + d);
  double ww0 = wstart();
  double ww1 = wstop();
  shift_ = 0.;
  wavcorr_ = 0.;
  wstart_ = ww0 + D + d*(ww0 + D);
  wstop_ = ww1 + D + d*(ww1 + D);
  wavcorr(0.);
  
  mio<<d<<' '<<D<<' '<<wstart_<<'\n';
}

void spectrum::deriv(){
  mark();
  fft();
  double dx=delw(),x;
  float r0,ir0;
  if (!iscomplex()) return;
  for(size_t i=0;i<nop();i++){
    x = dx*i;
    r0 = r[i];
    ir0 = ir_[i];
    r[i] = ir0*float(x);
    ir_[i] = -r0*float(x);
  };
  fft(-1);
  recenter();
  ir_.resize(0);
}

void spectrum::addMix(string c){
  double temper=0.,P,ZSum=0.,L=1.;
  istringstream istr(c);                                     // P   iso:Z
  string si,sii;
  size_t i;
  istr >> P;
  exp_cond.addMix(P);
  while(istr >> si){
     ZSum = 1.;
     i = si.find(':');
     if (i != string::npos) {
            sii = si.substr(0,i);
            si.erase(0,i+1);
            ZSum = atof(si.c_str());
     }else
           sii = si;
     exp_cond.addIso(atoi(sii.c_str()));
     exp_cond.Z(ZSum);
  };
};

//cmd_res spectrum::mouseClick(canvas* c,int x,int y,int shft){
//  cmd_res ok,bad(false),repaint;
//  repaint.repaint(true);
//  if (c->type != "graph") return bad;
//  graphics& g = *(graphics*)c;
//  cmd_res res;
//  if (in_type() == in_bline) {
//    res = bline.mouseClick(c,x,y,shft);
//    if (res.repaint()) send("bl_ch",true);
//    return res;
//  };
//  return ok;
//}

} // Goblin
