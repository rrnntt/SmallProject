#ifndef NUMERIC_CHEBOPERATOR_H
#define NUMERIC_CHEBOPERATOR_H

#include "Numeric/DllExport.h"
#include "Numeric/ChebFunction.h"
#include "Numeric/GSLMatrix.h"

#include <map>

namespace Numeric
{

/**
 * A base class for a linear operator acting on a chebfun.
 */
class NUMERIC_EXPORT ChebOperator
{
public:
  ChebOperator();
  virtual ~ChebOperator();
  /// Apply this operator to a chebfun.
  /// @param arg :: Function to apply the operator to.
  /// @param res :: Function - result of the operation.
  virtual void apply(const chebfun& arg, chebfun& res);
  /// Create operator matrix
  /// @param base :: The base of the result function
  virtual void createMatrix(ChebfunBase_const_sptr base, GSLMatrix& L) = 0;
protected:
  /// Cached operator matrices for different chebfun bases
  std::map< ChebfunBase_const_sptr, GSLMatrix* > m_matrixCache;
};

/**
 * A first derivative operator.
 */
class NUMERIC_EXPORT ChebDiff: public ChebOperator
{
public:
  /// Create operator matrix
  /// @param base :: The base of the result function
  void createMatrix(ChebfunBase_const_sptr base, GSLMatrix& L);
};

} // NUMERIC

#endif // NUMERIC_CHEBOPERATOR_H
