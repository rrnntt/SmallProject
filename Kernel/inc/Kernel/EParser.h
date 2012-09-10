#ifndef KERNEL_EPARSER_H
#define KERNEL_EPARSER_H

#include "Kernel/DllExport.h"
#include "Kernel/Operators.h"

//#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>

namespace Kernel
{
  //------------------------------------------------------------
  class KERNEL_EXPORT IParser
  {
  public:
    IParser():m_hasMatch(false),m_empty(false){}
    virtual ~IParser(){}
    virtual IParser* clone() const = 0;
    virtual std::string::const_iterator match(std::string::const_iterator start,std::string::const_iterator end);
    virtual std::string::const_iterator match(const std::string& str, std::string::size_type i = 0);
    virtual bool matchEmpty()const{return false;}
    bool hasMatch() const {return m_hasMatch;}
    bool isEmpty() const {return m_empty;/*m_start == m_end;*/}
    //std::string match() const {return std::string(m_start,m_end);}
    std::string match() const {return std::string(m_start,m_start + m_n);}
    std::string::const_iterator getStart()const{return m_start;}
    std::string::const_iterator getEnd()const{return m_start + m_n;}
    virtual void log(const std::string& padding = "") const = 0;
  protected:
    /**
    * Tries to match string starting at start. If unsuccessful returns start. If successful returns
    * the end of the matching string. Doesn't save the match.
    * @param start :: Start iterator
    * @param end :: End iterator
    */
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end)  = 0;
    std::string::const_iterator m_start;
    //std::string::const_iterator m_end;
    std::string::size_type m_n;
    bool m_hasMatch;
    bool m_empty;
  };

  //------------------------------------------------------------
  /**
   * Match 1 character from a list
   */
  class KERNEL_EXPORT CharParser: public IParser
  {
  public:
    CharParser(char c = 0):IParser(){if (c) m_chars.push_back(c);}
    CharParser(const std::string& chars):IParser(),m_chars(chars){}
    CharParser(const CharParser& chp):IParser(),m_chars(chp.m_chars){}
    IParser* clone() const{return new CharParser(*this);}
    void log(const std::string& padding = "") const 
    {
      std::cerr << padding << "CharParser[" << m_chars << "]:";
      if ( hasMatch() ) std::cerr << match();
      std::cerr << std::endl;
    }
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
    std::string m_chars; ///< alternative matches
  };

  //------------------------------------------------------------
  /**
   * Match 1 character not from a list
   */
  class KERNEL_EXPORT NotParser: public IParser
  {
  public:
    NotParser(const std::string& chars):IParser(),m_chars(chars){}
    NotParser(const NotParser& chp):IParser(),m_chars(chp.m_chars){}
    IParser* clone() const{return new NotParser(*this);}
    void log(const std::string& padding = "") const {std::cerr << padding << "NotParser " << std::endl;}
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
    std::string m_chars; ///< alternative matches
  };

  //------------------------------------------------------------
  /**
   * Match exact string
   */
  class KERNEL_EXPORT StringParser: public IParser
  {
  public:
    StringParser(const std::string& str):IParser(),m_string(str){}
    StringParser(const StringParser& p):IParser(),m_string(p.m_string){}
    IParser* clone() const{return new StringParser(*this);}
    void log(const std::string& padding = "") const {std::cerr << padding << "StringParser " << std::endl;}
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
    std::string m_string; ///< string to matche
  };

  //------------------------------------------------------------
  /**
   * Match all characters until a string is found.
   */
  class KERNEL_EXPORT NotStringParser: public IParser
  {
  public:
    NotStringParser(const std::string& str):IParser(),m_string(str){}
    NotStringParser(const NotStringParser& p):IParser(),m_string(p.m_string){}
    IParser* clone() const{return new NotStringParser(*this);}
    //bool matchEmpty()const{return true;}
    void log(const std::string& padding = "") const {std::cerr << padding << "NotStringParser " << std::endl;}
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
    std::string m_string; ///< doesn't match this string
  };

  //------------------------------------------------------------
  class KERNEL_EXPORT EmptyParser: public IParser
  {
  public:
    EmptyParser():IParser(){}
    EmptyParser(const EmptyParser& chp){}
    IParser* clone() const{return new EmptyParser(*this);}
    bool matchEmpty()const{return true;}
    void log(const std::string& padding = "") const {std::cerr << padding << "EmptyParser " << std::endl;}
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) 
    {
      m_hasMatch = true;
      m_empty = true;
      return start;
    }
  };

  //------------------------------------------------------------
  /**
   * Match all characters 
   */
  class KERNEL_EXPORT AllParser: public IParser
  {
  public:
    AllParser():IParser(){}
    AllParser(const AllParser& chp){}
    IParser* clone() const{return new AllParser(*this);}
    void log(const std::string& padding = "") const 
    {
      std::cerr << padding << "AllParser " ;
      if ( hasMatch() ) std::cerr << match() ;
      std::cerr << std::endl;
    }
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) 
    {
      m_empty = start == end;
      return end;
    }
  };

  //------------------------------------------------------------
  class KERNEL_EXPORT MultiParser: public IParser
  {
  public:
    MultiParser():IParser(){}
    MultiParser(const MultiParser& p);
    ~MultiParser();
    size_t size()const{return m_parsers.size();}
    IParser* getParser(size_t i)const{return m_parsers.at(i).parser;}
    template<class P>
    P* get(size_t i)const{return dynamic_cast<P*>(m_parsers.at(i).parser);}
    template<class P>
    P* get(size_t i,size_t j)const
    {
      return get<MultiParser>(i)->get<P>(j);
    }
    void logChildren(const std::string& padding = "") const 
    {
      for(auto it=m_parsers.begin(); it != m_parsers.end(); ++it)
      {
        it->parser->log(padding + "  ");
      }
    }
  protected:
    IParser* addParser(IParser* parser);
    struct ParserHolder
    {
      IParser* parser;
    };
    std::vector<ParserHolder> m_parsers;
  };

  //------------------------------------------------------------
  class KERNEL_EXPORT SeqParser: public MultiParser
  {
  public:
    SeqParser():MultiParser(){}
    SeqParser(const SeqParser& p);
    IParser* clone()const{return new SeqParser(*this);}
    using MultiParser::addParser;
    IParser* addParser(IParser* parser, char m);
    void log(const std::string& padding = "") const 
    {
      std::cerr << padding << "SeqParser" << std::endl;
      logChildren(padding);
    }
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
  };

  //------------------------------------------------------------
  class KERNEL_EXPORT ListParser: public MultiParser
  {
  public:
    enum Mutiplicity {One,OneMany,ZeroMany};
    ListParser(IParser* p, Mutiplicity m = OneMany);
    ListParser(const ListParser& p);
    IParser* clone()const{return new ListParser(*this);}
    bool matchEmpty()const{return m_multiplicity == ZeroMany;}
    void log(const std::string& padding = "") const 
    {
      std::cerr << padding << "ListParser" << std::endl;
      logChildren(padding);
    }
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
    Mutiplicity m_multiplicity;
  };
  
  //------------------------------------------------------------
  /**
   * Parse lists
   */
  //class KERNEL_EXPORT TokenParser: public ListParser
  //{
  //public:
  //  TokenParser(const std::string& sep);
  //};

  //------------------------------------------------------------
  class KERNEL_EXPORT AltParser: public MultiParser
  {
  public:
    AltParser():MultiParser(){}
    AltParser(const AltParser& p);
    IParser* clone()const{return new AltParser(*this);}
    bool matchEmpty()const;
    using MultiParser::addParser;
    IParser* getParser()const{return m_goodParser;}
    void log(const std::string& padding = "") const 
    {
      std::cerr << padding << "AltParser" << std::endl;
      logChildren(padding);
    }
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
    IParser* m_goodParser; ///< parser that had a match
  };

  //------------------------------------------------------------
  class KERNEL_EXPORT WordParser: public IParser
  {
  public:
    WordParser(const std::string& exclude = ""):IParser(),m_exclude(exclude){}
    WordParser(const WordParser& p);
    IParser* clone()const{return new WordParser(*this);}
    void log(const std::string& padding = "") const {std::cerr << padding << "WordParser " << std::endl;}
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
    std::string m_exclude;
  };

  //------------------------------------------------------------
  class KERNEL_EXPORT VarNameParser: public IParser
  {
  public:
    VarNameParser():IParser(){}
    VarNameParser(const VarNameParser& p){}
    IParser* clone()const{return new VarNameParser(*this);}
    void log(const std::string& padding = "") const {std::cerr << padding << "VarNameParser " << std::endl;}
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
  };

  //------------------------------------------------------------
  class KERNEL_EXPORT NumberParser: public IParser
  {
  public:
    NumberParser():IParser(){}
    NumberParser(const NumberParser& p){}
    IParser* clone()const{return new NumberParser(*this);}
    void log(const std::string& padding = "") const {std::cerr << padding << "NumberParser " << std::endl;}
  protected:
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
  };

  //------------------------------------------------------------
  class KERNEL_EXPORT BracketParser: public MultiParser
  {
  public:
    BracketParser(IParser* p = nullptr);
    BracketParser(const std::string& bra, const std::string& ket, IParser* p = nullptr);
    BracketParser(const BracketParser& p);
    IParser* clone()const{return new BracketParser(*this);}
    std::string::const_iterator getInnerStart()const{return m_start + m_bra.size();}
    std::string::const_iterator getInnerEnd()const{return m_start + m_n - m_ket.size();}
    //IParser* getInnerParser()const{return getParser(0);}
    bool isInnerEmpty() const {return isEmpty() || m_n <= m_bra.size() + m_ket.size();}
    void log(const std::string& padding = "") const 
    {
      std::cerr << padding << "BracketParser ";
      if ( hasMatch() ) std::cerr << match() ;
      std::cerr << std::endl;
      logChildren(padding);
    }
  protected:
    enum Part{Bra,Ket,Other};
    virtual std::string::const_iterator test(std::string::const_iterator start,std::string::const_iterator end) ;
    std::string::const_iterator isBraKet(const std::string& bra_or_ket, std::string::const_iterator start,std::string::const_iterator end) ;
    std::string m_bra;
    std::string m_ket;
  };

  //------------------------------------------------------------
  class KERNEL_EXPORT NameBracketParser: public SeqParser
  {
  public:
    NameBracketParser(IParser* p = nullptr);
    NameBracketParser(const std::string& bra, const std::string& ket, IParser* p = nullptr);
    NameBracketParser(const NameBracketParser& p);
    IParser* clone()const{return new NameBracketParser(*this);}
    std::string::const_iterator getInnerStart()const{return m_brackets->getInnerStart();}
    std::string::const_iterator getInnerEnd()const{return m_brackets->getInnerEnd();}
    void log(const std::string& padding = "") const 
    {
      std::cerr << padding << "NameBracketParser" << std::endl;
      logChildren(padding);
    }
  protected:
    VarNameParser* m_name;
    BracketParser* m_brackets;
  };

  //------------------------------------------------------------
  class KERNEL_EXPORT TermParser: public AltParser
  {
  public:
    TermParser();
    TermParser(const TermParser& p);
    IParser* clone()const{return new TermParser(*this);}
    void log(const std::string& padding = "") const 
    {
      std::cerr << padding << "TermParser" << std::endl;
      logChildren(padding);
    }
  };

  //------------------------------------------------------------
  class KERNEL_EXPORT EParser
  {
  public:
    EParser();
    EParser(const std::vector<std::string>& binary,const std::set<std::string>& unary);
    EParser(Operators_ptr operators);
    EParser(const EParser& p);
    EParser(const EParser* p);
    ~EParser();
    void parse(const std::string& str);
    void log(const std::string& padding = "")const;
    Operators_ptr operators() const {return m_operators;}
      /// Returns this expression as a string. It does not simply returns the input string 
      /// but recreates it.
      std::string str(bool printOp = false)const;
    /// Returns true if the expression is a function (i.e. has arguments)
    bool isFunct()const{return m_terms.size() > 0;}
    /// Returns the name of the expression which is a function or variable name.
    std::string name()const{return m_funct;}
    /// Returns the the expression's binary operator on its left. Can be an empty string.
    std::string operator_name()const{return m_op;}
    /// Returns the top level terms of the expression (function arguments). For a variable
    /// it empty.
    const std::vector<EParser*>& terms()const{return m_terms;}
    /// Returns the number of argumens
    size_t size()const{return m_terms.size();}
    typedef std::vector<EParser*>::const_iterator iterator;
    iterator begin()const{return m_terms.begin();}
    iterator end()const{return m_terms.end();}
    const EParser& operator[](size_t i)const{return *m_terms.at(i);}
    std::set<std::string> getVariables() const;

    size_t getStartPos() const {return m_ifrom;}
    size_t getStringSize() const {return m_n;}
    const EParser& parentOf(const EParser& p) const;
    EParser& parentOf(const EParser& p);

    /// Ensure the top function is a ','
    void toList();

  protected:

    void parse(const std::string& str, std::string::const_iterator start,std::string::const_iterator end);
    void setFunct(const std::string& str, IParser* parser);
    EParser* addTerm(const std::string& str, IParser* parser);
    void moveTerms(EParser* ep);
    void sortPrecedence();
    IParser* createBinParser()const;
    const EParser* findParentOf(const EParser* p) const;

    std::string m_funct; ///< Function name
    std::string m_op;    ///< Operator connecting this expression to its sibling on the left
    std::vector<EParser*> m_terms;///< Child expressions (function arguments)

    Operators_ptr m_operators;
    
    size_t m_ifrom;
    size_t m_n;

    static const std::string g_spaces;
  };

} // Kernel

#endif // KERNEL_EPARSER_H
