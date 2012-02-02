#ifndef NUMERIC_FUNCTIONDOMAIN_H
#define NUMERIC_FUNCTIONDOMAIN_H

#include "Numeric/DllExport.h"

#include <vector>

namespace Numeric
{

/** Base class that represents the domain of a function.
    A domain is a generalisation of x (argument) and y (value) arrays.
    A domain consists at least of a list of function arguments for which a function should 
    be evaluated and a buffer for the calculated values. If used in fitting also contains
    the fit data and weights.
 */
class NUMERIC_EXPORT FunctionDomain
{
public:
  /// Constructor.
  FunctionDomain(size_t n);
  /// Virtual destructor
  virtual ~FunctionDomain(){}
  /// Return the number of points, values, etc in the domain
  virtual size_t size() const {return m_calculated.size();}
  /// store i-th calculated value. 0 <= i < size()
  virtual void setCalculated(size_t i,double value) {m_calculated[i] = value;}
  /// get i-th calculated value. 0 <= i < size()
  virtual double getCalculated(size_t i) const {return m_calculated[i];}
  /// set a fitting data value
  virtual void setFitData(size_t i,double value);
  virtual void setFitData(const std::vector<double>& values);
  /// get a fitting data value
  virtual double getFitData(size_t i) const;
  /// set a fitting weight
  virtual void setFitWeight(size_t i,double value);
  virtual void setFitWeights(const std::vector<double>& values);
  /// get a fitting weight
  virtual double getFitWeight(size_t i) const;
protected:
  void setDataSize();
  std::vector<double> m_calculated; ///< buffer for calculated values
  std::vector<double> m_data;    ///< buffer for fit data
  std::vector<double> m_weights; ///< buffer for fitting weights (reciprocal errors)
};

} // namespace Numeric
#endif // NUMERIC_FUNCTIONDOMAIN_H
