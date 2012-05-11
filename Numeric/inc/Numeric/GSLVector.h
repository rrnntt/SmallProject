#ifndef NUMERIC_GSLVECTOR_H
#define NUMERIC_GSLVECTOR_H

#include "Numeric/DllExport.h"

#include <gsl/gsl_vector.h>
#include <stdexcept>

namespace Numeric
{

class NUMERIC_EXPORT GSLVector
{
public:
    /// The pointer to the GSL's internal jacobian matrix
    gsl_vector * m_vector;
    
  public:
    /// Constructor
    GSLVector():m_vector(NULL){}
    /// Constructor
    GSLVector(const size_t n)
    {
      m_vector = gsl_vector_alloc(n);
    }

    GSLVector(const GSLVector& v)
    {
      m_vector = gsl_vector_alloc(v.size());
      gsl_vector_memcpy(m_vector, v.gsl());
    }

    GSLVector(const std::vector<double>& v)
    {
      m_vector = gsl_vector_alloc(v.size());
      for(size_t i = 0; i < v.size(); ++i)
      {
        gsl_vector_set( m_vector, i, v[i] );
      }
    }

    GSLVector& operator=(const GSLVector& v)
    {
      if (m_vector && size() != v.size())
      {
        gsl_vector_free(m_vector);
        m_vector = NULL;
      }
      if (!m_vector)
      {
        m_vector = gsl_vector_alloc(v.size());
      }
      gsl_vector_memcpy(m_vector, v.gsl());
      return *this;
    }

    /// Destructor.
    ~GSLVector()
    {
      if (m_vector)
      {
        gsl_vector_free(m_vector);
      }
    }

    /// Get the pointer to the GSL vector
    gsl_vector * gsl(){return m_vector;}

    /// Get the pointer to the GSL vector
    const gsl_vector * gsl() const {return m_vector;}

    void resize(const size_t n)
    {
      gsl_vector_free(m_vector);
      m_vector = gsl_vector_alloc(n);
    }

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

};

} // NUMERIC

#endif // NUMERIC_GSLVECTOR_H
