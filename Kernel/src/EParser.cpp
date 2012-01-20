#include "Kernel/EParser.h"

//#include <boost/spirit/include/qi.hpp>
#include <boost/scoped_ptr.hpp>
#include <locale>
#include <stdexcept>
#include <iostream>
#include <sstream>

namespace Kernel
{

  /**
   * Tries to match string starting at start. If unsuccessful returns start. If successful returns
   * the end of the matching string.
   * Saves the match which can be retrieved with match(void) method.
   * @param start :: Start iterator
   * @param end :: End iterator
   */
std::string::const_iterator IParser::match(std::string::const_iterator start,std::string::const_iterator end)
{
  m_hasMatch = false;
  if (start == end)
  {
    m_start = end;
    m_n = 0;
    m_hasMatch = this->matchEmpty();
    m_empty = true;
    return m_start;
  }
  m_start = start;
  std::string::const_iterator _end = this->test(start,end);

  if (_end == m_start)
  {
    m_n = 0;
    m_empty = true;
  }
  else
  {
    m_hasMatch = true;
  }
  m_n = _end - m_start;

  return _end;
}

/**
 * Overloaded method to match the whole string
 */
std::string::const_iterator IParser::match(const std::string& str, std::string::size_type i)
{
  return match(str.begin() + i,str.end());
}

//-----------------------------------------------------
std::string::const_iterator CharParser::test(std::string::const_iterator start,std::string::const_iterator end) 
{
  if (m_chars.empty())
  {// match any char if string is not empty
    return ++start;
  }
  else
  {// match specific char (one of the m_chars)
    auto found = m_chars.find(*start);
    if ( found == std::string::npos )
    {
      return start;
    }
    else
    {
      return ++start;
    }
  }
  return start;
}

//-----------------------------------------------------
std::string::const_iterator NotParser::test(std::string::const_iterator start,std::string::const_iterator end) 
{
  if (m_chars.empty())
  {// match nothing
    return start;
  }
  else
  {// match any char that not in the m_chars)
    auto found = m_chars.find(*start);
    if ( found == std::string::npos )
    {// match
      return ++start;
    }
    else
    {// not match
      return start;
    }
  }
  return start;
}

//-----------------------------------------------------
std::string::const_iterator StringParser::test(std::string::const_iterator start,std::string::const_iterator end) 
{
  if (m_string.empty())
  {
    return start;
  }
  else
  {
    auto it = start;
    auto mit = m_string.begin();
    while( it != end && mit != m_string.end() && *it == *mit )
    {
      ++it;
      ++mit;
    }
    if (mit != m_string.end()) return start;
    return it;
  }
  return start;
}

//-----------------------------------------------------
std::string::const_iterator NotStringParser::test(std::string::const_iterator start,std::string::const_iterator end) 
{
  if (m_string.empty())
  {
    return end;
  }
  auto it = start;
  auto mit = m_string.begin();
  while( it != end && mit != m_string.end() )
  {
    if (*it != *mit)
    {
      mit = m_string.begin();
    }
    else
    {
      ++mit;
    }
    ++it;
  }
  if (mit == m_string.end())
  {// if m_string found return the part of the string which ends where m_string starts
    return it - m_string.size();
  }
  return end;
}

//-----------------------------------------------------
MultiParser::MultiParser(const MultiParser& p):
IParser()
{
  for(auto h = p.m_parsers.begin(); h != p.m_parsers.end(); ++h)
  {
    if (h->parser)
    {
      ParserHolder holder;
      holder.parser = h->parser->clone();
      m_parsers.push_back(holder);
    }
  }
}

MultiParser::~MultiParser()
{
  for(auto h = m_parsers.begin(); h != m_parsers.end(); ++h)
  {
    if (h->parser)
    {
      delete h->parser;
    }
  }
}

IParser* MultiParser::addParser(IParser* parser)
{
  ParserHolder holder;
  holder.parser = parser;
  m_parsers.push_back(holder);
  return parser;
}

SeqParser::SeqParser(const SeqParser& p):
MultiParser(p)
{
}

//-----------------------------------------------------
IParser* SeqParser::addParser(IParser* parser, char m)
{
  switch(m)
  {
  case '+': return addParser(new ListParser(parser,ListParser::OneMany));
  case '*': return addParser(new ListParser(parser,ListParser::ZeroMany));
  default:  return addParser(parser);
  }
  return NULL;
}

std::string::const_iterator SeqParser::test(std::string::const_iterator start,std::string::const_iterator end) 
{
  auto it = start;
  for(auto h = m_parsers.begin(); h != m_parsers.end(); ++h)
  {
    it = h->parser->match(it,end);
    if (!h->parser->hasMatch())
    {
      return start;
    }
  }

  return it;
}

//-----------------------------------------------------
ListParser::ListParser(IParser* p, Mutiplicity m)
:MultiParser(),
m_multiplicity(m)
{
  if (p == NULL)
  {
    throw std::runtime_error("ListParser cannot be initialized with NULL parser");
  }
  addParser(p);
}

ListParser::ListParser(const ListParser& p):
MultiParser(p),
m_multiplicity(p.m_multiplicity)
{
}

std::string::const_iterator ListParser::test(std::string::const_iterator start,std::string::const_iterator end) 
{
  auto h = m_parsers.begin();
  auto it = h->parser->match(start,end);
  if ( !h->parser->hasMatch() && m_multiplicity == ZeroMany )
  {
    m_hasMatch = true;
    m_empty = true;
    return start;
  }

  if (m_multiplicity == One)
  {
    return it;
  }

  while(h->parser->hasMatch())
  {
    addParser(h->parser->clone());
    h = m_parsers.begin() + m_parsers.size() - 1;
    it = h->parser->match(it,end);
  }

  return it;
}

//-----------------------------------------------------
AltParser::AltParser(const AltParser& p):
MultiParser(p),
m_goodParser(nullptr)
{
}

std::string::const_iterator AltParser::test(std::string::const_iterator start,std::string::const_iterator end) 
{
  auto it = start;
  for(auto h = m_parsers.begin();h != m_parsers.end(); ++h)
  {
    it = h->parser->match(it,end); // can use test(...) here?
    if (h->parser->hasMatch())
    {
      m_goodParser = h->parser;
      return it;
    }
  }
  return start;
}

//-----------------------------------------------------
WordParser::WordParser(const WordParser& p)
{
}

std::string::const_iterator WordParser::test(std::string::const_iterator start,std::string::const_iterator end) 
{
  if (m_exclude.empty())
  {
    return std::find_if(start,end,isspace);
  }
  const std::string& exc = m_exclude;
  return std::find_if(start,end,[&exc](char c)->bool{
    return isspace(c) || exc.find(c) != std::string::npos;
  });
}

//-----------------------------------------------------
std::string::const_iterator VarNameParser::test(std::string::const_iterator start,std::string::const_iterator end) 
{
  auto it = start;
  if ( !isalpha(*start) ) return start;
  ++it;
  return std::find_if_not(it,end,isalnum);
}

//-----------------------------------------------------
std::string::const_iterator NumberParser::test(std::string::const_iterator start,std::string::const_iterator end) 
{
  bool readDot = false;
  auto it = start;
  for(;it != end; ++it)
  {
    if (*it == '.')
    {
      if (!readDot)
      {
        readDot = true;
      }
      else
      {
        return it;
      }
    }
    else if (!isdigit(*it))
    {
      return it;
    }
  }
  return it;
}

//-----------------------------------------------------
TermParser::TermParser():
AltParser()
{
  addParser(new NameBracketParser);
  addParser(new BracketParser);
  addParser(new VarNameParser);
  addParser(new NumberParser);
}

TermParser::TermParser(const TermParser& p):
AltParser(p)
{
}

//-----------------------------------------------------
BracketParser::BracketParser(IParser* p):
MultiParser(),
m_bra("("),
m_ket(")")
{
  if (p)
  {
    addParser(p);
  }
  else
  {
    addParser(new AllParser);
  }
}

BracketParser::BracketParser(const std::string& bra, const std::string& ket, IParser* p):
MultiParser(),
m_bra(bra),
m_ket(ket)
{
  if (p)
  {
    addParser(p);
  }
  else
  {
    addParser(new AllParser);
  }
}

BracketParser::BracketParser(const BracketParser& p):
MultiParser(p),
m_bra(p.m_bra),
m_ket(p.m_ket)
{
}

std::string::const_iterator BracketParser::isBraKet(const std::string& bra_or_ket, std::string::const_iterator start,std::string::const_iterator end)
{
  auto it = start;
  auto bra_it = bra_or_ket.begin();
  auto bra_end = bra_or_ket.end();
  for(; bra_it != bra_end && it != end; ++bra_it,++it)
  {
    if (*it != *bra_it)
    {
      return start;
    }
  }
  return it;
}

std::string::const_iterator BracketParser::test(std::string::const_iterator start,std::string::const_iterator end) 
{
  if (size_t(end - start) < m_bra.size() + m_ket.size())
  {
    return start;
  }
  auto it = isBraKet(m_bra,start,end);
  if (it == start)
  {
    return start;
  }
  size_t level = 1;
  for(; it != end; )
  {
    auto it1 = isBraKet(m_bra,it,end);
    if (it1 != it)
    {
      it = it1;
      ++level;
    }
    else
    {
      it1 = isBraKet(m_ket,it,end);
      if (it1 != it)
      {
        it = it1;
        --level;
        if (level == 0) break;
      }
      else
      {
        ++it;
      }
    }
  }
  if (level == 0)
  {
    if (size() > 0)
    {
      IParser* parser = getParser(0);
      auto inner_start = start + m_bra.size();
      auto inner_end = end - m_ket.size();
      parser->match(inner_start,inner_end);
      if (parser->hasMatch())
      {
        return it;
      }
    }
    else
    {
      return it;
    }
  }
  return start;
}

//-----------------------------------------------------
NameBracketParser::NameBracketParser(IParser* p):
SeqParser()
{
  m_name = static_cast<VarNameParser*>(addParser(new VarNameParser));
  addParser(new CharParser(' '),'*');
  m_brackets = static_cast<BracketParser*>(addParser(new BracketParser(p)));
}

NameBracketParser::NameBracketParser(const std::string& bra, const std::string& ket, IParser* p):
SeqParser()
{
  m_name = static_cast<VarNameParser*>(addParser(new VarNameParser));
  addParser(new CharParser(' '),'*');
  m_brackets = static_cast<BracketParser*>(addParser(new BracketParser(bra,ket,p)));
}

NameBracketParser::NameBracketParser(const NameBracketParser& p):
SeqParser(p)
{
  m_name = static_cast<VarNameParser*>(getParser(0));
  m_brackets = static_cast<BracketParser*>(getParser(2));
}

//-----------------------------------------------------
/*--- EParser ---*/  
EParser::EParser()
{

  m_operators.reset(new Operators());
  // Define binary operators. Put them in the reverse precedence order (from lower to higher prec.)
  std::vector<std::string> ops;
  ops.push_back(";");
  ops.push_back(",");
  ops.push_back("= != > < <= >=");
  ops.push_back("&& || ^^");
  ops.push_back("+ -");
  ops.push_back("* /");
  ops.push_back("^");

  m_operators->add_operators(ops);

  // Define unary operators
  std::set<std::string> unary;
  unary.insert("+");
  unary.insert("-");

  m_operators->add_unary(unary);
}

/// contructor
EParser::EParser(const std::vector<std::string>& binary,const std::set<std::string>& unary)
{
  m_operators.reset(new Operators());
  m_operators->add_operators(binary);
  m_operators->add_unary(unary);
}

EParser::EParser(Operators_ptr operators)
{
  m_operators = operators;
}

EParser::EParser(const EParser& expr):
m_funct(expr.m_funct),
m_op(expr.m_op),
m_terms(expr.m_terms),
m_operators(expr.m_operators)
{
}

EParser::EParser(const EParser* pexpr)
:m_operators(pexpr->m_operators)
{
}

EParser::~EParser()
{
  for(auto ep = m_terms.begin(); ep !=m_terms.end(); ++ep)
  {
    if (*ep != nullptr)
    {
      delete *ep;
    }
  }
}

void EParser::parse(const std::string& str)
{
  parse(str.begin(),str.end());
}

void EParser::parse(std::string::const_iterator start,std::string::const_iterator end)
{
  m_terms.clear();
  m_op = "";
  m_funct = "";

  SeqParser expr;
  expr.addParser(new CharParser(' '),'*');
  IParser* un = expr.addParser(new CharParser(m_operators->getUnSymbols()),'*');
  expr.addParser(new CharParser(' '),'*');
  TermParser* firstTerm = static_cast<TermParser*>(expr.addParser(new TermParser()));

  std::string symbols = m_operators->getBinSymbols() + m_operators->getUnSymbols() + " ";
  SeqParser* terms = new SeqParser;
  //terms->addParser(new CharParser(' '),'*');
  terms->addParser(new CharParser(symbols),'+');
  //terms->addParser(new CharParser(' '),'*');
  terms->addParser(new TermParser());

  ListParser* otherTerms = static_cast<ListParser*>(expr.addParser(terms,'*'));

  expr.match(start,end);

  if (expr.hasMatch())
  {
    // 
    if (otherTerms->isEmpty())
    {// One term
      if ( !un->isEmpty() )
      {// has unary operator
        std::string un_op = un->match();
        m_funct = un_op;
        EParser* ep = new EParser;
        m_terms.push_back(ep);
        ep->setFunct(firstTerm->getParser());
      }
      else
      {// no unary operator
        setFunct(firstTerm->getParser());
      }
    }
    else
    {// multiple terms

      m_op = "";
      m_funct = "void";
      EParser* ep = addTerm(firstTerm->getParser());
      if (un->hasMatch())
      {
        ep->m_op = un->match();
      }
      for(size_t i = 0; i < otherTerms->size(); ++i)
      {
        SeqParser* term = static_cast<SeqParser*>(otherTerms->getParser(i));
        if (term->hasMatch())
        {
          EParser* ep = addTerm(term->getParser(1)); // TermParser
          ep->m_op = term->getParser(0)->match();    // List of CharParser
        }
      }
    }

    sortPrecedence();
  }
  else
  {
    m_funct = "void";
  }

}

void EParser::setFunct(IParser* parser)
{
  TermParser* term = dynamic_cast<TermParser*>(parser);
  if(term)
  {
    parser = term->getParser();
  }
  VarNameParser* var = dynamic_cast<VarNameParser*>(parser);
  NumberParser* num = dynamic_cast<NumberParser*>(parser);
  if (var || num)
  {
    m_funct = parser->match();
    return;
  }
  
  BracketParser* brak = dynamic_cast<BracketParser*>(parser);
  if (brak)
  {
    auto start = brak->getInnerStart();
    auto end = brak->getInnerEnd();
    this->parse(start,end);
    return;
  }

  NameBracketParser* fun = dynamic_cast<NameBracketParser*>(parser);
  if (fun)
  {
    m_funct = fun->getParser(0)->match();
    EParser* ep = new EParser;
    ep->parse(fun->getInnerStart(),fun->getInnerEnd());
    if (ep->m_funct != "void")
    {
      m_terms.push_back(ep);
    }
    else
    {
      moveTerms(ep);
      //for(auto term = ep.m_terms.begin();term!=ep.m_terms.end();++term)
      //{
      //  m_terms.push_back(*term);
      //  //EParser& ep = m_terms.back();
      //}
    }
    return;
  }
  
  //addParser(new BracketParser);

  throw std::runtime_error("setFunct failed");
}

EParser* EParser::addTerm(IParser* parser)
{
  EParser* ep = new EParser;
  ep->setFunct(parser);
  m_terms.push_back(ep);
  return ep;
}

IParser* EParser::createBinParser()const
{
  std::vector<std::string> ops = m_operators->getAllBinary();
  std::sort(ops.begin(),ops.end(),[](std::string& l,std::string& r)->bool{
    if (l.size() != r.size())
    {
      return l.size() > r.size();
    }
    return l > r;
  });
  AltParser* parser = new AltParser;
  std::for_each(ops.begin(),ops.end(),[&parser](std::string& s){
    parser->addParser(new StringParser(s));
  });
  return parser;
}

/**
 * Rearrange the terms according to operator precedence.
 */
void EParser::sortPrecedence()
{
  if ( !m_funct.empty() && m_funct != "void") return;
  if (m_terms.empty()) return;
  boost::scoped_ptr<IParser> BinaryParser(createBinParser());
  // Find the smallest precedence prec
  size_t prec = 1000;
  for(auto term=m_terms.begin(); term!=m_terms.end(); ++term)
  {
    // Build parser to parse a binary op followed by optional unary operator,
    // all surroun
    SeqParser op;
    IParser* bin = nullptr;
    // Binary ops start with the second term
    if (term != m_terms.begin())
    {
      op.addParser(new CharParser(' '),'*');
      //bin = op.addParser(new CharParser(m_operators->getBinSymbols()),'*');
      bin = op.addParser(BinaryParser->clone());
    }
    op.addParser(new CharParser(' '),'*');
    IParser* un = op.addParser(new CharParser(m_operators->getUnSymbols()),'*');
    op.addParser(new CharParser(' '),'*');
    //std::cerr << "op='" << (**term).m_op << "'\n";
    op.match((**term).m_op);
    std::string un_op = un->hasMatch() && !un->isEmpty() ? un->match() : "";
    std::string bin_op = bin && bin->hasMatch() ? bin->match() : "";
    // if term has a unary op, make that op the function and the term its argument
    if (!un_op.empty())
    {
      EParser* new_term = new EParser;
      new_term->m_terms.push_back(*term);
      *term = new_term;
      (**term).m_funct = un_op;
      (**term).m_terms[0]->m_op = "";
      if (bin && bin->hasMatch())
      {
        (**term).m_op = bin_op;
      }
      else
      {
        (**term).m_op = "";
      }
    }
    if (bin)
    {
      if (bin_op.empty()) bin_op = "*";
      
      (**term).m_op = bin_op;
      size_t p = m_operators->op_prec(bin_op);
      if (p < prec)
      {
        prec = p;
      }
    }
    // sort the children
    (**term).sortPrecedence();
  }

  m_funct = m_operators->what_is_binary_name(prec);
  std::vector<EParser*> new_terms;
  size_t i0 = 0;
  for(size_t i=1; i <= m_terms.size(); ++i)
  {
    if ( i == m_terms.size() || m_operators->precedence(m_terms[i]->m_op) == prec )
    {
      EParser* p = nullptr;
      if (i - i0 > 1)
      {
        p = new EParser;
        p->m_op = m_terms[i0]->m_op;
        m_terms[i0]->m_op = "";
        for(size_t j = i0; j < i; ++j)
        {
          p->m_terms.push_back(m_terms[j]);
        }
        p->sortPrecedence();
      }
      else
      {
        p = m_terms[i0];
      }
      i0 = i;
      new_terms.push_back(p);
    }
  }
  m_terms.assign(new_terms.begin(),new_terms.end());
}

void EParser::log(const std::string& padding)
{
  std::string op = m_op.empty() ? " " : m_op;
  std::cerr << padding << op << '['<<m_funct<<']';
  if (!m_terms.empty())
  {
    std::cerr <<"(";
  }
  std::cerr << std::endl;
  for(auto term=m_terms.begin(); term!=m_terms.end();++term)
  {
    (**term).log(padding + "  ");
  }
  if (!m_terms.empty())
  {
    std::cerr << padding ;
    std::cerr <<")";
    std::cerr << std::endl;
  }
}

/**
 * Move all terms from ep and push back it to this
 */
void EParser::moveTerms(EParser* ep)
{
  for(auto term=ep->m_terms.begin(); term!=ep->m_terms.end();++term)
  {
    m_terms.push_back(*term);
  }
  ep->m_terms.clear();
}

std::string EParser::str(bool printOp)const
{
  std::ostringstream res;
  if (printOp)
  {
    res << m_op.c_str();
  }
  size_t prec = m_operators->op_prec(m_funct);
  if (!prec || size() == 1)
  {
    res << m_funct.c_str();
    prec = 1000;
  }

  if (m_terms.size())
  {
    bool bk = prec > m_operators->op_prec(m_terms[0]->m_funct);
    if (bk) res << '(' ;
    for(size_t i=0;i<m_terms.size();i++)
    {
      res << m_terms[i]->str(true).c_str();
    }
    if (bk) res <<')';
  }
  return res.str();
}

std::set<std::string> EParser::getVariables() const
{
  std::set<std::string> vars;
  if (!isFunct())
  {
    vars.insert(m_funct);
    return vars;
  }

  for(auto t = begin(); t != end(); ++t)
  {
    std::set<std::string> v = (**t).getVariables();
    vars.insert(v.begin(),v.end());
  }
  return vars;
}



} // Kernel