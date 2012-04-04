#ifndef NUMERIC_FUNCTIONVALUES_H_
#define NUMERIC_FUNCTIONVALUES_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/DllExport.h"
#include "Numeric/FunctionDomain.h"
#include "Numeric/IFunctionValues.h"

#include <vector>

namespace Numeric
{
/** Base class that represents the domain of a function.
    A domain is a generalisation of x (argument) and y (value) arrays.
    A domain consists at least of a list of function arguments for which a function should 
    be evaluated and a buffer for the calculated values. If used in fitting also contains
    the fit data and weights.

*/
class NUMERIC_EXPORT FunctionValues: public IFunctionValues
{
public:
  /// Default constructor.
  FunctionValues(){}
  /// Constructor.
  FunctionValues(const FunctionDomain& domain);
  /// Copy constructor.
  FunctionValues(const FunctionValues& values);
  
  /// Return the number of values
  size_t size() const {return m_calculated.size();}
  /// Expand values to a new size, preserve stored values.
  void expand(size_t n);

  /// Get a pointer to calculated data at index i
  double* getPointerToCalculated(size_t i);
  /// Set all calculated values to zero
  void zeroCalculated();
  /// set all calculated values to same number
  void setCalculated(double value);

  /// Reset the values to match a new domain.
  void reset(const FunctionDomain& domain);
  /// store i-th calculated value. 0 <= i < size()
  void setCalculated(size_t i,double value) {m_calculated[i] = value;}
  /// get i-th calculated value. 0 <= i < size()
  double getCalculated(size_t i) const {return m_calculated[i];}
  double operator[](size_t i) const {return m_calculated[i];}
  void addToCalculated(size_t i, double value) {m_calculated[i] += value;}

  /// Add other calculated values
  FunctionValues& operator+=(const FunctionValues& values)
  {
    if (size() != values.size())
    {
      throw std::runtime_error("Cannot add values: sizes do not match");
    }
    values.add(getPointerToCalculated(0));
    return *this;
  }
  /// Multiply by other calculated values
  FunctionValues& operator*=(const FunctionValues& values)
  {
    if (size() != values.size())
    {
      throw std::runtime_error("Cannot multiply values: sizes do not match");
    }
    values.multiply(getPointerToCalculated(0));
    return *this;
  }
  /// Add other calculated values to these values starting with i.
  void addToCalculated(size_t i, const FunctionValues& values)
  {
    if (i + size() < values.size())
    {
      throw std::runtime_error("Cannot add values: sizes do not match");
    }
    values.add(getPointerToCalculated(i));
  }

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
  /// Copy calculated values to a buffer
  /// @param to :: Pointer to the buffer
  void copyTo(double* to) const;
  /// Add calculated values to values in a buffer and save result to the buffer
  /// @param to :: Pointer to the buffer, it must be large enough
  void add(double* to) const;
  /// Multiply calculated values by values in a buffer and save result to the buffer
  /// @param to :: Pointer to the buffer, it must be large enough
  void multiply(double* to) const;

  std::vector<double> m_calculated; ///< buffer for calculated values
  std::vector<double> m_data;    ///< buffer for fit data
  std::vector<double> m_weights; ///< buffer for fitting weights (reciprocal errors)
};

/// typedef for a shared pointer
typedef boost::shared_ptr<FunctionValues> FunctionValues_ptr;

} // namespace Numeric

#endif /*MANTID_API_FUNCTIONVALUES_H_*/
