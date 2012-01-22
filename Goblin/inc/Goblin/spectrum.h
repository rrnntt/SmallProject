#ifndef SPECTRUM_H
#define SPECTRUM_H

#include "Goblin/DllExport.h"
#include "Goblin/spline.h"
#include "Goblin/exp_cond.h"
#include "Goblin/ifun.h"

#include <valarray>
#include <string>
#include "Goblin/object.h"
#include "Goblin/utils.h"

namespace Goblin
{

class lineparams;

class GOBLIN_EXPORT spectrum:public object {
  double wstart_,w0;
  double wstop_,w1;
  double wavcorr_;
  double shift_;
  double center_;
  float hminimum;
  float hmaximum;
  float noise_;
  double lwidth_;
  valarray<float> ir_,weight_;
  ifun *fun_,dlt_;
  float fft_rr0,fft_ir0;
  bool wr_dat;
public:
  //rgb color_,im_color_;
  double cp1_w,cp1_dw,cp2_w,cp2_dw;
  enum spectrum_type {unknown,absorbance,transmittance,emission};
  enum input_type {in_spectrum,in_bline};
  spectrum_type type_;
  input_type in_type_;
  valarray<float> r;
  spline bline;
  Exp_cond exp_cond;
  spectrum();
  ~spectrum();
  spectrum *exp_sp;
  bool fit;
  void wr_data(bool boo){wr_dat=boo;}
  bool wr_data(){return wr_dat;}
  spectrum_type type(){return type_;}
  void type(spectrum_type tp);
  input_type in_type(){return in_type_;}
  void in_type(input_type tp){in_type_ = tp;}
  double wstart(){return w0;}
  double wstop(){return w1;}
  double wavcorr(){return wavcorr_;}
  void set_wavcorr(double ww){wavcorr(ww);}
  void wavcorr(double ww){
     wavcorr_ = ww;
     w0 = wstart_ + shift_ + ww * (wstart_ + shift_);
     w1 = wstop_ + shift_ + ww * (wstop_ + shift_);
  }
  void wave_corr();
  double delw(){return (nop()>1)?(wstop()-wstart())/(nop()-1):0.;}
  size_t nop(){return r.size();}
  double w(size_t i){return wstart() + delw()*i;}
  size_t index(double ww);
  float rr(double ww){
    if (ww < wstart() || ww >= wstop() || delw() <= 0.) return 0.;
    size_t i = index(ww);
    return r[i] + (r[i+1]-r[i])*(ww - w(i))/delw();
  }
  float irr(double ww){
    if (!ir_.size()) return 0.;
    if (ww < wstart() || ww >= wstop() || delw() <= 0.) return 0.;
    size_t i = index(ww);
    return ir_[i] + (ir_[i+1]-ir_[i])*(ww - w(i))/delw();
  }
  float rr(size_t i){return r[i];}
  float ir(size_t i){return ir_.size()?ir_[i]:0.;}
  void ir(size_t i,float a){if (ir_.size()<nop()) ir_.resize(nop());ir_[i]=a;}
  bool iscomplex(){return ir_.size()==nop();}
  void complex(){if(!iscomplex()){ir_.resize(nop());ir_=0.;};}
  bool make(double w1,double w2,size_t n,spectrum *sp=0);
  bool make(double w1,double w2,double dw,spectrum *sp=0);
  bool make(spectrum& sp);
  void zero(){r = 0;if (ir_.size()) ir_ = 0;}
  bool load(string fn);
  cmd_res cmd(string str);
  fun_res fun(string str);
  bool loadGremlin(const string fn);
  bool loadASCII(const string fn);
//  bool draw(canvas& c);
//  void draw(graphics& g,rgb color = rgb(1,0,0),bool im=false);
//  void drawRes(graphics& g,spectrum& sp,rgb color = rgb(255,0,0));
//  void drawRes(graphics& g,rgb color = rgb(255,0,0));
//  cmd_res mouseClick(canvas* c,int x,int y,int shft=0);
//  void color(rgb col){color_=col;}
//  void icolor(rgb col){im_color_=col;}

  float hmin(){return hminimum;}
  void set_hmin(float h){hminimum = h;}
  float hmax(){return hmaximum;}
  void set_hmax(float h){hmaximum = h;}
  float noise(){return noise_;}
  void noise(float nse){noise_=nse;}
  double lwidth(){return lwidth_;}
  void lwidth(double lw){lwidth_=lw;}
  bool save(string fn);
  bool save();
  void add(lineparams& lp,size_t i1,size_t i2,int way=1);
  float val(lineparams& lp,size_t i1,size_t i2,double ww);
  void add(spline& bl);
  void sub(spline& bl);
  float hweight(size_t i);
  float weight(size_t i){return weight_.size()==nop()?weight_[i]:hweight(i);}
  void weight(size_t i,float we);
  bool calcLines(lineparams& lp,int i1=-1,int i2=-1,spectrum *sp=0,int way=0);
  bool calcLines(lineparams& lp,spectrum *sp,int i1=-1,int i2=-1){return calcLines(lp,i1,i2,sp);}
  void calcLines(lineparams& lp,int i1,int i2,int ii1,int ii2,spectrum *sp=0);
  bool calcLines(lineparams& lp,vector<int>& ind,spectrum *sp,int way=1);
//  void addLines(lineparams& lp,size_t i1=-1,size_t i2=-1,spectrum *sp=0);
  double shift(){return shift_;}
  void shift(double dw);
  void fft(spectrum& sp, int isign=1);
  void fft(int isign=1);
  void conv(spectrum& sp);
  void conv(ifun& fun,char way='c');
  void conv(const string fn,char way='c'); // 'c' - for convolution, 'd' - for deconvolution
  void conv(){conv("");}
  void afun(const string fn);
  ifun& afun(){return fun_?(*fun_):dlt_;}
  bool afunQ(){return fun_!=0;}
  double integrate(double ww1,double ww2);
  double integrate_im(double ww1,double ww2);
  void add_noise(float nse);
  void cut(double x1,double x2);
  void copy(spectrum& sp,double x1=-1e30,double x2=1e30,size_t n=1);
  void tst();
  void set(size_t i,double x,double y);
  void imul(const string fn,char way='c');
  void rrot();
  void imadd();
  string print();
  void mark();
  double center(){return center_;}
  void recenter();
  void extend(int k,float rr0=0.,float ir0=0.);
  void symm();
  void mirror();
  void deriv();
  void addMix(string);
};

} // Goblin

#endif
