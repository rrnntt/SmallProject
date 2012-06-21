#ifndef FORMULA_SCRIPTPARSER_H
#define FORMULA_SCRIPTPARSER_H

#include "Formula/DllExport.h"
#include "Kernel/EParser.h"

#include <vector>

namespace Formula
{

/**
 *  Parses syntax constructs: loops, ifs, etc.
 */
class SyntaxParser: public Kernel::AltParser
{
public:
  SyntaxParser();
private:
  /// Finds curly brackets
  Kernel::BracketParser* m_blockParser;
  /// Find lines separated by semi-colons ';'
  Kernel::SeqParser* m_linesParser;
  /// Collects all if other parsers failed - it must be an expression
  Kernel::AllParser* m_expression;
};

/**
 * Parses a complete unit of script: expression, block, module.
 */
class FORMULA_EXPORT ScriptParser
{
public:
  ScriptParser();
  ~ScriptParser();
  /// Parse the script in a string
  void parse(const std::string& str);
private:
  /// Parses syntax constructs: loops, ifs, etc.
  SyntaxParser m_syntaxParser;
  /// Expression parser
  Kernel::EParser m_expressionParser;
  /// Chlid script units: lines of a block, control elements of a loop, etc.
  std::vector<ScriptParser*> m_lines;
  /// Type of the parsed unit: expression, if, for, function definition, etc.
  std::string m_type;
};

} // FORMULA

#endif // FORMULA_SCRIPTPARSER_H
