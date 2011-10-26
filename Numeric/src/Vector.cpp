#include "nu/Vector.h"

#include <iostream>

namespace nu
{

  /**
   * Vector constructor. Allocates memory for the data.
   * @param n :: Number of elements (size) in the vector.
   */
  Vector::Vector(const size_t n)
  {
    gsl_vector* v = gsl_vector_alloc(n);
    std::cerr << v << ' ' << v->block << std::endl;
  }

} // namespace nu
