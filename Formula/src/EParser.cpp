#include "Formula/EParser.h"

#include <boost/spirit/include/qi.hpp>
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

  return m_end;
}

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

void MultiParser::addParser(IParser* parser)
{
  ParserHolder holder;
  holder.parser = parser;
  m_parsers.push_back(holder);
}

SeqParser::SeqParser(const SeqParser& p):
MultiParser(p)
{
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

ListParser::ListParser(IParser* p)
{
  if (p == NULL)
  {
    throw std::runtime_error("ListParser cannot be initialized with NULL parser");
  }
  addParser(p);
}

ListParser::ListParser(const ListParser& p):
MultiParser(p)
{
}

std::string::const_iterator ListParser::test(std::string::const_iterator start,std::string::const_iterator end) 
{
  auto it = start;
  auto h = m_parsers.begin();
  it = h->parser->match(it,end);
  //if (it == start)
  while(h->parser->hasMatch())
  {
    addParser(h->parser->clone());
    h = m_parsers.begin() + m_parsers.size() - 1;
    it = h->parser->match(it,end);
  }

  return it;
}

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
