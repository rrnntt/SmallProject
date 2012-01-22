#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Goblin
{

using namespace std;

string  brakets(string str,const string bk="[]");
string  strDouble(double d,size_t prec=0,size_t wdt=0);
string  strDoubleE(double d,size_t prec=0,size_t wdt=0);
string  strInt(int d,size_t wdt=0);
bool  complex_command(string ccom,string& who,size_t& n,string& com);

class  file_name{
  string file_,name_;
public:
  file_name():file_(""),name_(""){}
  string file(){return file_;}
  string name(){return name_;}
  void name(string s);
  bool empty(){return file_.empty();} 
  string dir();
  string operator()(){return file_;}
};

} // Goblin

#endif
