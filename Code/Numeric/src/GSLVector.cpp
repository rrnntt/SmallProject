#include "Numeric/GSLVector.h"

#include <algorithm>

namespace Numeric
{

  /// Constructor
  GSLVector::GSLVector():m_vector(NULL){}
  /// Constructor
  GSLVector::GSLVector(const size_t n)
  {
    m_vector = gsl_vector_alloc(n);
  }

  GSLVector::GSLVector(const GSLVector& v)
  {
    m_vector = gsl_vector_alloc(v.size());
    gsl_vector_memcpy(m_vector, v.gsl());
  }

  GSLVector::GSLVector(const std::vector<double>& v)
  {
    m_vector = gsl_vector_alloc(v.size());
    for(size_t i = 0; i < v.size(); ++i)
    {
      gsl_vector_set( m_vector, i, v[i] );
    }
  }

  GSLVector& GSLVector::operator=(const GSLVector& v)
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
  GSLVector::~GSLVector()
  {
    if (m_vector)
    {
      gsl_vector_free(m_vector);
    }
  }

  //==========================================================
  // Private namespace
  //==========================================================
  namespace
  {
    struct SortAscending
    {
      const GSLVector& m_vec;
      SortAscending(const GSLVector& vec):m_vec(vec){}
      bool operator()(size_t i, size_t j)
      {
        return m_vec[i] < m_vec[j];
      }
    };
  }

  /**
   * Generate indices that would sort this vector in ascending order
   * @param indx :: STD vector to output the sorting indices.
   */
  void GSLVector::indexSort(std::vector<size_t>& indx)
  {
    indx.resize( size() );
    for(size_t i = 0; i < indx.size(); ++i)
    {
      indx[i] = i;
    }
    std::sort(indx.begin(),indx.end(),SortAscending(*this));
  }

} // NUMERIC
