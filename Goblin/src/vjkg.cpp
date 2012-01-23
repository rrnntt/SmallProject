#include "Goblin/vjkg.h"

#include <stdio.h>
#include <sstream>
#include <iostream>

namespace Goblin
{

using namespace std;

int VJKG::d_Iso(11);
vector<molecule> VJKG::Isotope;

bool operator==(const VJKG& q1,const VJKG& q2){
   if (q1.isoi() != q2.isoi()) return false;
   if (q1.nv() != q2.nv()) return false;
   bool ok = true;
   for(size_t i=0;i<q1.nv();i++) ok = ok && q1.v(i) == q2.v(i);
   return ok && q1.j == q2.j && q1.k == q2.k && q1.g == q2.g;
}

bool operator!=(const VJKG& q1,const VJKG& q2){
  return !(q1==q2);
}

ostream& operator << (ostream& ostr,const vibq& v){
  ostr<<'(';
  bool bigV = false;
  if (v.assigned()){
    for(size_t i=0;i<v.n();i++) if (v(i) > 9) bigV = true;
    for(size_t i=0;i<v.n();i++) {
      ostr<<(int)v(i);
      if (bigV) ostr << ' ';
    };
  }else{
    for(size_t i=0;i<v.n();i++) ostr<<'v';
  };
  ostr<<")";
  return ostr;
}

istream& operator >> (istream& istr,vibq& v)
{
  return istr;
}

bool operator<(const vibq& v1,const vibq& v2){
  if (v1.n() < v2.n()) return true;
  if (v1.n() > v2.n()) return false;
  for(size_t i=0;i<v1.n();i++) if (v1(i) != v2(i)) return v1(i) < v2(i);
  return false;
}

bool operator==(const vibq& v1,const vibq& v2){
  unsigned char c1,c2;
  if (v1.n() != v2.n()) return false;
  for(size_t i=0;i<v1.n();i++) {
    c1 = v1(i);
    c2 = v2(i);
    if (c1 == 255 || c2 == 255) continue;
    if (c1 != c2) return false;
  };
  return true;
}
bool operator!=(const vibq& v1,const vibq& v2){
  return !(v1==v2);
}


ostream& operator << (ostream& ostr,const VJKG& q){
  ostr<<'['<<q.Iso()<<"] ";
  if (q.assigned()){
    ostr<<'(';
    if (q.bigV())
      for(size_t i=0;i<q.nv();i++) ostr<<int(q.v(i))<<' ';
    else{
      for(size_t i=0;i<q.nv();i++) ostr<<int(q.v(i));
    };
    ostr<<")  ";
    ostr<<q.j<<' '<<q.k<<' '<<q.g;
  }else{
    ostr<<'(';
    for(size_t i=0;i<q.nv();i++) ostr<<'v';
    ostr<<") j k g";
  };
  return ostr;
}

std::istream& operator >> (std::istream& istr,Goblin::VJKG& q)
{
  return istr;
}

bool operator<(const VJKG& q1,const VJKG& q2){
  if (q1.iso() != q2.iso()) return  q1.iso() < q2.iso();
  else if (q1.v != q2.v) return  q1.v < q2.v;
  else if (q1.j != q2.j) return  q1.j < q2.j;
  else if (q1.k != q2.k) return  q1.k < q2.k;
  return q1.g > q2.g;
}

unsigned char vibq::bad_v=0;

vibq& vibq::operator=(const string& str0){
  string str;
  size_t i0,i1;
  i0 = str0.find_first_not_of(" (");
  i1 = str0.find_last_not_of(" )");
  if (i0 > i1 || i0==string::npos || i1==string::npos) return *this;
  str = str0.substr(i0,i1-i0+1);

  if (n()==0) {
    i0 = str.find(' ');
    if (i0==string::npos) n(str.size());
  };

  if (str.empty()) {
   deassign();
   return *this;
  };


  if (str.size() == n()){   // if every v_i < 10
    char c2[] = "0";
    for(size_t i=0;i<n();i++) {
      c2[0] = str[i];
      v_[i] = isdigit(c2[0])?atoi(c2):255;
    };
  }else{                     // otherwise every v_i must be separated by space characeter(s)
    istringstream istr(str);
    size_t i = 0,iv;
    if (n()){
      while(istr>>iv){
        v_[i++] = (unsigned char)(iv);
        if ( i == n() ) break;
      };
    }else{
      vector<unsigned char> vv;
      while(istr>>iv){
        vv.push_back((unsigned char)(iv));
      };
      n(vv.size());
      for(size_t i=0;i<n();i++) v_[i] = vv[i];
    };
  };
  return *this;
}

string vibq::str(){
  ostringstream ostr;
  ostr<<(*this);
  return ostr.str();
}

void vibq::deassign(){
  for(size_t i=0;i<n();i++)
    v_[i] = 255;
}

bool vibq::assigned()const{
  for(size_t i=0;i<n();i++)
    if (v_[i] != 255) return true;
  return false;
}

VJKG VJKG::bad_q;

VJKG::VJKG():j(),k(),g(){
  setIso(d_Iso);
//  v.resize(d_nv);
  setV("");
}
VJKG::~VJKG(){
}

int VJKG::newIso(int ii){  //  ii -- "global" isotope number; returns "local" isotope number
  for(size_t k=0;k<Isotope.size();k++)
     if (ii == Isotope[k].Iso()) {
       return k;
     };
  molecule mol(ii);
  Isotope.push_back(mol);
  std::cout <<"Iso "<<ii<<" mass="<<mol.mass()<<" nv="<<mol.nv()<<" Iso="<<mol.Iso()<<'\n';
  return Isotope.size() - 1;
}

void VJKG::setIso(int ii){
  iiso = newIso(ii);
  set_nv(Isotope[iiso].nv());
}

void VJKG::isoi(int i){
  if (i >= Isotope.size()) setIso(d_Iso);
  else
    setIso(Isotope[i].Iso());
}

//     str = "[Iso] (vvvv) j k g"
//  or str = "{iso} (vvvv) j k g"
void VJKG::assign(const string& str){
 string si;
 string::size_type i1,i2=0;

 i1 = str.find('[');
 if (i1 != string::npos) { // reading Iso inside of []
   i2 = str.find(']');
   if (i2 < i1) return;
   si = str.substr(i1+1,i2-i1);
   setIso(atoi(si.c_str()));
 }else{
   i1 = str.find('{');
   if (i1 != string::npos) { // reading Iso inside of {}
     i2 = str.find('}');
     if (i2 < i1) return;
     si = str.substr(i1+1,i2-i1);
     setIso(atoi(si.c_str()));
   }else setIso(d_Iso);
 };

 i1 = str.find('(');
 if (i1 != string::npos) { // reading vib inside of ()
   i2 = str.find(')');
   if (i2 < i1) return;
   si = str.substr(i1+1,i2-i1-1);
   setV(si.c_str());
 };

 istringstream isi(str.substr(i2+1));
 j = k = g = 255;
 isi>>j>>k>>g;
}

void VJKG::deassign(){
  v.deassign();
  j = k = g = 255;
}

void VJKG::setV(const string str){
  v = str;
/*  if (str.empty()) {
   for(size_t i=0;i<nv();i++) v[i] = 0;
   return;
  };
  if (str.size() == nv()){   // if every v_i < 10
    char c2[] = "0";
    for(size_t i=0;i<nv();i++) {
      c2[0] = str[i];
      v[i] = atoi(c2);
    };
  }else{                     // otherwise every v_i must be separated by space characeter(s)
    string str1=str;
    for(size_t j=0;j<str.size();j++)
      if (str1[j]=='_')  str1[j] = ' ';
    istringstream istr(str1);
    size_t i = 0,iv;
    while(istr>>iv){
      v[i++] = (unsigned char)(iv);
      if ( i == nv() ) break;
    };
  };*/
}
/** No descriptions */
void VJKG::setV(size_t i,unsigned char vv){
  if (i >= nv()) return;
  v[i] = vv;
}

string VJKG::str(){
  ostringstream ostr;
  ostr<<(*this);
  return ostr.str();
}

//---------------------------------------------------------------------------------
molecule::molecule(int iso){
  iso_ = iso;
  mass_ = mass(iso);
  switch(iso){
    case 11: nv(3);
             break;
    case 12: nv(3);
             break;
    case 13: nv(3);
             break;
    case 14: nv(3);
             break;
    case 15: nv(3);
             break;
    case 16: nv(3);
             break;
    case 71: nv(1);
             break;
    default:
             nv(1);
             break;
  };
};

double molecule::mass(int iso){
  switch(iso){
    case 11: return 2*1.0072765 + 15.990526;
    case 12: return 2*1.0072765 + 18.;
    case 13: return 2*1.0072765 + 17.;
    case 14: return 2.+ 1.0072765 + 15.990526;
    case 15: return 2.+ 1.0072765 + 18.;
    case 16: return 2.+ 1.0072765 + 17.;
    case 71: return 2*15.990526;
    default:
             return 1.;
  };
};

size_t molecule::symm(VJKG& q){
  size_t n;
  switch(q.iso()){
    case 11:
    case 12:
    case 13: if (q.v.n() != 3) return 0;
      n = q.v(2) + q.k + q.g;
      if (n / 2 * 2 == n){
        if (q.g / 2 * 2 == q.g) return 0;
        else return 1;
      }else{
        if (q.g / 2 * 2 == q.g) return 2;
        else return 3;
      };
      break;
    default:
             return 0;
  };
}

size_t molecule::nsymm(VJKG& q){
  switch(q.iso()){
    case 11:
    case 12:
    case 13: return 4;
    default:
             return 1;
  };
}


size_t molecule::tsymm(VJKG& q){
  size_t qsymm = symm(q);
  switch(q.iso()){
    case 11:
    case 12:
    case 13:
      switch(qsymm){
        case 0: return 1;
        case 1: return 0;
        case 2: return 3;
        case 3: return 2;
      };
      return 0;
    default: return 0;
  };
}

//------------------ VJKGColumn ----------------------//
  DECLARE_COLUMN(VJKGColumn,vjkg);

  void VJKGColumn::read(std::istream& s, int index)
  {
    std::string str;// = m_data[index];
    s >> str;
    if (str.empty()) return;
    if (str[0] == '"')
    {
      if (str == "\"\"") 
      {
        str.clear();
        return;
      }
      str.erase(0,1);
      char c;
      while(s.good())
      {
        s.get(c);
        if (c == '"' && (str.empty() || str.back() != '\\')) return;
        str.push_back(c);
      }
    }
    m_data[index].assign(str);
  }

/// Save into a file
void VJKGColumn::saveAsci(std::ostream& s, int index) const
{
  s << '"';
  this->print(s,index);
  s << '"';
}

/// Read from a file
void VJKGColumn::loadAsci(std::istream& s, int index)
{
  this->read(s,index);
}

void VJKGColumn::fromString(const std::string& str,size_t i)
{
  m_data[i].assign(str);
}


} // Goblin
