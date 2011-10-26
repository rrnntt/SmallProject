#ifndef NUMERIC_VECTOR_H
#define NUMERIC_VECTOR_H

#include "nu/DllExport.h"
#include "gsl/gsl_vector.h"

namespace nu
{
  /**
   * A wrapper around gsl_vector. A vector of doubles
   */
  class NUMERIC_DLL Vector: public gsl_vector
  {
  public:
    Vector(const size_t n);
  };

} // namespace nu

#endif /*NUMERIC_VECTOR_H*/