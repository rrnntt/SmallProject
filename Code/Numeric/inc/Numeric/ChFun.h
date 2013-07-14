#ifndef NUMERIC_CHFUN_H
#define NUMERIC_CHFUN_H

#include "Numeric/DllExport.h"
#include "Numeric/Chebfun.h"
#include "Numeric/IFunction1D.h"
#include "Numeric/TiesFunction.h"

namespace Numeric
{

class NUMERIC_EXPORT ChFun: public chebfun, public IFunction1D, public TiesFunction
{
public:
  ChFun(size_t n = 0, const double& startX = -1,const double& endX = 1);
  ChFun(const ChFun& fun);
  ChFun& operator=(const ChFun& fun);
  ~ChFun();
  void function1D(double* out, const double* xValues, const size_t nData)const;
  void set(size_t n,const double& startX = -1,const double& endX = 1);

  /// Set i-th parameter
  virtual void setParameter(size_t, const double& value, bool explicitlySet = true);
  /// Set i-th parameter description
  virtual void setParameterDescription(size_t, const std::string& description);
  /// Get i-th parameter
  virtual double getParameter(size_t i)const;
  /// Set parameter by name.
  virtual void setParameter(const std::string& name, const double& value, bool explicitlySet = true);
  /// Set description of parameter by name.
  virtual void setParameterDescription(const std::string& name, const std::string& description);
  /// Get parameter by name.
  virtual double getParameter(const std::string& name)const;
  /// Total number of parameters
  virtual size_t nParams()const{return m_parameters.size();}
  /// Returns the index of parameter name
  virtual size_t parameterIndex(const std::string& name)const;
  /// Returns the name of parameter i
  virtual std::string parameterName(size_t i)const;
  /// Returns the description of parameter i
  virtual std::string parameterDescription(size_t i)const;
  /// Checks if a parameter has been set explicitly
  virtual bool isExplicitlySet(size_t i)const;
  /// Get the fitting error for a parameter
  virtual double getError(size_t i) const;
  /// Set the fitting error for a parameter
  virtual void setError(size_t i, double err);

  /// Check if a declared parameter i is active
  virtual bool isFixed(size_t i)const;
  /// Removes a declared parameter i from the list of active
  virtual void fix(size_t i);
  /// Restores a declared parameter i to the active status
  virtual void unfix(size_t i);

  /// Return parameter index from a parameter reference. Usefull for constraints and ties in composite functions
  virtual size_t getParameterIndex(const ParameterReference& ref)const;
  /// Get the containing function
  IFunction_sptr getContainingFunction(const ParameterReference& ref)const;
  /// Get the containing function
  IFunction_sptr getContainingFunction(IFunction_sptr fun);

private:
  /// The index map. m_indexMap[i] gives the total index for active parameter i
  std::vector<bool> m_isFixed;
  /// Keeps parameter errors
  std::vector<double> m_errors;
  /// Flags of explicitly set parameters
  std::vector<bool> m_explicitlySet;
};

} // Numeric

#endif // NUMERIC_CHFUN_H
