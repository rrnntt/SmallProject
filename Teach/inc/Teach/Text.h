#ifndef PROBLEM_TEXT_H
#define PROBLEM_TEXT_H

#include "Problem/DllExport.h"
#include <boost/ptr_container/ptr_vector.hpp>

#include <ostream>
#include <vector>

namespace Problem
{

class PROBLEM_EXPORT Text
{
public:
  Text(){}
  ~Text(){}
  class PROBLEM_EXPORT Part
  {
  public:
    virtual ~Part(){}
    //virtual void print(std::ostream& os)const = 0;
    virtual std::ostream& operator<<(std::ostream& os)const = 0;
  };
  void append(Part* part);
protected:
  boost::ptr_vector<Part> m_parts;
};

class StringPart: public Text::Part
{
public:
  //virtual void print(std::ostream& os)const { os << m_string; }
  virtual std::ostream& operator<<(std::ostream& os)const { /*os << m_string;*/ return os;}
protected:
  std::string m_string;
};

} // Problem

#endif // PROBLEM_TEXT_H
