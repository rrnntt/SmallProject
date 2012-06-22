#include "Formula/ScriptParser.h"

namespace Formula
{

/**
 * Constructor.
 */
ScriptParser::ScriptParser()
{
  // lines parser: 
  m_linesParser.addParser( new SyntaxParser, '+' );

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
  m_linesParser.match( str );
  std::cerr << m_linesParser.match() << std::endl;
  m_linesParser.log();
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

  // 'if' parser: finds if-statements

  // expression - it's what the other parsers couldn't parse
  m_expression = new Kernel::AllParser;
  addParser( m_expression );
}

} // Formula
