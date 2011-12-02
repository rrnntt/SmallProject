#ifndef PROBLEM_TEXT_H
#define PROBLEM_TEXT_H

#include "Teach/DllExport.h"
#include "Kernel/EParser.h"
#include <boost/ptr_container/ptr_vector.hpp>

#include <ostream>
#include <istream>
#include <vector>
#include <string>

namespace Teach
{

class TEACH_EXPORT Text
{
public:
  Text(){}
  ~Text(){}
  class TEACH_EXPORT Part
  {
  public:
    virtual ~Part(){}
    virtual void print(std::ostream& os)const {}
  };
  void append(Part* part);
  void print(std::ostream& os, bool endline = false)const;
  void read(std::istream& is);
  void read(std::string& str);
protected:
  boost::ptr_vector<Part> m_parts;
};

class TEACH_EXPORT StringPart: public Text::Part
{
public:
  StringPart(const std::string& str):m_string(str){}
  virtual void print(std::ostream& os)const { os << m_string; }
protected:
  std::string m_string;
};

class TEACH_EXPORT ExpressionPart: public Text::Part
{
public:
  ExpressionPart(const std::string& str);
  virtual void print(std::ostream& os)const;
protected:
  Kernel::EParser m_expression;
};

} // Problem

TEACH_EXPORT std::ostream& operator<<(std::ostream& os,const Teach::Text& text);

#endif // PROBLEM_TEXT_H
