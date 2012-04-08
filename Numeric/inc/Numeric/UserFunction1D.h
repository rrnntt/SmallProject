#ifndef NUMERIC_USERFUNCTION1D_H
#define NUMERIC_USERFUNCTION1D_H

#include "Numeric/DllExport.h"
#include "Numeric/IFunction1D.h"
#include "Numeric/ParamFunction.h"

namespace Formula
{
  class Namespace;
  class Expression;
}

namespace Numeric
{

class NUMERIC_EXPORT UserFunction1D: public IFunction1D, public ParamFunction
{
public:
  /// Returns the function's name
  virtual std::string name()const {return "UserFunction1D";}
  /// Function you want to fit to.
  virtual void function1D(double* out, const double* xValues, const size_t nData)const;
  /// Returns the number of attributes associated with the function
  virtual size_t nAttributes()const{return 1;}
  /// Returns a list of attribute names
  virtual std::vector<std::string> getAttributeNames()const{return std::vector<std::string>(1,"Formula");}
  /// Return a value of attribute attName
  virtual Attribute getAttribute(const std::string& attName)const;
  /// Set a value to attribute attName
  virtual void setAttribute(const std::string& attName,const Attribute& );
  /// Check if attribute attName exists
  virtual bool hasAttribute(const std::string& attName)const { return attName == "Formula";}
protected:
  /// Parse the formula and create the expression
  void setFormula();
  /// store the formula
  std::string m_formula;
  /// namespace
  boost::shared_ptr<Formula::Namespace> m_namespace;
  boost::shared_ptr<Formula::Expression> m_expression;
  /// the x variable
  mutable double m_x;
};

} // namespace Numeric
#endif // NUMERIC_USERFUNCTION1D_H
