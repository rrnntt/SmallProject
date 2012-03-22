#ifndef NUMERIC_FUNCTIONVALUES_H_
#define NUMERIC_FUNCTIONVALUES_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/DllExport.h"
#include "Numeric/FunctionDomain.h"

#include <vector>

namespace Numeric
{
/** Base class that represents the domain of a function.
    A domain is a generalisation of x (argument) and y (value) arrays.
    A domain consists at least of a list of function arguments for which a function should 
    be evaluated and a buffer for the calculated values. If used in fitting also contains
    the fit data and weights.

*/
class NUMERIC_EXPORT FunctionValues
{
public:
  /// Default constructor.
  FunctionValues(){}
  /// Constructor.
  FunctionValues(const FunctionDomain& domain);
  /// Copy constructor.
  FunctionValues(const FunctionValues& values);
  /// Reset the values to match a new domain.
  void reset(const FunctionDomain& domain);
  /// Return the number of values
  size_t size() const {return m_calculated.size();}
  /// store i-th calculated value. 0 <= i < size()
  void setCalculated(size_t i,double value) {m_calculated[i] = value;}
  /// set all calculated values to same number
  void setCalculated(double value);
  /// get i-th calculated value. 0 <= i < size()
  double getCalculated(size_t i) const {return m_calculated[i];}
  double operator[](size_t i) const {return m_calculated[i];}
  void addToCalculated(size_t i, double value) {m_calculated[i] += value;}
  void addToCalculated(size_t i, const FunctionValues& values);
  /// Get a pointer to calculated data at index i
  double* getPointerToCalculated(size_t i);
  /// Add other calculated values
  FunctionValues& operator+=(const FunctionValues& values);
  /// Multiply by other calculated values
  FunctionValues& operator*=(const FunctionValues& values);
  /// Set all calculated values to zero
  void zeroCalculated();

  /// set a fitting data value
  void setFitData(size_t i,double value);
  void setFitData(const std::vector<double>& values);
  /// get a fitting data value
  double getFitData(size_t i) const;
  /// set a fitting weight
  void setFitWeight(size_t i,double value);
  void setFitWeights(const std::vector<double>& values);
  void setFitWeights(const double& value);
 /// get a fitting weight
  double getFitWeight(size_t i) const;
  void setFitDataFromCalculated(const FunctionValues& values);
protected:
  std::vector<double> m_calculated; ///< buffer for calculated values
  std::vector<double> m_data;    ///< buffer for fit data
  std::vector<double> m_weights; ///< buffer for fitting weights (reciprocal errors)
};

/// typedef for a shared pointer
typedef boost::shared_ptr<FunctionValues> FunctionValues_ptr;

} // namespace Numeric

#endif /*MANTID_API_FUNCTIONVALUES_H_*/
