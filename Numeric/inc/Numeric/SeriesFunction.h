#ifndef NUMERIC_SERIESFUNCTION_H
#define NUMERIC_SERIESFUNCTION_H

#include "Numeric/DllExport.h"
#include "Numeric/IFunction.h"

namespace Numeric
{

class NUMERIC_EXPORT SeriesFunction: public virtual IFunction
{
public:
  SeriesFunction();
  ~SeriesFunction(){}
  ParamFunction();
  ~ParamFunction(){}
  /// Get i-th parameter
  virtual double getParameter(size_t i)const;
  /// Get parameter by name.
  virtual double getParameter(const std::string& name)const;
  /// Set i-th parameter
  virtual void setParameter(size_t, const double& value);
  /// Set parameter by name.
  virtual void setParameter(const std::string& name, const double& value);
  /// Total number of parameters
  virtual size_t nParams()const;
  /// Returns the index of parameter name
  virtual size_t parameterIndex(const std::string& name)const;
  /// Returns the name of parameter i
  virtual std::string parameterName(size_t i)const;

  /// Number of active (in terms of fitting) parameters
  virtual size_t nActive()const;
  /// Value of i-th active parameter. Override this method to make fitted parameters different from the declared
  virtual double activeParameter(size_t i)const;
  /// Set new value of i-th active parameter. Override this method to make fitted parameters different from the declared
  virtual void setActiveParameter(size_t i, double value);
  /// Returns the name of active parameter i
  virtual std::string nameOfActive(size_t i)const;

  /// Check if declared parameter i is fixed
  virtual bool isFixed(size_t i)const;
  /// Fixes declared parameter i
  virtual void fix(size_t i);
  /// Restores declared parameter i for fitting
  virtual void unfix(size_t i);

  /// Returns the number of attributes associated with the function
  virtual size_t nAttributes()const{return 1;}
  /// Returns a list of attribute names
  virtual std::vector<std::string> getAttributeNames()const;
  /// Return a value of attribute attName
  virtual Attribute getAttribute(const std::string& attName)const;
  /// Set a value to attribute attName
  virtual void setAttribute(const std::string& attName,const Attribute& );
  /// Check if attribute attName exists
  virtual bool hasAttribute(const std::string& attName)const;

protected:
  std::vector<double> m_parameters;
  int m_order;
};

} // namespace Numeric
#endif // NUMERIC_SERIESFUNCTION_H
