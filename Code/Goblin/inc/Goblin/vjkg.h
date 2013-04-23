#ifndef GOBLIN_VJKG_H
#define GOBLIN_VJKG_H

#include "Goblin/DllExport.h"
#include "API/TableColumn.h"
#include "API/CompoundColumn.h"

#include <string>
#include <iostream>
#include <ostream>
#include <sstream>
#include <valarray>
#include <vector>

namespace Goblin
{

using namespace std;

const int MAXV = 10;
class VJKG;

class GOBLIN_EXPORT molecule{
  int iso_;
  size_t nv_;
  void nv(size_t n){nv_ = n;}
  double mass_;  //   molecule molar mass
public:
  static double mass(int iso);
  molecule(int iso);
  int Iso(){return iso_;}
  size_t nv(){return nv_;}
  double mass(){return mass_;}
  size_t symm(VJKG& q);  //  symmetry of state q
  size_t tsymm(VJKG& q); // transition symmetry 
  size_t nsymm(VJKG& q);//  number of symmetries
};

class GOBLIN_EXPORT vibq{
public:
  static unsigned char bad_v;
	valarray<unsigned char> v_;
  size_t n()const{return v_.size();}
  void n(size_t nn){v_.resize(nn);v_=0;}
  unsigned char operator()(size_t i)const{return i<n()?v_[i]:0;}
  unsigned char& operator[](size_t i){return i<n()?v_[i]:bad_v;}
  vibq& operator=(const vibq& v){v_.resize(v.n());v_=v.v_;return *this;}
  vibq& operator=(const string& s);
  string str();
  bool assigned()const;
  void deassign();
};

ostream& operator << (ostream& ostr,const vibq& v);
istream& operator >> (istream& ostr,vibq& v);
//ostringstream& operator << (ostringstream& ostr,const VJKG& q);
bool operator==(const VJKG& q1,const VJKG& q2);
bool operator!=(const VJKG& q1,const VJKG& q2);
bool operator<(const vibq& v1,const vibq& v2);
bool operator==(const vibq& v1,const vibq& v2);
bool operator!=(const vibq& v1,const vibq& v2);

class GOBLIN_EXPORT VJKG {
	void set_nv(size_t nn){if (v.n() != nn) v.n(nn);}
public:
	VJKG();
	~VJKG();
	static VJKG bad_q;
	static vector<molecule> Isotope;
	static int d_Iso; // default iso
	static void default_Iso(int diso){d_Iso = diso;}
  static int newIso(int i); //   i - a "global" isotope number
	void assign(const string& str);
	void deassign();
	bool bigV() const{for(size_t i=0;i<nv();i++) if (v(i)>9) return true; return false;}
  void setV(const string str);
  void setV(size_t i,unsigned char vv);
  void setV(size_t i,int vv){setV(i,(unsigned char)(vv));}
  int isoi()const {return iiso;}
  int iso()const {return Isotope[iiso].Iso();}
  int Iso()const {return iso();}
  void setIso(int i); //   i - a "global" isotope number
  void iso(int i){setIso(i);} //   i - a "global" isotope number
  void isoi(int i); //   i - a "local" isotope number
  double mass(){return Isotope[iiso].mass();}
  size_t symm(){return Isotope[iiso].symm(*this);}
  size_t nsymm(){return Isotope[iiso].nsymm(*this);}
  size_t tsymm(){return Isotope[iiso].tsymm(*this);}
  bool assigned()const{return v.assigned() || j!=255 || k!=255 || g!=255;}
	size_t nv()const{return v.n();} // number of vibrations <= MAXV
	bool vib_equal(VJKG& q){return q.v == v;}
	bool rot_equal(VJKG& q){return q.j == j && q.k == k && q.g == g;}
	string str();
	//---------------------------------
	int iiso;         //  contains "local" isotope number (index in Isotope vector)
	vibq v;
	int j,k,g;
};

bool operator<(const VJKG& q1,const VJKG& q2);
std::ostream& operator << (std::ostream& ostr,const Goblin::VJKG& q);
std::istream& operator >> (std::istream& istr,Goblin::VJKG& q);

class VJKGColumn: public API::TableColumn<VJKG>, public API::CompoundColumn
{
public:
  VJKGColumn():API::TableColumn<VJKG>(){}
  /// Return value of a cell as a string
  virtual std::string asString(size_t i) const;
  /// Read value from a string
  virtual void fromString(const std::string& str,size_t i);
  virtual void componentFromString(size_t row, const std::string& , const std::string&);

};

} // Goblin

#endif // GOBLIN_VJKG_H


