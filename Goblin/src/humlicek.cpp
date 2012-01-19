#include <stdlib.h>
#include <math.h>
#include <complex>
#include "../Troll1/mio.h"

typedef complex<double> double_complex;

inline double_complex  APPROX1(double_complex T){
  return (T * 0.5641896) / (0.5 + (T * T));
}

inline double_complex  APPROX2(double_complex T,double_complex U){
  return (T * (1.410474 + U*.5641896))/ (.75 + (U *(3.+U)));
}

inline double_complex  APPROX3(double_complex T){
  return ( 16.4955 + T * (20.20933 + T*(11.96482 + T * (3.778987 + 0.5642236*T))))
          / ( 16.4955 + T * (38.82363 + T*(39.27121 + T * (21.69274 + T * (6.699398 + T)))));
}

inline double_complex  APPROX4(double_complex T,double_complex U){
  return (T * (36183.31 - U * (3321.99 - U * (1540.787 - U
              *(219.031 - U *(35.7668 - U *(1.320522 - U * .56419))))))
            / (32066.6 - U * (24322.8 - U * (9022.23 - U * (2186.18
               - U * (364.219 - U * (61.5704 - U * (1.84144 - U))))))));
}



/*inline double humlicek(double xx,double dl,double dd){
  static double s1 = sqrt(log(2.));
  static double s2 = 2*sqrt(log(2.));
  double y = s1*dl/((dd>0)?dd:0.000001);
  double x = s2*xx/((dd>0)?dd:0.000001);
  return humlicek(x,y);
}
*/

double humlicek(double x,double y){

      double s,ax;
      double_complex   t, u, v;

      if (y > 15.) {
         t = double_complex(y,-x);
         v = APPROX1(t);
      } else if (y < 15. && y >= 5.5) {
         t  = double_complex(y,-x);
         s  = abs(x) + y;
         if (s >= 15.) v = APPROX1(t);
         else{
            u = t * t;
            v = APPROX2(t,u);
         };
      } else if (y < 5.5 && y > 0.75) {
           t  = double_complex(y,-x);
           s  = abs(x) + y;
           if (s >= 15.) v = APPROX1(t);
           else if (s < 5.5) v = APPROX3(t);
           else{
              u = t * t;
              v = APPROX2(t,u);
           };
      } else {
           t  = double_complex(y,-x);
           ax = abs(x);
           s  = ax + y;
           if (s >= 15.) v= APPROX1(t);
           else if (s < 15.0 && s >= 5.5) {
               u = t * t;
               v = APPROX2(t,u);
           } else if (s < 5.5 && y >= (0.195*ax - 0.176)) v = APPROX3(t);
           else{
              u  = t * t;
              v = exp(u) - APPROX4(t,u);
           };
      };


      if (y == 0)
         v = double_complex(exp(-x*x), v.imag());

      return v.real();
}

double_complex cprob(double x,double y){

      double s,ax;
      double_complex   t, u, v;

      if (y > 15.) {
         t = double_complex(y,-x);
         v = APPROX1(t);
      } else if (y < 15. && y >= 5.5) {
         t  = double_complex(y,-x);
         s  = abs(x) + y;
         if (s >= 15.) v = APPROX1(t);
         else{
            u = t * t;
            v = APPROX2(t,u);
         };
      } else if (y < 5.5 && y > 0.75) {
           t  = double_complex(y,-x);
           s  = abs(x) + y;
           if (s >= 15.) v = APPROX1(t);
           else if (s < 5.5) v = APPROX3(t);
           else{
              u = t * t;
              v = APPROX2(t,u);
           };
      } else {
           t  = double_complex(y,-x);
           ax = abs(x);
           s  = ax + y;
           if (s >= 15.) v= APPROX1(t);
           else if (s < 15.0 && s >= 5.5) {
               u = t * t;
               v = APPROX2(t,u);
           } else if (s < 5.5 && y >= (0.195*ax - 0.176)) v = APPROX3(t);
           else{
              u  = t * t;
              v = exp(u) - APPROX4(t,u);
           };
      };


      if (y == 0)
         v = double_complex(exp(-x*x), v.imag());

      return v;
}

