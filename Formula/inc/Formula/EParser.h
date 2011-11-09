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
  //------------------------------------------------------------
  class FORMULA_EXPORT IParser
  {
  public:
    IParser():m_hasMatch(false){}
    virtual ~IParser(){}
    virtual IParser* clone() const = 0;
    virtual std::string::const_iterator match(std::string::const_iterator start,std::string::const_iterator end);
    virtual std::string::const_iterator match(const std::string& str);
    bool hasMatch() const {return m_hasMatch;}
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
    bool m_hasMatch;
  };

  //------------------------------------------------------------
  class FORMULA_EXPORT CharParser: public IParser
  {
  public:
    CharParser(char c = 0):IParser(){if (c) m_chars.push_back(c);}
    CharParser(const std::string& chars):IParser(),m_chars(chars){}
    CharParser(const CharParser& chp):IParser(),m_chars(chp.m_chars){}
    IParser* clone() const{return new CharParser(*this);}
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
    std::string m_chars; ///< alternative matches
  };

  //------------------------------------------------------------
  class FORMULA_EXPORT EmptyParser: public IParser
  {
  public:
    EmptyParser():IParser(){}
    EmptyParser(const EmptyParser& chp){}
    IParser* clone() const{return new EmptyParser(*this);}
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) 
    {
      m_hasMatch = true;
      return start;
    }
  };

  //------------------------------------------------------------
  class FORMULA_EXPORT MultiParser: public IParser
  {
  public:
    MultiParser():IParser(){}
    MultiParser(const MultiParser& p);
    ~MultiParser();
    size_t size()const{return m_parsers.size();}
    IParser* getParser(size_t i)const{return m_parsers.at(i).parser;}
  protected:
    IParser* addParser(IParser* parser);
    struct ParserHolder
    {
      IParser* parser;
    };
    std::vector<ParserHolder> m_parsers;
  };

  //------------------------------------------------------------
  class FORMULA_EXPORT SeqParser: public MultiParser
  {
  public:
    SeqParser():MultiParser(){}
    SeqParser(const SeqParser& p);
    IParser* clone()const{return new SeqParser(*this);}
    using MultiParser::addParser;
    IParser* addParser(IParser* parser, char m);
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
  };

  //------------------------------------------------------------
  class FORMULA_EXPORT ListParser: public MultiParser
  {
  public:
    enum Mutiplicity {One,OneMany,ZeroMany};
    ListParser(IParser* p, Mutiplicity m = OneMany);
    ListParser(const ListParser& p);
    IParser* clone()const{return new ListParser(*this);}
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
    Mutiplicity m_multiplicity;
  };

  //------------------------------------------------------------
  class FORMULA_EXPORT AltParser: public MultiParser
  {
  public:
    AltParser():MultiParser(){}
    AltParser(const AltParser& p);
    IParser* clone()const{return new AltParser(*this);}
    using MultiParser::addParser;
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
  };

  //------------------------------------------------------------
  class FORMULA_EXPORT WordParser: public IParser
  {
  public:
    WordParser():IParser(){}
    WordParser(const WordParser& p);
    IParser* clone()const{return new WordParser(*this);}
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
  };

  //------------------------------------------------------------
  class FORMULA_EXPORT VarNameParser: public IParser
  {
  public:
    VarNameParser():IParser(){}
    VarNameParser(const VarNameParser& p){}
    IParser* clone()const{return new VarNameParser(*this);}
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
  };

  //------------------------------------------------------------
  class FORMULA_EXPORT NumberParser: public IParser
  {
  public:
    NumberParser():IParser(){}
    NumberParser(const NumberParser& p){}
    IParser* clone()const{return new NumberParser(*this);}
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
  };

  //------------------------------------------------------------
  class FORMULA_EXPORT TermParser: public AltParser
  {
  public:
    TermParser();
    TermParser(const TermParser& p);
    IParser* clone()const{return new TermParser(*this);}
  };

  //------------------------------------------------------------
  class FORMULA_EXPORT BracketParser: public MultiParser
  {
  public:
    BracketParser(IParser* p,bool recursive);
    BracketParser(const std::string& bra, const std::string& ket);
    BracketParser(const BracketParser& p);
    IParser* clone()const{return new BracketParser(*this);}
  protected:
    enum Part{Bra,Ket,Other};
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
    std::string::const_iterator isBraKet(const std::string& bra_or_ket, std::string::const_iterator start,std::string::const_iterator end) ;
    std::string m_bra;
    std::string m_ket;
    bool m_recursive;
  };

  //------------------------------------------------------------
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
