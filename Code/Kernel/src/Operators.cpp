#include "Kernel/Operators.h"
#include "Kernel/Str.h"

#include <stdexcept>

void Operators::add_operators(const std::vector<std::string>& ops)
{
  m_binary = ops;
  // Fill in the precedence table (m_op_precedence)
  for(size_t i=0;i<m_binary.size();i++)
  {
    char j = 0;
    std::vector<std::string> vops = Str::split(m_binary[i],' ');
    for(std::vector<std::string>::iterator it = vops.begin();it != vops.end(); it++ )
    {
      m_precedence[*it] = i + 1;
      m_op_number[*it] = j++;
    }
  }

  for(size_t i=0;i<ops.size();i++)
  {
    std::string str = ops[i];
    for(size_t j=0;j<str.size();j++)
    {
      char c = str[j];
      if ( c == ' ' ) continue;
      m_bin_symbols.insert(c);
    }
  }

}

void Operators::add_unary(const std::set<std::string>& ops)
{
  m_unary = ops;
  for(std::set<std::string>::const_iterator it=ops.begin();it!=ops.end();it++)
  {
    for(std::string::const_iterator c=it->begin();c!=it->end();c++)
    {
      m_un_symbols.insert(*c);
    }
  }
}

size_t Operators::op_prec(const std::string& op)const
{
  std::map<const std::string, size_t>::const_iterator i = m_precedence.find(op);
  if (i == m_precedence.end()) return 0;
  return i->second;
}

bool Operators::is_unary(const std::string& op)const
{
  return m_unary.count(op) != 0;
}

bool Operators::is_op_symbol(const char c)const
{
  return m_bin_symbols.count(c) != 0;
}

bool Operators::is_un_symbol(const char c)const
{
  return m_un_symbols.count(c) != 0;
}

const std::string& Operators::binary(size_t prec)const
{
  size_t i = prec-1;
  if (i < m_binary.size())
  {
    return m_binary[i];
  }
  throw std::out_of_range("Index to Operators::binary is out of range");
}

std::string Operators::what_is_binary_name(const std::string& op)const
{
  size_t prec = op_prec(op);
  if (prec > 0)
  {
    return Str::split(binary(prec),' ')[0];
  }
  return "";
}

std::string Operators::what_is_binary_name(size_t prec)const
{
  if (prec > 0)
  {
    return Str::split(binary(prec),' ')[0];
  }
  return "";
}

std::string Operators::getBinSymbols()const
{
  return std::string(m_bin_symbols.begin(),m_bin_symbols.end());
}

std::string Operators::getUnSymbols()const
{
  return std::string(m_un_symbols.begin(),m_un_symbols.end());
}

std::vector<std::string> Operators::getAllBinary()const
{
  std::vector<std::string> res;
  for(size_t i=0;i<m_binary.size();i++)
  {
    std::vector<std::string> vops = Str::split(m_binary[i],' ');
    res.insert(res.end(),vops.begin(),vops.end());
  }
  return res;
}