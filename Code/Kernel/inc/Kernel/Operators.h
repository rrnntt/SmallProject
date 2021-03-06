#ifndef KERNEL_OPERATORS_H
#define KERNEL_OPERATORS_H

#include "Kernel/DllExport.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <set>
#include <map>

/**
 * Keeps operator that can be used in an expression
 */
class KERNEL_EXPORT Operators
{
public:
  /**
   * Returns the precedence of operator op. Precedence starts with 1
   * and 0 means op is not a binary operator
   * @param op The operator
   * @return The precedence
   */
  size_t op_prec(const std::string& op)const;
  /**
   * Adds new binary operators to the expression
   * @param ops A vector with operators in reverse order of precedence
   */
  void add_operators(const std::vector<std::string>& ops);
  /**
   * Adds new unary operators to the expression
   * @param ops A vector with unary operators
   */
  void add_unary(const std::set<std::string>& ops);
  /**
   * Check if a string is a unary operator
   * @param op The string to check
   * @return True if the argument is a unary operator
   */
  bool is_unary(const std::string& op)const;
  /**
   * Check if a character is a part of an operator
   * @param c The character to check
   * @return True if it is true
   */
  bool is_op_symbol(const char c)const;
  bool is_un_symbol(const char c)const;
  std::string getBinSymbols()const;
  std::string getUnSymbols()const;

  /**
    * return main binary operator name for precedence prec
    */
  const std::string& binary(size_t prec)const;

  size_t precedence(const std::string& op){return m_precedence[op];}

  size_t maxPrecedence()const{return m_binary.size() + 1;}

  std::vector<std::string> getAllBinary()const;

  /**
    * Get name of the main binary operator: for "+" and "-" it will be "+"
    */
  std::string what_is_binary_name(const std::string& op)const;
  std::string what_is_binary_name(size_t prec)const;

private:
  std::vector<std::string> m_binary;          ///< Binary operators in reverse precedence order
  std::set<std::string>    m_unary;           ///< Unary operators
  std::map<const std::string, size_t> m_precedence; ///< Map of the operator precedence order
  std::set<char> m_bin_symbols;                   ///< All the symbols that are used in the binary operators
  std::set<char> m_un_symbols;                   ///< All the symbols that are used in the unary operators
  std::map<std::string, char> m_op_number;    ///<
};

typedef boost::shared_ptr<Operators> Operators_ptr;

#endif // KERNEL_OPERATORS_H
