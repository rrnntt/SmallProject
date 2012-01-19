#include <math.h>
#include "matrix.h"
using namespace std;

ostream& operator << (ostream& ostr,const matrix& m){
  for(size_t i=0;i<m.dim1();i++){
     for(size_t j=0;j<m.dim2();j++)
       ostr<<m(i,j)<<' ';
     ostr<<'\n';
  };
  ostr<<'\n';
  return ostr;
};

ostream& operator << (ostream& ostr,const vec& m){
  for(size_t j=0;j<m.size();j++) ostr<<m(j)<<' ';
  ostr<<'\n';
  return ostr;
}

//=============================================================================
void vec::mul(matrix& m,vec& b){
   if (m.dim2() != b.dim()) return;
   size_t n1 = m.dim1();
   size_t n2 = b.dim();
   double sum;
   v.resize(n1);
   for(size_t i=0;i<n1;i++){
     sum = 0;
     for(size_t j=0;j<n2;j++) sum += m(i,j)*b(j);
     val(i) = sum;
   };
}

void vec::mul(vec& b,matrix& m){
   if (m.dim1() != b.dim()) return;
   size_t n1 = m.dim2();
   size_t n2 = b.dim();
   double sum;
   v.resize(n1);
   for(size_t i=0;i<n1;i++){
     sum = 0;
     for(size_t j=0;j<n2;j++) sum += b(j)*m(j,i);
     val(i) = sum;
   };
}

//=============================================================================
jac_matrix::jac_matrix(size_t nn,size_t ii,size_t jj){
  make(nn,ii,jj);
}

void jac_matrix::make(size_t nn,size_t ii,size_t jj){
  if (ii<nn && jj < nn){
    matrix::make(2,2);
    n = nn;
    i = ii;
    j = jj;
    val(0,0) = val(1,1) = 1.;
    val(0,1) = val(0,1) = 0.;
  };
}

double jac_matrix::operator()(size_t ii,size_t jj)const{
  if (ii == jj){
    if (ii == i && ii == j) return val(0,0);
    else return 1.;
  }else if (ii == i && jj == j) return val(0,1);
  else if (ii == j && jj == i) return val(1,0);
  return 0.;
}

//=============================================================================
double matrix::bad_value;

void matrix::make(size_t x,size_t y){
  d1 = x;
  d2 = y;
  v.resize(d1*d2);
}

void matrix::identity(size_t x){
  make(x,x);
  zero();
  for(size_t i=0;i<x;i++) val(i,i) = 1.;
}

void matrix::read(string fn){
  ifstream fil(fn.c_str());
  size_t n,m;
  fil>>n>>m;
  make(n,m);
  for(size_t i=0;i<n;i++)
  for(size_t j=0;j<m;j++) fil>>val(i,j);
}



/*
    Replaces the matrix by its LU decomposition of a rowwise
    permutation of itself. indx is an output vector that records
    the row permutation effected by the partial pivoting;
    d is output as +/-1 depending on whether the number of rows
    interchanges was even or odd respectively.
*/
bool matrix::ludcmp(valarray<size_t>& indx,double& d){
  if (dim1() != dim2()) return false;
  if (indx.size() < dim1()) {
    indx.resize(dim1());
  };
  size_t i,imax,j,k,n;
  double big,dum,sum,temp;
  const double tiny = 1e-20;
  valarray<double> vv(dim1()); // vv stores the implicit scaling of each row
  d = 1.;
  n = dim1();

  // Loop over rows to get the implicit scaling information
  for(i=0;i<n;i++){
     big = 0.;
     for(j=0;j<n;j++)
        if ((temp=fabs(val(i,j))) > big) big = temp;
     if (big == 0.) return false; // Singular matrix
     vv[i] = 1./big;
  };

  for(j=0;j<n;j++){
     for(i=0;i<j;i++){
        sum = val(i,j);
        for(k=0;k<i;k++) sum -= val(i,k)*val(k,j);
        val(i,j) = sum;
     };
     big = 0.;
     for(i=j;i<n;i++){
        sum = val(i,j);
        for(k=0;k<j;k++) sum -= val(i,k)*val(k,j);
        val(i,j) = sum;
        if ((dum = vv[i]*fabs(sum)) >= big){
           big = dum;
           imax = i;
        };
     };

     if (j != imax){
       for(k=0;k<n;k++){
          dum = val(imax,k);
          val(imax,k) = val(j,k);
          val(j,k) = dum;
       };
       d *= -1.;
       dum = vv[imax];
       vv[imax] = vv[j]; //??????? interchange?
       vv[j] = dum;
     };
     indx[j] = imax;
     if (val(j,j) == 0) val(j,j) = tiny;
     if (j < n-1){
        dum = 1./val(j,j);
        for(i=j+1;i<n;i++) val(i,j) *= dum;
     };
  };
  return true;
}

