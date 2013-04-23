#include "Formula/Parser.h"
#include "Kernel/Str.h"

#include <sstream>
#include <iostream>
#include <stdexcept>
#include <cctype>

namespace Formula
{

Parser::Parser()
{

  m_operators.reset(new Operators());
  // Define binary operators. Put them in the reverse precedence order (from lower to higher prec.)
  std::vector<std::string> ops;
  ops.push_back(";");
  ops.push_back(",");
  ops.push_back(":");
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

/// contructor
Parser::Parser(const std::vector<std::string>& ops)
{
  m_operators.reset(new Operators());
  m_operators->add_operators(ops);
}

/// contructor
Parser::Parser(const std::vector<std::string>& binary,const std::set<std::string>& unary)
{
  m_operators.reset(new Operators());
  m_operators->add_operators(binary);
  m_operators->add_unary(unary);
}

Parser::Parser(Operators_ptr operators)
{
  m_operators = operators;
}

Parser::Parser(const Parser& expr)
:
//m_tokens(expr.m_tokens),
//m_expr(expr.m_expr),
m_funct(expr.m_funct),
m_op(expr.m_op),
m_terms(expr.m_terms),
m_operators(expr.m_operators)
{
}
Parser::Parser(const Parser* pexpr)
:m_operators(pexpr->m_operators)
{
}

/// Assignment operator
Parser& Parser::operator=(const Parser& expr)
{
  m_operators = expr.m_operators;
  m_funct = expr.m_funct;
  m_op = expr.m_op;
  m_terms = expr.m_terms;
  //m_expr = expr.m_expr;
  //m_tokens = expr.m_tokens;
  return *this;
}

void Parser::trim(std::string& str)
{
  size_t i = str.find_first_not_of(" \t\n\r");
  size_t j = str.find_last_not_of(" \t\n\r");
  if (i == std::string::npos || j == std::string::npos || j < i)
  {
    str = "";
  }
  else
  {
    str = str.substr(i,j-i+1);
  }
}

void Parser::parse(const std::string &str)
{
  m_expr = str;
  trim(m_expr);

  if (m_expr.size() > 1 && m_expr[0] == '(' && m_expr[m_expr.size()-1] == ')')
  {
    if (m_expr.find('(',1) == std::string::npos)
    {
      m_expr.erase(0,1);
      m_expr.erase(m_expr.size()-1,1);
      trim(m_expr);
    }
  }

  tokenize();

  if (m_tokens.size() == 0)
  {
    setFunct(m_expr);
    return;
  }

  std::string op = GetOp(0);
  size_t prec = m_operators->op_prec(op);
  std::vector<std::string> vops = Str::split(m_operators->binary(prec),' ');

  setFunct(*vops.begin());

  for(size_t i=0;i<=m_tokens.size();i++)
  {
    m_terms.push_back(Parser(this));
    Parser& t = m_terms.back();
    if (i)
      t.m_op = GetOp(i-1);
    t.parse(GetToken(i));
  }
  m_expr = "";
  m_tokens.clear();
}

void Parser::tokenize()
{
  m_tokens.clear();

  unsigned int min_prec = 1000;
  size_t is = 0;
  size_t is1 = 0;
  unsigned int lvl = 0;
  size_t last = m_expr.size() - 1;
  bool inString = false;
  int skip = 0;
  bool canBeBinary = false;
  bool isNumber = false; // if parser is inside a number (important case is 123.45e+67)
  bool canDotBeAdded = false;
  bool canEBeAdded = false;
  bool canPlusBeAdded = false;
  Tokens tokens;
  for(size_t i=0;i<m_expr.size();i++)
  {
    char c = m_expr[i];
    bool isOpSymbol = m_operators->is_op_symbol(c);


    if (!inString && skip == 0)
    {
      if (isNumber)
      {
        if (c == '.')
        {
          if (canDotBeAdded)
          {
            canDotBeAdded = false;
          }
          else
          {
            isNumber = false;
          }
        }
        else if (c == 'e' || c == 'E')
        {
          if (canEBeAdded)
          {
            canEBeAdded = false;
            canDotBeAdded = false;
            canPlusBeAdded = true;
          }
          else
          {
            isNumber = false;
          }
        }
        else if (c == '+' || c == '-')
        {
          if (canPlusBeAdded)
          {
            canPlusBeAdded = false;
            canEBeAdded = false;
            canDotBeAdded = false;
          }
          else
          {
            isNumber = false;
          }
        }
        else if (!isdigit(c))
        {
          isNumber = false;
        }
      }
      else if (isdigit(c))
      {
        isNumber = true;
        canDotBeAdded = true;
        canEBeAdded = true;
        canPlusBeAdded = false;
      }

      if (lvl == 0 && !isNumber && isOpSymbol)// insert new token
      {
        if (i == last) 
        {
          break;
          //throw std::runtime_error("Parser: syntax error");
        }

        if (m_operators->is_op_symbol(m_expr[i+1]))
        {
          is1 = i + 2;
        }
        else
        {
          is1 = i + 1;
        }

        if (is1 > last) 
        {
          throw std::runtime_error("Parser: syntax error");
        }

        std::string op = m_expr.substr(i,is1-i);
        size_t prec = canBeBinary? m_operators->precedence(op) : 0;
        if (!prec) // operator does not exist
        {
          std::ostringstream mess;
          bool error = true;
          // check if it's a binary and a unary operators together
          if (op.size() == 2)
          {
            if (m_operators->is_unary(op))
            {
              is1 -= 2;
              skip = 2;
              prec = min_prec + 1; // do not add token
              error = false;
            }
            else
            {
              is1 -= 1;
              std::string uop = op.substr(1,1);
              op = op[0];
              if (m_operators->is_op_symbol(m_expr[is1+1]))
              {
                uop += m_expr[is1+1];
                if (is1 + 2 > last) 
                {
                  mess << "Parser: syntax error at " << is1+1;
                  throw std::runtime_error(mess.str());
                }
              }
              if (m_operators->is_unary(uop))
              {
                prec = m_operators->precedence(op);
                if (prec)
                {// we don't want to create a new token with unary operator. it is processed in SetFunct()
                  skip = 1;
                  error = false;
                }
              }
            }
          }// op.size == 2
          else if (op.size() == 1)
          {
            //skip = 1;
            prec = min_prec + 1; // do not add token
            error = false;
          }
          if (error)
          {
            mess << "Parser: unrecognized operator " << op;
            throw std::runtime_error(mess.str());
          }
        }

        if (prec <= min_prec)
        {
          if (prec < min_prec) min_prec = prec;
          Token tok(is,i-1,is1,prec);
          tokens.push_back(tok);
          is = is1;
        }

        i = is1 - 1;

        canBeBinary = false;

      }// insert new token
      else if (c != ' ' && c != '\t' && c != '\r' && c != '\n')
      {
        canBeBinary = true;
      }

      if ( c == '(') lvl++;
      if ( c == ')')
      {
        if (lvl) lvl--;
        else
        {
          throw std::runtime_error("Unmatched brackets");
        }
      }
    } // !inString || skip
    else if (skip > 0)
    {
      skip --;
    }

    if (c == '"')
    {
      if ( !inString )
      {
        inString = true;
      }
      else
      {
        inString = false;
      }
    }


  } // for i

  if (tokens.size())
  {
    // remove operators of higher prec
    m_tokens.push_back(Token(tokens[0]));
    for(size_t i=0;i<tokens.size();i++)
    {
      Token& tok = tokens[i];
      std::string op = m_expr.substr(tok.ie+1,tok.is1-tok.ie-1);//?
      if (m_operators->precedence(op) == min_prec)
      {
        Token& last_tok = m_tokens.back();
        last_tok.ie = tok.ie;
        last_tok.is1= tok.is1;
        if (i != tokens.size()-1)
          m_tokens.push_back(Token(tokens[i+1]));
      }
    }
  }


}

std::string Parser::GetToken(size_t i)
{
  if (m_tokens.size() == 0) return m_expr;

  if (i < m_tokens.size())
  {
    Token& tok = m_tokens[i];
    return m_expr.substr(tok.is,tok.ie-tok.is+1);
  }

  if (i == m_tokens.size())
  {
    Token& tok = m_tokens[i-1];
    return m_expr.substr(tok.is1);
  }

  return "";

}

std::string Parser::GetOp(size_t i)
{
  if (m_tokens.size() == 0 || i >= m_tokens.size()) return "";

  Token& tok = m_tokens[i];
  return m_expr.substr(tok.ie+1,tok.is1-tok.ie-1);
}

void Parser::logPrint(const std::string& pads)const
{
  std::string myPads = pads + "   ";
  if (m_terms.size())
  {
    std::cerr<<myPads<<m_op<<'['<<m_funct<<']'<<"("<<'\n';
    for(size_t i=0;i<m_terms.size();i++)
      m_terms[i].logPrint(myPads);
    std::cerr<<myPads<<")"<<'\n';
  }
  else
    std::cerr<<myPads<<m_op<<m_funct<<'\n';
}

void Parser::setFunct(const std::string& name)
{
  if ( !m_operators->op_prec(name))
  {
    std::string op = "";
    if (name.size() > 1 && m_operators->is_op_symbol(name[0]))
    {
      op = name.substr(0,1);
      if (name.size() > 2 && m_operators->is_op_symbol(name[1]))
      {
        op += name[1];
      }
    }
    if (! op.empty() && m_operators->is_unary(op) )
    {
      m_funct = op;
      Parser tmp(this);
      tmp.parse(name.substr(op.size()));
      m_terms.push_back(tmp);
      return;
    }
  }

  m_funct = name;
  trim(m_funct);
  if (m_funct.empty())
  {
    throw std::runtime_error("Parser: Syntax error");
  }

  // Check if the function has arguments
  std::string::size_type i = std::string::npos;

  bool inQuotes = false;
  for(std::string::const_iterator c = name.begin(); c != name.end(); c++)
  {
    if (*c == '"')
    {
      if (inQuotes) inQuotes = false;
      else
        inQuotes = true;
      continue;
    }

    if ( !inQuotes && *c == '(')
    {
      i = c - name.begin();
      break;
    }
  }

  if (i != std::string::npos)
  {
    std::string::size_type j = name.find_last_of(')');
    if (j == std::string::npos || j < i)
    {
      throw std::runtime_error("Unmatched brackets");
    }

    if (j > i + 1)// nonzero argument list
    {
      std::string args = name.substr(i+1,j-i-1);//?
      trim(args);
      std::string f = name.substr(0,i);
      trim(f);
      Parser tmp(this);
      tmp.parse(args);
      if ( !tmp.isFunct() || tmp.name() != ",")
      {
        m_terms.push_back(tmp);
      }
      else
      {
        std::string my_op = m_op;
        *this = tmp;
        m_op = my_op;
      }
      m_funct = f;
    }

  }
}

std::string Parser::str(bool printOp)const
{
  std::ostringstream res;
  if (printOp)
  {
    res << m_op.c_str();
  }
  size_t prec = m_operators->op_prec(m_funct);
  if (!prec || size() == 1)
  {
    res << m_funct.c_str();
    prec = 1000;
  }

  if (m_terms.size())
  {
    bool bk = prec > m_operators->op_prec(m_terms[0].m_funct);
    if (bk) res << '(' ;
    for(size_t i=0;i<m_terms.size();i++)
    {
      res << m_terms[i].str(true).c_str();
    }
    if (bk) res <<')';
  }
  return res.str();
}

void Parser::toMap()
{
  if (name() == ",")
  {
    m_funct = "map";
    for(iterator it = begin(); it != end(); ++it)
    {
      if (it->name() != "=" || it->size() != 2)
      {
        std::runtime_error("Expression "+str()+" cannot be changed to a map");
      }
    }
  }
  else if (name() == "=" && size() == 2)
  {
    m_op = ",";
    Parser tmp(*this);
    m_terms.clear();
    m_terms.push_back(tmp);
    m_funct = "map";
    m_op = "";
  }
  else
  {
    std::runtime_error("Expression "+str()+" cannot be changed to a map");
  }
}

void Parser::toList()
{
  if (name() == ",")
  {
    m_funct = "list";
  }
  else
  {
    m_op = ",";
    Parser tmp(*this);
    m_terms.clear();
    m_terms.push_back(tmp);
    m_funct = "list";
    m_op = "";
  }
}

} // Formula
