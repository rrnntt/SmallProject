#ifndef FORMULA_EPARSER_H
#define FORMULA_EPARSER_H

#include "Formula/DllExport.h"
#include "Formula/Operators.h"

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <map>
#include <set>

namespace Formula
{

  class FORMULA_EXPORT IParser
  {
  public:
    virtual ~IParser(){}
    virtual std::string::const_iterator match(std::string::const_iterator start,std::string::const_iterator end);
    bool hasMatch() const {return m_start != m_end;}
    std::string match() const {return std::string(m_start,m_end);}
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) const = 0;
    std::string::const_iterator m_start;
    std::string::const_iterator m_end;
  };

  class FORMULA_EXPORT CharParser: public IParser
  {
  public:
    CharParser(char c = 0){if (c) m_chars.push_back(c);}
    CharParser(const std::string& chars):m_chars(chars){}
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) const;
    std::string m_chars; ///< alternative matches
  };

class FORMULA_EXPORT EParser
{
public:
    EParser();
    void parse(const std::string& str);
private:

    std::string m_funct; ///< Function name
    std::string m_op;    ///< Operator connecting this expression to its sibling on the left
    std::vector<EParser> m_terms;///< Child expressions (function arguments)

    Operators_ptr m_operators;

};

} // Formula

#endif // FORMULA_EPARSER_H