/*
   Solves set of n linear equations A.x = b . This matrix must be
   LU decomposition of A after using method ludcmp(...). indx is
   input as the permutation vector returned by ludcmp. b is input as
   righ-hand side vector b, and returns with the solution vector x.
   Matrix and indx are not modified by this method. This routine takes
   into account the possibility that b will begin with many zero
   elements, so it is efficient for use in matrix inversion.
*/
bool matrix::lubksb(valarray<size_t>& indx,vec& b){
  size_t i0,i,ii,ip,j,n=b.size();
  double sum;
  bool ok = false;
  if (dim1() != n || dim2() != n || indx.size() != n) return false;

  for(i=0;i<n;i++){
    ip = indx[i];
    sum = b[ip];
    b[ip] = b[i];

    if (ok)  {
       for(j=ii;j<i;j++) sum -= val(i,j)*b[j];
    } else if (sum)
       {ii = i; ok = true;};
    b[i] = sum;
  };

  for(i0=n;i0>=1;i0--){
    i = i0-1;
    sum = b[i];
    for(j=i+1;j<n;j++) sum -= val(i,j)*b[j];
    b[i] = sum/val(i,i);
  };

  return true;
}


bool matrix::solve(vec& x){
   valarray<size_t> indx(dim1());
   double d;
   if (!ludcmp(indx,d)) return false;
   return lubksb(indx,x);
}

void matrix::mul(matrix& m1,matrix& m2){
   size_t n = m1.dim1();
   size_t nm = m1.dim2();
   size_t m = m2.dim2();
   if (nm != m2.dim1()) return;
   double dum;
   make(n,m);

   for(size_t i=0;i<n;i++){
     for(size_t j=0;j<m;j++){
        dum = 0.;
        for(size_t k=0;k<nm;k++) dum += m1(i,k)*m2(k,j);
        val(i,j) = dum;
     };
   };
}

//  m = m * m2
void matrix::mul(matrix& m2){
   size_t n = dim1();
   size_t nm = dim2();
   size_t m = m2.dim2();
   if (nm != m2.dim1()) return;
   double dum;
   matrix a;
   a.copy(*this);
   make(n,m);

   for(size_t i=0;i<n;i++){
     for(size_t j=0;j<m;j++){
        dum = 0.;
        for(size_t k=0;k<nm;k++) dum += a(i,k)*m2(k,j);
        val(i,j) = dum;
     };
   };
}

void matrix::inverse(){
  if ( dim1() != dim2() ) return;
  size_t i,j,n=dim1();
  double d;
  matrix y(n,n);
  valarray<size_t> indx(n);
  vec col(n);

  ludcmp(indx,d);
  for(j=0;j<n;j++){
     col = 0.;
     col[j] = 1.;
     lubksb(indx,col);
     for(i=0;i<n;i++) y(i,j) = col[i];
  };
  v = y.v;
}

double pythag(double a,double b){
  double aa;
  double bb,tmp;
  aa = fabs(a);
  bb = fabs(b);
  if (aa > bb) {
     tmp = bb / aa;
     return aa*sqrt(1.+tmp*tmp);
  }else if (bb == 0.) return 0.;
  tmp = aa / bb;
  return bb*sqrt(1.+tmp*tmp);
}

