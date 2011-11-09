#include "Formula/EParser.h"

//#include <boost/spirit/include/qi.hpp>
#include <locale>
#include <stdexcept>

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
    return m_end;
  }
  m_start = start;
  m_end = this->test(start,end);

  if (m_end == m_start)
  {
    m_end = m_start;
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
MultiParser(p)
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
  addParser(new VarNameParser);
  addParser(new NumberParser);
}

TermParser::TermParser(const TermParser& p):
AltParser(p)
{
}

//-----------------------------------------------------
BracketParser::BracketParser(IParser* p,bool recursive):
MultiParser(),
m_bra("("),
m_ket(")"),
m_recursive(recursive)
{
  addParser(p);
}

BracketParser::BracketParser(const std::string& bra, const std::string& ket):
MultiParser(),
m_bra(bra),
m_ket(ket)
{
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
    auto inner_start = start + m_bra.size();
    auto inner_end = end - m_ket.size();
    IParser* parser = getParser(0);
    parser->match(inner_start,inner_end);
    if (parser->hasMatch())
    {
      return it;
    }
  }
  return start;
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
}

} // Formula