double galatryG(double x,double y,double z){
  double zz=z,pi=sqrt(2*acos(0.)),z4=4*pow(z,0.868);
  double_complex c[9];
  double_complex w=cprob(x,y),h,q=double_complex(x,y);
//  static int reg=0;
  c[0]=1.;
  c[1]=c[2]=0.;
  c[3]= double_complex(0.,1.)*zz/12.; zz*=z;
  c[4]=-zz/48.; zz*=z;
  c[5]= double_complex(0.,-1.)*zz/240.; zz*=z;
  c[6]= zz/1440.+c[3]*c[3]/2.; zz*=z;
  c[7]= double_complex(0.,1.)*zz/10080.+c[3]*c[4]; zz*=z;
  c[8]=-zz/80640.+c[3]*c[5]+c[4]*c[4]/2.;
  if ( (y<=0.5) && (z<=0.04) && fabs(x)<=2.){ // region I
//    if (reg!=1){
//      reg=1;
//      mio<<"Region I "<<x<<' '<<y<<' '<<z<<endl;
//    };
    double_complex w1=double_complex(0.,2.)/pi-2.*q*w;
    double_complex w2=-2.*(q*w1+w);
    double_complex wn;
    h = w;
    for(size_t n=3;n<=8;n++){
      wn = -2.*(q*w1+double(n-1)*w2);
      h += c[n]*wn;
      w2 = w1;
      w1 = wn;
    };
    return h.real();
  }else if ((z<=0.1)||(y>0.5 && y<20. && y>z4)||(y>=20.&&z<5.)){// region III
//    if (reg!=3){
//      reg=3;
//      mio<<"Region III "<<x<<' '<<y<<' '<<z<<' '<<z4<<'('<<2+size_t(37*exp(-0.6*y))<<')'<<endl;
//    };
    size_t n = 2+size_t(37*exp(-0.6*y));
    h = 0.;
    for(size_t i=n;i>0;i--){
      h = 0.5*i/(double_complex(z*i+y,-x)+h);
    };
    h = 1./(double_complex(y,-x)+h);
    return h.real()/pi;
  };
// region II
//    if (reg!=2){
//      reg=2;
//      mio<<"Region II "<<x<<' '<<y<<' '<<z<<endl;
//    };
  size_t n = 4 + size_t(pow(z,-1.05)*(1.+3*exp(-1.1*y)));
  double d=0.5/(z*z);
  double_complex t(d+y/z,-x/z);
  double_complex hn = 1./t;
  h = hn;
  for(size_t i=1;i<=n;i++){
    hn *= d/(t+double(i));
    h += hn;
  };
  return h.real()/(pi*z);
}

double galatryH(double x,double y,double z,double s){
  double pi=sqrt(2*acos(0.)),z4=4*pow(z,0.868);
  double_complex zz=double_complex(z,s),c[9];
  double_complex w=cprob(x,y),h,q=double_complex(x,y),q1=double_complex(y,-x);
  c[0]=1.;
  c[1]=c[2]=0.;
  c[3]= double_complex(0.,1.)*zz/12.; zz*=double_complex(z,s);
  c[4]=-zz/48.; zz*=double_complex(z,s);
  c[5]= double_complex(0.,-1.)*zz/240.; zz*=double_complex(z,s);
  c[6]= zz/1440.+c[3]*c[3]/2.; zz*=double_complex(z,s);
  c[7]= double_complex(0.,1.)*zz/10080.+c[3]*c[4]; zz*=double_complex(z,s);
  c[8]=-zz/80640.+c[3]*c[5]+c[4]*c[4]/2.;
  zz = double_complex(z,s);
  if ( (y<=0.5) && (z<=0.04) && fabs(x)<=2.){ // region I
    double_complex w1=double_complex(0.,2.)/pi-2.*q*w;
    double_complex w2=-2.*(q*w1+w);
    double_complex wn;
    h = w;
    for(size_t n=3;n<=8;n++){
      wn = -2.*(q*w1+double(n-1)*w2);
      h += c[n]*wn;
      w2 = w1;
      w1 = wn;
    };
    return h.real();
  }else if ((z<=0.1)||(y>0.5 && y<20. && y>z4)||(y>=20.&&z<5.)){// region III
    size_t n = 2+size_t(37*exp(-0.6*y));
    h = 0.;
    for(size_t i=n;i>0;i--){
      h = 0.5*i/(q1+zz*double(i)+h);
    };
    h = 1./(q1+h);
    return h.real()/pi;
  };
// region II
  size_t n = 4 + size_t(pow(z,-1.05)*(1.+3*exp(-1.1*y)));
  double_complex d=0.5/(zz*zz);
  double_complex t=d+q1/zz;
  double_complex hn = 1./t;
  h = hn;
  for(size_t i=1;i<=n;i++){
    hn *= d/(t+double(i));
    h += hn;
  };
  h/=zz;
  return h.real()/pi;
}
