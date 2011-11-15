#ifndef FORMULA_PARSER_H
#define FORMULA_PARSER_H

#include "Formula/DllExport.h"
#include "Formula/Operators.h"

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <map>
#include <set>

namespace Formula
{

    /** 
    
    This class represents an expression made up of names, binary operators and brackets.
    The input for an Expression is a text string. If an Expression is a function (sum, product,
    sine, etc) it has arguments. Each argument is an Expression itself. So Expression is a tree structure
    with functions in its nodes and the branches are the arguments.

    */
    class FORMULA_EXPORT Parser
    {
    public:
      /// Default contructor
      Parser();
      /// contructor
      Parser(const std::vector<std::string>& ops);
      /// contructor
      Parser(const std::vector<std::string>& binary,const std::set<std::string>& unary);
      /// constructor
      Parser(Operators_ptr operators);
      /// copy contructor
      Parser(const Parser& expr);
      /// Assignment operator
      Parser& operator=(const Parser& expr);
      /**
       * Parse a string and create an expression.
       * @param str The input string.
       */
      void parse(const std::string& str);
      /**
       * Print the expression into std::cerr to show its structure
       * @param pads Padding to make indentation
       */
      void logPrint(const std::string& pads = "")const;
      /// Returns this expression as a string. It does not simply returns the input string 
      /// but recreates it.
      std::string str(bool printOp = false)const;
      /// Returns true if the expression is a function (i.e. has arguments)
      bool isFunct()const{return m_terms.size() > 0;}
      /// Returns the name of the expression which is a function or variable name.
      std::string name()const{return m_funct;}
      /// Returns the the expression's binary operator on its left. Can be an empty string.
      std::string operator_name()const{return m_op;}
      /// Returns the top level terms of the expression (function arguments). For a variable
      /// it empty.
      const std::vector<Parser>& terms()const{return m_terms;}
      /// Returns the number of argumens
      size_t size()const{return m_terms.size();}
      typedef std::vector<Parser>::const_iterator iterator;
      iterator begin()const{return m_terms.begin();}
      iterator end()const{return m_terms.end();}
      const Parser& operator[](int i)const{return m_terms.at(i);}
      Operators_ptr operators()const{return m_operators;}
      /// Makes a key=value map of this parser (after parsing).
      /// The terms must contain one or more key=value pairs (comma separated)
      void toMap();
      void toList();
    private:

      /// copy contructor
      Parser(const Parser* pexpr);
      /** 
       * This is a struct to mark a token in a string expression.
       * Tokens in an expressions are separated by operators.
       * A token is either a symbolic name (not containing operators and empty spaces)
       * or another expression.
       */
      struct Token
      {
        /**
         * Constructor.
         * @param i The index of the first symbol of the token.
         * @param j The index of the last symbol of the token.
         * @param k The index of the first symbol of the next token. The substring
         *    between j and k contains the operator connecting the next token to this one.
         * @param p The precedence of the connecting operator.
         */
        Token(unsigned int i,unsigned int j,unsigned int k,size_t p)
          :is(i),ie(j),is1(k),prec(p){}
        /**
         * The copy constructor.
         */
        Token(const Token& t)
          :is(t.is),ie(t.ie),is1(t.is1),prec(t.prec){}
        unsigned int is;  ///< The index of the first symbol of the token.
        unsigned int ie;  ///< The index of the last symbol of the token.
        unsigned int is1; ///< The index of the first symbol of the next token.
        size_t prec;      ///< The precedence of the connecting operator.
      };
      /// The container type
      typedef std::vector< Token > Tokens;
      /// Get i-th token
      std::string GetToken(size_t i);
      /// Get the operator connecting i-th token
      std::string GetOp(size_t i);
      /// Analyze the string in m_expr and find all top level tokens.
      /// Result: filled in m_tokens structure.
      void tokenize();
      /**
       * Set the function name of this expression. It is also called when
       * name cannot be split into tokens. 
       * @param name The name of the function
       */
      void setFunct(const std::string& name);
      /** 
       * Remove leading and ending empty spaces from a string
       * @param str The string.
       */
      static void trim(std::string& str);

      Tokens m_tokens;     ///< The container for the token markers
      std::string m_expr;  ///< Saved expression string

      std::string m_funct; ///< Function name
      std::string m_op;    ///< Operator connecting this expression to its sibling on the left
      std::vector<Parser> m_terms;///< Child expressions (function arguments)

      Operators_ptr m_operators;

    };

}
#endif /* FORMULA_PARSER_H*/