#define SIGN(a,b) (((b)<0.)?-(a):(a));

void matrix::svd(vec& w,matrix& v){
}


void matrix::jacob2(size_t i,size_t j,matrix *V1,matrix *V2){
  if (V1 != 0 && V2 != 0){
    V1->make(2,2);
    V2->make(2,2);
  };
  if (i>=dim1() || j>=dim2()) return;
  double t2,t,c1,s1,c2,s2;
  double &a=val(i,i);
  double &b=val(i,j);
  double &c=val(j,i);
  double &d=val(j,j);
  t2 = (b*b - c*c + a*a - d*d)/2/(a*c + b*d);
  if (fabs(t2) < 1e60){
    t = fabs(t2)-sqrt(t2*t2+1.);
    if (t2 > 0) t = -t;
  }else t = t2/2;

  c1 = 1./sqrt(1.+t*t);
  s1 = t*c1;
  double A,B,C,D;
  A = b*c1 + d*s1;
  B = a*c1 + c*s1;
  C = c*c1 - a*s1;
  if (B!=0.){
    t = A/B;
    D = C/t;
    c2 = 1./sqrt(1.+t*t);
    s2 = t*c2;
  }else if (A!=0.){
    t = B/A;
    D = C*t;
    s2 = 1./sqrt(1.+t*t);
    c2 = t*s2;
  }else{
    t = a/b;
    D = 0.;
    c2 = 1./sqrt(1.+t*t);
    s2 = -t*c2;
  };

  if (V1 != 0 && V2 != 0){
    V1->val(0,0) = V1->val(1,1) = c1;
    V1->val(0,1) = s1;
    V1->val(1,0) =-s1;
    V2->val(0,0) = V2->val(1,1) = c2;
    V2->val(0,1) =-s2;
    V2->val(1,0) = s2;
  };
  a = B*c2 + A*s2;
  d = D*c2 - C*s2;
  b = c = 0.;
  for(size_t k=0;k<dim1();k++){
    if(k != i && k != j) {
      A = val(k,i);
      B = val(k,j);
      val(k,i) = A*c2 + B*s2;
      val(k,j) =-A*s2 + B*c2;
      A = val(i,k);
      B = val(j,k);
      val(i,k) = A*c1 + B*s1;
      val(j,k) =-A*s1 + B*c1;
    };
  };
}

void matrix::hous_col(size_t k,size_t r,matrix &P){
  if(k >= dim1() || r >= dim2()) return;
  valarray<double> u,p;
  if (P.dim1() != dim1() || P.dim2() != dim1()){
    P.identity(dim1());
  };
  u.resize(dim1());
  double s = 0.;
  for(size_t i=0;i<u.size();i++)
    if (i<r) u[i] = 0.;
    else {
      u[i] = val(i,k);
      s += u[i]*u[i];
    };
  u[r] += sqrt(s);
  s += - val(r,k)*val(r,k) + u[r]*u[r];
  u /= sqrt(s/2);

  p.resize(u.size());
  p = 0.;

  for(size_t i=0;i<p.size();i++)
  for(size_t j=r;j<u.size();j++) p[i] += P(j,i)*u[j];

  for(size_t i=r;i<u.size();i++)
  for(size_t j=0;j<u.size();j++) P(i,j) -= u[i]*p[j];

}

void matrix::hous_row(size_t k,size_t r,matrix &P){
  if(r >= dim1() || k >= dim2()) return;
  valarray<double> u,p;
  if (P.dim1() != dim2() || P.dim2() != dim2()){
    P.identity(dim2());
  };
  u.resize(dim2());
  double s = 0.;
  for(size_t i=0;i<u.size();i++)
    if (i<r) u[i] = 0.;
    else {
      u[i] = val(k,i);
      s += u[i]*u[i];
    };
  u[r] += sqrt(s);
  s += - val(k,r)*val(k,r) + u[r]*u[r];
  u /= sqrt(s/2);

  p.resize(u.size());
  p = 0.;

  for(size_t i=0;i<p.size();i++)
  for(size_t j=r;j<u.size();j++) p[i] += P(i,j)*u[j];

  for(size_t i=0;i<u.size();i++)
  for(size_t j=r;j<u.size();j++) P(i,j) -= p[i]*u[j];

}

