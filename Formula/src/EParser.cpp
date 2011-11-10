#include "Formula/EParser.h"

//#include <boost/spirit/include/qi.hpp>
#include <locale>
#include <stdexcept>
#include <iostream>

namespace Formula
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
    m_start = m_end = end;
    m_hasMatch = this->matchEmpty();
    m_empty = true;
    return m_end;
  }
  m_start = start;
  m_end = this->test(start,end);

  if (m_end == m_start)
  {
    m_end = m_start; //?
    m_empty = true;
  }
  else
  {
    m_hasMatch = true;
  }

  return m_end;
}

/**
 * Overloaded method to match the whole string
 */
std::string::const_iterator IParser::match(const std::string& str)
{
  return match(str.begin(),str.end());
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
  return std::find_if_not(start,end,isalnum);
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

  SeqParser* terms = new SeqParser;
  terms->addParser(new CharParser(' '),'*');
  terms->addParser(new CharParser(m_operators->getBinSymbols()),'*');
  terms->addParser(new CharParser(' '),'*');
  terms->addParser(new TermParser());

  ListParser* otherTerms = static_cast<ListParser*>(expr.addParser(terms,'*'));

  expr.match(start,end);

  if (expr.hasMatch())
  {
    if (terms->isEmpty())
    {// One term
      if ( !un->isEmpty() )
      {// has unary operator
        std::string un_op = un->match();
        m_funct = un_op;
        m_terms.push_back(EParser());
        EParser& ep = m_terms.back();
        ep.setFunct(firstTerm->getParser());
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
      EParser& ep = addTerm(firstTerm->getParser());
      if (un->hasMatch())
      {
        ep.m_op = un->match();
      }
      for(size_t i = 0; i < otherTerms->size(); ++i)
      {
        SeqParser* term = static_cast<SeqParser*>(otherTerms->getParser(i));
        if (term->hasMatch())
        {
          EParser& ep = addTerm(term->getParser(3)); // TermParser
          ep.m_op = term->getParser(1)->match();
        }
      }
    }
    std::cerr << "Found " << expr.match() << std::endl;
  }
  else
  {
    std::cerr << "Parsing failed\n";
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
    EParser ep;
    ep.parse(fun->getInnerStart(),fun->getInnerEnd());
    if (ep.m_funct != "void")
    {
      m_terms.push_back(ep);
    }
    else
    {
      for(auto term = ep.m_terms.begin();term!=ep.m_terms.end();++term)
      {
        m_terms.push_back(*term);
        //EParser& ep = m_terms.back();
      }
    }
    return;
  }
  
  //addParser(new BracketParser);

  throw std::runtime_error("setFunct failed");
}

EParser& EParser::addTerm(IParser* parser)
{
  m_terms.push_back(EParser());
  EParser& ep = m_terms.back();
  ep.setFunct(parser);
  return ep;
}

void EParser::log(const std::string& padding)
{
  std::cerr << padding << m_op << '['<<m_funct<<']';
  if (!m_terms.empty())
  {
    std::cerr <<"(";
  }
  std::cerr << std::endl;
  for(auto term=m_terms.begin(); term!=m_terms.end();++term)
  {
    term->log(padding + "  ");
  }
  std::cerr << padding ;
  if (!m_terms.empty())
  {
    std::cerr <<")";
  }
  std::cerr << std::endl;
}

} // Formula
