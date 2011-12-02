#include "Teach/Text.h"
#include "Kernel/EParser.h"

using namespace Kernel;

namespace Teach
{

  void Text::append(Part* part)
  {
    m_parts.push_back(part);
  }

  void Text::print(std::ostream& os, bool endline)const
  {
    for(auto p = m_parts.begin(); p != m_parts.end(); ++p)
    {
      p->print(os);
      os << ' ';
    }
    if (endline)
    {
      os << std::endl;
    }
  }

  void Text::read(std::istream& is)
  {
    std::string str;
    while( std::getline(is,str) )
    {
      if ( str.empty() )
      {
        return;
      }
      read(str);
    }
  }

  void Text::read(std::string& str)
  {
    SeqParser seq; // parse entire string
    SeqParser* part = new SeqParser; // parse a part
    AltParser* word = new AltParser; // parse a word or object
    word->addParser(new BracketParser("[$","$]")); // get an equation
    word->addParser(new WordParser); // get a word. Must be the last parser in word as it matches every non-space symbols
    part->addParser(word);
    part->addParser(new CharParser(' '),'*');
    seq.addParser(part,'+');
    seq.match(str);
    const ListParser* list = dynamic_cast<const ListParser*>(seq.getParser(0));
    if (list && list->hasMatch())
    {
      for(size_t i = 0; i < list->size(); ++i)
      {
        const AltParser* alt = list->get<const AltParser>(i,0);
        if (alt && alt->hasMatch())
        {
          const IParser* what = alt->getParser();
          const WordParser* wrd = dynamic_cast<const WordParser*>(what);
          const BracketParser* bk = dynamic_cast<const BracketParser*>(what);
          if (wrd)
          {
            append(new StringPart(alt->match()));
          }
          else if (bk)
          {
            append(new ExpressionPart(std::string(bk->getInnerStart(),bk->getInnerEnd())));
          }
        }
      }
    }
  }

  ExpressionPart::ExpressionPart(const std::string& str)
  {
    m_expression.parse(str);
  }

  void ExpressionPart::print(std::ostream& os)const
  {
    os << m_expression.str();
  }

} // Problem

std::ostream& operator<<(std::ostream& os,const Teach::Text& text)
{
  text.print(os);
  return os;
}

