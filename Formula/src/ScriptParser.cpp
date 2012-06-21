#include "Formula/ScriptParser.h"

namespace Formula
{

/**
 * Constructor.
 */
ScriptParser::ScriptParser()
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
  // first try syntax construct
}

//-----------------------------------------------------------------------------------

/**
 * Constructor. Build up the alt-parser
 */
SyntaxParser::SyntaxParser()
{
  // block parser: finds curly brackets {...}
  m_blockParser = new Kernel::BracketParser("{","}");
  addParser( m_blockParser );

  // lines parser: 
  m_linesParser = new Kernel::SeqParser;
  auto singleLine = new Kernel::SeqParser;
  singleLine->addParser(this->clone());
  singleLine->addParser(new Kernel::CharParser(';'));
  m_linesParser->addParser( singleLine, '+' );
  addParser( m_linesParser );

  // 'if' parser: finds if-statements
  m_expression = new Kernel::AllParser;
  addParser( m_expression );
}

} // Formula