void matrix::qr_tall(matrix &R){
  valarray<double> u,p;
  size_t k,r;
  double s;
  R.make(dim2(),dim2());
  R.zero();
  u.resize(dim1());
  p.resize(dim2());

  for(k=0;k<dim2();k++){
    r = k;
    s = 0.;
    for(size_t i=0;i<u.size();i++)
      if (i<r) u[i] = 0.;
      else {
        u[i] = val(i,k);
        s += u[i]*u[i];
      };
    u[r] += sqrt(s);
    s += - val(r,k)*val(r,k) + u[r]*u[r];
//    mio<<"s("<<k<<")="<<s<<'\n';
    u /= sqrt(s/2);

    p = 0.;

    for(size_t i=r;i<p.size();i++)
    for(size_t j=r;j<u.size();j++) p[i] += val(j,i)*u[j];

    for(size_t i=r;i<u.size();i++)
    for(size_t j=r;j<u.size();j++) val(i,j) -= u[i]*p[j];

    // store only independent components; r-th comp. can be restored
    // from the comdition |u| = sqrt(2)
    for(size_t i=r+1;i<u.size();i++) val(i,k) = u[i];
  };

}

// reduction of a square symmetric matrix to tridiagonal form
void matrix::d3(matrix &U){
  valarray<double> u,p,q;
  size_t k,r;
  double s;
  U.identity(dim1());
  u.resize(dim1());
  p.resize(dim2());
  q.resize(dim2());

  for(k=0;k<dim2();k++){
    r = k+1;
    if (r >= dim1()-1) break;
    s = 0.;
    for(size_t i=0;i<u.size();i++)
      if (i<r) u[i] = 0.;
      else {
        u[i] = val(i,k);
        s += u[i]*u[i];
      };
    u[r] -= SIGN(sqrt(s),u[r]);
    s += - val(r,k)*val(r,k) + u[r]*u[r];
//    mio<<"s("<<k<<")="<<s<<'\n';
    if (s == 0.) continue;
    u /= sqrt(s/2);
//    mio<<"u="; for(size_t i=0;i<u.size();i++) mio<<u[i]<<' ';mio<<'\n';

    p = 0.;
    q = 0.;
    s = 0.;
    for(size_t i=0;i<p.size();i++){
      for(size_t j=k;j<u.size();j++){
        if (i>=k) p[i] += val(j,i)*u[j];
        q[i] += U(j,i)*u[j];
      };
      s += u[i]*p[i];
    };
//    mio<<"p="; for(size_t i=0;i<p.size();i++) mio<<p[i]<<' ';mio<<'\n';
    s /= 2;
    p -= u*s;

    for(size_t i=k;i<u.size();i++)
    for(size_t j=0;j<u.size();j++) {
      if (j>=k) val(i,j) -= u[i]*p[j] + p[i]*u[j];
      U(i,j) -= u[i]*q[j];
    };

//    for(size_t i=k;i<u.size();i++)
//    for(size_t j=0;j<u.size();j++) {
//      U(i,j) -= u[i]*q[j];
//    };

    // store only independent components; r-th comp. can be restored
    // from the condition |u| = sqrt(2)
    for(size_t i=r+1;i<u.size();i++) {
      val(i,k) = 0.;//u[i];
      val(k,i) = 0.;
    };
  };

}

