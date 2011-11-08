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
    virtual IParser* clone() const = 0;
    virtual std::string::const_iterator match(std::string::const_iterator start,std::string::const_iterator end);
    bool hasMatch() const {return m_start != m_end;}
    std::string match() const {return std::string(m_start,m_end);}
  protected:
    /**
    * Tries to match string starting at start. If unsuccessful returns start. If successful returns
    * the end of the matching string. Doesn't save the match.
    * @param start :: Start iterator
    * @param end :: End iterator
    */
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end)  = 0;
    std::string::const_iterator m_start;
    std::string::const_iterator m_end;
  };

  class FORMULA_EXPORT CharParser: public IParser
  {
  public:
    CharParser(char c = 0){if (c) m_chars.push_back(c);}
    CharParser(const std::string& chars):m_chars(chars){}
    CharParser(const CharParser& chp):m_chars(chp.m_chars){}
    IParser* clone() const{return new CharParser(*this);}
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
    std::string m_chars; ///< alternative matches
  };

  class FORMULA_EXPORT MultiParser: public IParser
  {
  public:
    MultiParser(){}
    MultiParser(const MultiParser& p);
    ~MultiParser();
  protected:
    void addParser(IParser* parser);
    struct ParserHolder
    {
      IParser* parser;
    };
    std::vector<ParserHolder> m_parsers;
  };

  class FORMULA_EXPORT SeqParser: public MultiParser
  {
  public:
    SeqParser():MultiParser(){}
    SeqParser(const SeqParser& p);
    IParser* clone()const{return new SeqParser(*this);}
    using MultiParser::addParser;
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
  };

  class FORMULA_EXPORT ListParser: public MultiParser
  {
  public:
    ListParser(IParser* p);
    ListParser(const ListParser& p);
    IParser* clone()const{return new ListParser(*this);}
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
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
