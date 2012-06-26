#include "Formula/ScriptParser.h"

namespace Formula
{

/**
 * Constructor.
 */
ScriptParser::ScriptParser():
m_linesParser( new SyntaxParser, Kernel::ListParser::OneMany )
{
}

/**
 * Destructor.
 */
ScriptParser::~ScriptParser()
{
}

/**
 * Parse the script in a string
 * @param str :: A string with some script
 */
void ScriptParser::parse(const std::string& str)
{
  m_lines.clear();
  // first try syntax construct
  m_linesParser.match( str );
  std::cerr << m_linesParser.match() << std::endl;
  for(size_t i = 0; i < m_linesParser.size(); ++i)
  {
    auto line = m_linesParser.get<SyntaxParser>( i );
    if ( !line || line->isEmpty() ) continue;
    //std::cerr << "=>" << line->getParser(0)->match() << std::endl;
    m_lines.push_back( line );
  }
  // if parser didn't get to the end of the str and the trailing chars are not spaces:
  // it is a syntax error
  if (m_linesParser.getEnd() != str.end())
  {
    std::string tail(m_linesParser.getEnd(), str.end());
    auto i = tail.find_first_not_of(" \t\n");
    if ( i == std::string::npos )
    {
      tail.clear();
    }
    if ( !tail.empty() )
    {
      if ( tail.size() > 40 )
      {
        tail.erase(40);
        tail += "...";
      }
      throw std::runtime_error("Syntax error in: " + tail );
    }
  }
}

//-----------------------------------------------------------------------------------

/**
 * Constructor. 
 */
SyntaxParser::SyntaxParser()
{
  init();
}

/**
 * Copy constructor.
 */
SyntaxParser::SyntaxParser(const SyntaxParser& sp):
Kernel::SeqParser()// do not copy, all SyntaxParsers are the same
{
  init();
}

/**
 * Virtual copy constructor.
 */
Kernel::IParser* SyntaxParser::clone() const
{
  return new SyntaxParser(*this);
}

/**
 * Create all internals.
 */
void SyntaxParser::init()
{
  auto line = new Kernel::AltParser;
  // block parser: finds curly brackets {...}
  m_blockParser = new Kernel::BracketParser("{","}");
  line->addParser( m_blockParser );

  // 'if' parser: finds if-statements
  m_ifParser = new IfParser;
  line->addParser( m_ifParser );

  // var parser: finds variable definitions
  m_varParser = new VarParser;
  line->addParser( m_varParser );

  // expression - it's what the other parsers couldn't parse
  m_expression = new Kernel::NotStringParser(";");
  line->addParser( m_expression );

  line->addParser( new Kernel::EmptyParser );

  this->addParser( line );
  this->addParser( new Kernel::CharParser(" \t\n"), '*' );
  this->addParser( new Kernel::CharParser(";") );
  this->addParser( new Kernel::CharParser(" \t\n"), '*' );
}

/**
 * Returns a pointer to a BracketParser if there was a match to a block {...}
 * or nullptr otherwise
 */
Kernel::BracketParser* SyntaxParser::block() const
{
  return m_blockParser->hasMatch() ? m_blockParser : nullptr;
}

/**
 * Returns a pointer to a IParser if there was a match to an expression
 * or nullptr otherwise
 */
Kernel::IParser* SyntaxParser::expression() const
{
  return m_expression->hasMatch() ? m_expression : nullptr;
}

/**
 * Returns a pointer to a IfParser if there was a match to an if-block
 * or nullptr otherwise
 */
IfParser* SyntaxParser::ifBlock() const
{
  return m_ifParser->hasMatch() ? m_ifParser : nullptr;
}

/**
 * Returns a pointer to a VarParser if there was a match to a variable declaration
 * or nullptr otherwise
 */
VarParser* SyntaxParser::defVar() const
{
  return m_varParser->hasMatch() ? m_varParser : nullptr;
}


//-----------------------------------------------------------------------------
/**
 * Constructor.
 */
IfParser::IfParser():Kernel::SeqParser()
{
  m_conditionBlock = new Kernel::BracketParser("(",")");
  m_trueBlock = new Kernel::BracketParser("{","}");
  addParser( new Kernel::StringParser("if") );
  addParser( new Kernel::CharParser(" \t\n"), '*' );
  addParser( m_conditionBlock );
  addParser( new Kernel::CharParser(" \t\n"), '*' );
  addParser( m_trueBlock );
  assert( size() == 5 );

  m_elseBlock = new Kernel::BracketParser("{","}");
  auto elseBlock = new Kernel::SeqParser;
  elseBlock->addParser( new Kernel::CharParser(" \t\n"), '*' );
  elseBlock->addParser( new Kernel::StringParser("else") );
  elseBlock->addParser( new Kernel::CharParser(" \t\n"), '*' );
  elseBlock->addParser( m_elseBlock );

  addParser( elseBlock, '*' );
  assert( size() == 6 );
}

Kernel::BracketParser* IfParser::condition() const
{
  return m_conditionBlock;
}

Kernel::BracketParser* IfParser::trueBlock() const
{
  return m_trueBlock;
}

Kernel::BracketParser* IfParser::elseBlock() const
{
  return m_elseBlock->hasMatch() ? m_elseBlock : nullptr;
}

//-----------------------------------------------------------------------------
/**
 * Constructor.
 */
VarParser::VarParser():Kernel::SeqParser()
{
  m_type = new Kernel::VarNameParser;
  m_var = new Kernel::VarNameParser;
  m_value = new Kernel::NotStringParser(";");

  // parse the type
  addParser( m_type );
  addParser( new Kernel::CharParser(" \t\n"), '+' );
  // parse the name
  addParser( m_var );

  // optionally parse the initial value
  auto valueSeq = new Kernel::SeqParser;
  valueSeq->addParser( new Kernel::CharParser(" \t\n"), '*' );
  valueSeq->addParser( new Kernel::CharParser("=") );
  valueSeq->addParser( new Kernel::CharParser(" \t\n"), '*' );
  valueSeq->addParser( m_value );

  addParser( valueSeq, '*' );
}

Kernel::IParser* VarParser::type() const
{
  return m_type;
}

Kernel::IParser* VarParser::var() const
{
  return m_var;
}

Kernel::IParser* VarParser::value() const
{
  return m_value->hasMatch() ? m_value : nullptr;
}

} // Formula
