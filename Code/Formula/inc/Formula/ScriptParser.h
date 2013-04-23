#ifndef FORMULA_SCRIPTPARSER_H
#define FORMULA_SCRIPTPARSER_H

#include "Formula/DllExport.h"
#include "Kernel/EParser.h"

#include <vector>

namespace Formula
{

//---------------------------------------------------------
//  Forward declarations
//---------------------------------------------------------
class IfParser;
class VarParser;

/**
 *  Parses syntax constructs: loops, ifs, etc.
 * It inherits from Kernel::SeqParser and has two members:
 *  the first is a AltParser with the contents of a script line,
 *  the second is a character ';' ending the line.
 */
class FORMULA_EXPORT SyntaxParser: public Kernel::SeqParser
{
public:
  SyntaxParser();
  SyntaxParser(const SyntaxParser&);
  Kernel::IParser* clone() const;
  void log(const std::string& padding = "") const 
  {
    std::cerr << padding << "SyntaxParser" << std::endl;
    logChildren(padding);
  }
  Kernel::BracketParser* block() const;
  Kernel::IParser* expression() const;
  IfParser* ifBlock() const;
  VarParser* defVar() const;
private:
  /// initialize the instance
  void init();
  /// Finds curly brackets
  Kernel::BracketParser* m_blockParser;
  /// Collects all if other parsers failed - it must be an expression
  Kernel::IParser* m_expression;
  IfParser* m_ifParser;
  VarParser* m_varParser;
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
  /// Return number of lines
  size_t size() const {return m_lines.size();}
  /// Return i-th line's syntax parser
  SyntaxParser* line(size_t i) const {return m_lines[i];}
private:
  /// Find lines separated by semi-colons ';'
  Kernel::ListParser m_linesParser;
  /// Single-line script units: 
  std::vector<SyntaxParser*> m_lines;
};

/**
 * Parsers the if contructs
 */
class FORMULA_EXPORT IfParser: public Kernel::SeqParser
{
public:
  IfParser();
  Kernel::BracketParser* condition() const;
  Kernel::BracketParser* trueBlock() const;
  Kernel::BracketParser* elseBlock() const;
private:
  Kernel::BracketParser* m_conditionBlock;
  Kernel::BracketParser* m_trueBlock;
  Kernel::BracketParser* m_elseBlock;
};

/**
 * Parsers the vriable definition
 */
class FORMULA_EXPORT VarParser: public Kernel::SeqParser
{
public:
  VarParser();
  Kernel::IParser* type() const;
  Kernel::IParser* var() const;
  Kernel::IParser* value() const;
private:
  Kernel::VarNameParser* m_type;
  Kernel::VarNameParser* m_var;
  Kernel::IParser* m_value;
};

} // FORMULA

#endif // FORMULA_SCRIPTPARSER_H