void matrix::transpose(matrix &a){
  if (a.dim1() != a.dim2()) return;
  make(a.dim1(),a.dim2());
  for(size_t i=0;i<dim1();i++)
  for(size_t j=i;j<dim1();j++){
    if (i == j) val(i,j) = a(i,j);
    else{
      val(i,j) = a(j,i);
      val(j,i) = a(i,j);
    };
  };
}


double det(vec &d,valarray<double> &e){
  size_t n = d.size();
  if (n != e.size()) return 0.;
  if (n == 1) return d[0];
  double d1,d2,dd;
  d2 = 1.;
  d1 = d[n-1];
  for(size_t i=n-2;i<n;i--){
    dd = d[i]*d1 - e[i]*e[i]*d2;
    d2 = d1;
    d1 = dd;
  };
  return dd;
}

void matrix::diag(vec &d){
  size_t k,r,n=dim1();
  double c,s,ss;
  d.resize(n);
  valarray<double> u(n),p(n),q(n),e(n);

  for(k=0;k<n;k++){
    r = k+1;
    if (r >= n-1) break;
    s = 0.;
    for(size_t i=0;i<u.size();i++)
      if (i<r) u[i] = 0.;
      else {
        u[i] = val(i,k);
        s += u[i]*u[i];
      };
    u[r] += SIGN(sqrt(s),u[r]);
    s += - val(r,k)*val(r,k) + u[r]*u[r];
//    mio<<"s("<<k<<")="<<s<<'\n';
    if (s != 0.){
       u /= sqrt(s/2);
//       mio<<"u="; for(size_t i=0;i<u.size();i++) mio<<u[i]<<' ';mio<<'\n';

       p = 0.;
       q = 0.;
       s = 0.;
       for(size_t i=k;i<p.size();i++){
         for(size_t j=k;j<u.size();j++){
           p[i] += val(j,i)*u[j];
         };
         s += u[i]*p[i];
       };
   //    mio<<"p="; for(size_t i=0;i<p.size();i++) mio<<p[i]<<' ';mio<<'\n';
       s /= 2;
       p -= u*s;

       for(size_t i=k;i<u.size();i++)
       for(size_t j=k;j<u.size();j++) {
         val(i,j) -= u[i]*p[j] + p[i]*u[j];
       };

    };
    d[k] = val(k,k);
    e[k] = val(r,k);
    for(size_t i=r;i<n;i++) {
      val(i,k) = u[i];
      val(k,i) = 0.;
    };
  };
  e[n-2] = val(n-2,n-1);
  d[n-2] = val(n-2,n-2);
  d[n-1] = val(n-1,n-1);

//  mio<<"n="<<n<<'\n';
//  mio<<"d="; for(size_t i=0;i<d.size();i++) mio<<d[i]<<' ';mio<<'\n';
//  mio<<"e="; for(size_t i=0;i<e.size();i++) mio<<e[i]<<' ';mio<<'\n';

  val(n-1,n-1) =1.; val(n-2,n-2) = 1.;
  val(n-1,n-2) = val(n-2,n-1) = 0.;
  for(size_t k=n-2;k<n;k--){
    r = k + 1;
//    mio<<"k="<<k<<'\n';

    u = 0.;
    for(size_t i=r;i<u.size();i++) {
      u[i] = val(i,k);
      val(i,k) = 0.;
    };
//    mio<<"u="; for(size_t i=0;i<u.size();i++) mio<<u[i]<<' ';mio<<'\n';
    val(k,k) = 1.;

    q = 0.;
    for(size_t i=r;i<n;i++){
      for(size_t j=r;j<n;j++) q[i] += val(i,j)*u[j];
    };
    for(size_t i=r;i<n;i++)
    for(size_t j=r;j<n;j++) {
      val(i,j) -= q[i]*u[j];
    };

  };
//  --- diagonalizing ------------

//  mio<<"d="; for(size_t i=0;i<d.size();i++) mio<<d[i]<<' ';mio<<'\n';
//  mio<<"e="; for(size_t i=0;i<e.size();i++) mio<<e[i]<<' ';mio<<'\n'<<'\n';
//  mio<<"Det="<<det(d,e)<<'\n';
  size_t it;
  double c2,s2,cs,ces,d1,d2,e1,ee,e0;
  for(size_t k=0;k<n-1;k++){
    r = k + 1;
    for(it=0;it<300;it++){
      d1 = d[k];
      d2 = d[r];
      e1 = e[k];
      ee = fabs(d1)+fabs(d2);
      if (ee == 0.){
        if (e1 == 0.) break;
        else{
          c = 0.;
          s = 1.;
        };
      }else{
        ss = fabs(e1)/ee;
        if (ss < 1e-12) break;

        ss = e1/(d2-d1);

        c = 1./sqrt(1.+ss*ss);
        s = ss*c;
      };
      ee = 0.;
      for(size_t m=k;m<n-1;m++){
        c2 = c*c;
        s2 = s*s;
        cs = c*s;
        ces = cs*e1*2;
        d[m] = c2*d1 - ces + d2*s2;
        d[m+1] = c2*d2 + ces + d1*s2;
        e[m] = e1*(c2 - s2) + cs*(d1 - d2);
        if (m > k){
          e[m-1] *= c;
          e[m-1] -= ee*s;
        };
        ee = e[m+1];
        e[m+1] *= c;
        ee *= -s;

//        mio<<"k="<<k<<" ("<<m<<','<<m+1<<')'<<it<<'\n';
//        mio<<"c="<<c<<" s="<<s<<' '<<cs<<'\n';
//        if (k==0) s = -s;
        for(size_t l=0;l<n;l++){
          d1 = val(m,l);
          d2 = val(m+1,l);
          val(m,l)   = c*d1 - s*d2;
          val(m+1,l) = c*d2 + s*d1;
        };
//           mio<<"d="; for(size_t i=0;i<d.size();i++) mio<<d[i]<<' ';mio<<'\n';
//           mio<<"e="; for(size_t i=0;i<e.size()-1;i++) mio<<e[i]<<' ';mio<<ee<<'\n'<<'\n';
        if (m == n-2) break;
        d1 = d[m+1];
        d2 = d[m+2];
        e1 = e[m+1];
        e0 = e[m];
        if ((fabs(e0)+fabs(ee))/(fabs(d1)+fabs(d2)) < 1e-12){
          c = 1.;
          s = 0.;
        }else if (fabs(e0) != 0.){
          ss = ee/e0;
          c = 1./sqrt(1.+ss*ss);
          s = -ss*c;
        }else{
          c = 0.;
          s = 1.;
        };
      };
    };
  };

}

