#include <sstream>
#include "Goblin/utils.h"

namespace Goblin
{

using namespace std;

string brakets(string str,const string bk){
  string s = str;
  size_t jb,je,nb;
  jb = s.find(bk[0]); // '['
  if (jb != string::npos){
    nb = 0;// number of open brakets
    for(size_t j=jb+1;j<s.size();j++){
      if (s[j] == bk[0]) nb++;  //  '['
      else {
        if (s[j] == bk[1])   // ']'
          if (nb == 0){
            jb++;
            je = j;
            break;
          }else nb--;
      };
    };
    if (nb || je < jb || je >= s.size() || jb >= s.size()) return "$ERROR$";
    if (je == jb) return " ";
    return s.substr(jb,je-jb);
  };
  return "";
}

bool complex_command(string ccom,string& who,size_t& n,string& com){
  size_t i;
  string ss = ccom;
  if ( (i = ss.find('.')) == string::npos){
    who.clear();
    n = string::npos;
    com = ccom;
    return false;
  };
  com = ss.substr(i+1);
  ss.erase(i);
  if ( (i = ss.find('#')) != string::npos){
    ss[i] = ' ';
    istringstream iss(ss);
    iss>>who>>n>>com;
  }else{
    istringstream iss(ss);
    iss>>who>>com;
    n = string::npos;
  };
  return true;
}

void file_name::name(string s){
  file_ = s;
  name_ = file_;
  size_t i = name_.rfind('/'); // operating system dependent
  if (i != string::npos) name_.erase(0,i+1);
}

string file_name::dir(){
  size_t i = file_.find_last_of('/'); // operating system dependent
  if (i < file_.size()) return file_.substr(0,i+1);
  return file_;
}

string strDouble(double d,size_t prec,size_t wdt){
  ostringstream ostr;
  if (prec) ostr.precision(prec);
  if (wdt) ostr.width(wdt);
  ostr.setf(ios::fixed);
  ostr<<d;
  return ostr.str();
}

string strDoubleE(double d,size_t prec,size_t wdt){
  ostringstream ostr;
  if (prec) ostr.precision(prec);
  if (wdt) ostr.width(wdt);
  ostr.setf(ios::scientific);
  ostr<<d;
  return ostr.str();
}

string strInt(int d,size_t wdt){
  ostringstream ostr;
  if (wdt) ostr.width(wdt);
  ostr<<d;
  return ostr.str();
}

} // Goblin
