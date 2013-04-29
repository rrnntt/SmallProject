#ifndef NUMERIC_GSLVECTOR_H
#define NUMERIC_GSLVECTOR_H

#include "Numeric/DllExport.h"

#include <gsl/gsl_vector.h>

#include <vector>
#include <stdexcept>
#include <iostream>
#include <iomanip>

namespace Numeric
{

/**
  @brief A general purpose vector of doubles.
 */
class NUMERIC_EXPORT GSLVector
{
public:
    /// The pointer to the GSL's internal jacobian matrix
    gsl_vector * m_vector;
    
  public:
    /// Constructor
    GSLVector();
    /// Constructor
    GSLVector(const size_t n);

    GSLVector(const GSLVector& v);

    GSLVector(const std::vector<double>& v);

    GSLVector& operator=(const GSLVector& v);

    GSLVector& operator=(const std::vector<double>& v);

    /// Destructor.
    ~GSLVector();

    /// Get the pointer to the GSL vector
    gsl_vector * gsl(){return m_vector;}

    /// Get the pointer to the GSL vector
    const gsl_vector * gsl() const {return m_vector;}

    void resize(const size_t n);

    /// Size of the vector
    size_t size() const {return m_vector? m_vector->size : 0;}

    /// set an element
    void set(size_t i, double value)
    {
      if (i < m_vector->size) gsl_vector_set(m_vector,i,value);
      else
      {
        throw std::out_of_range("GSLVector index is out of range.");
      }
    }
    /// get an element
    double get(size_t i) const
    {
      if (i < m_vector->size) return gsl_vector_get(m_vector,i);
      throw std::out_of_range("GSLVector index is out of range.");
    }

    /// get an element
    double operator[](size_t i) const
    {
      if (i < m_vector->size) return gsl_vector_get(m_vector,i);
      throw std::out_of_range("GSLVector index is out of range.");
    }

    // Set all elements to zero
    void zero()
    {
      gsl_vector_set_zero( m_vector );
    }

    GSLVector& operator+=(const GSLVector& v)
    {
      gsl_vector_add(m_vector, v.gsl());
      return *this;
    }

    GSLVector& operator-=(const GSLVector& v)
    {
      gsl_vector_sub(m_vector, v.gsl());
      return *this;
    }

    GSLVector& operator*=(const double d)
    {
      gsl_vector_scale(m_vector, d);
      return *this;
    }

    void indexSort(std::vector<size_t>& indx);

};

  inline std::ostream& operator<<(std::ostream& ostr, const GSLVector& v)
  {
    ostr << std::scientific << std::setprecision(6);
    for(size_t j = 0; j < v.size(); ++j)
    {
      ostr << std::setw(13) << v.get(j) << ' ';
    }
    ostr << std::endl;
    return ostr;
  }

} // NUMERIC

#endif // NUMERIC_GSLVECTOR_H