//---------- matr_object ---------------------------
double matr_object::operator()(size_t i,size_t j)const{
  return m->val(i,j);
}


matr_show::matr_show(matrix &mm,double ma,double mi):matr_object(mm){
  matrix &a = *m;
  double d;
  if (ma == 0.) {
     vmax = 0.;
     for(size_t i=0;i<a.size();i++){
       d = fabs(a[i]);
       if (vmax < d) vmax = d;
     };
  }else
    vmax = fabs(ma);
  if (mi == 0.) {
     vmin = vmax;
     for(size_t i=0;i<a.size();i++){
       d = fabs(a[i]);
       if (vmin > d) vmin = d;
     };
  }
  else
    vmin = fabs(mi);
};

ostream& operator << (ostream& ostr,const matr_show& m){
  string s="0123456789";
  int n,nn=s.size()-1;
  char c;
  double dv = m.vmax - m.vmin,dn;
  ostr<<m.vmax<<' '<<m.vmin<<'\n';
  for(size_t i=0;i<m.m->dim1();i++){
     for(size_t j=0;j<m.m->dim2();j++){
       dn = (fabs(m(i,j)) - m.vmin)/dv;
       if (dn < 0) c = '.';
       else if (dn >= 1.) c = '*';
       else c = s[int(dn*nn)];
       ostr<<c;
     };
     ostr<<'\n';
  };
  ostr<<'\n';
  return ostr;
};

