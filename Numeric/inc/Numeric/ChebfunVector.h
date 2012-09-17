#ifndef NUMERIC_CHEBFUNVECTOR_H
#define NUMERIC_CHEBFUNVECTOR_H

#include "Numeric/DllExport.h"
#include "Numeric/ChebFunction.h"

#include "API/Workspace.h"

#include <vector>

namespace Numeric
{

//========================================================
//    Forward declaration
//========================================================
class GSLMatrix;

/**
 * A vector of ChebFunctions
 */
class NUMERIC_EXPORT ChebfunVector: public API::Workspace
{
public:
  /// Constructor
  ChebfunVector();
  /// ~Destructor
  ~ChebfunVector();
  /// Workspace type id
  virtual std::string id() const {return "ChebfunVector";}
  /// get i-th function
  ChebFunction& fun(size_t i){return *m_funs[i];}
  /// get i-th function
  const ChebFunction& cfun(size_t i) const {return *m_funs[i];}
  /// add a function
  void add( ChebFunction_sptr f ){m_funs.push_back(f);}
  /// number of functions in the matrix
  size_t size() const {return m_funs.size();}
  /// Create a ChebfunVector (this) from another ChebfunVector as their linear combinations
  void fromLinearCombinations(const ChebfunVector& basis, const GSLMatrix& coeffs);
  /// Sort the functions in the vector according to a table
  void sort(const std::vector<size_t>& indx);
protected:
  /// The functions
  std::vector<ChebFunction_sptr> m_funs;
};

/// typedef shared pointer
typedef boost::shared_ptr<ChebfunVector> ChebfunVector_sptr;
typedef boost::shared_ptr<const ChebfunVector> ChebfunVector_const_sptr;

} // Numeric

#endif // NUMERIC_CHEBFUNVECTOR_H
