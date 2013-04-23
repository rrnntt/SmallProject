#ifndef NUMERIC_VECTOR_H
#define NUMERIC_VECTOR_H

#include "Numeric/DllExport.h"
#include "gsl/gsl_vector.h"

namespace Numeric
{
  /**
   * A wrapper around gsl_vector. A vector of doubles
   */
  class NUMERIC_EXPORT Vector: public gsl_vector
  {
  public:
    Vector(const size_t n);
  };

} // namespace Numeric

#endif /*NUMERIC_VECTOR_H*/