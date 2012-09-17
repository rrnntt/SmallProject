#ifndef NUMERIC_CHEBFUNVECTOR_H
#define NUMERIC_CHEBFUNVECTOR_H

#include "Numeric/DllExport.h"
#include "Numeric/ChebFunction.h"

#include "API/Workspace.h"

#include <vector>

namespace Numeric
{

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
  /// add a function
  void add( ChebFunction_sptr f ){m_funs.push_back(f);}
  /// number of functions in the matrix
  size_t size() const {return m_funs.size();}
protected:
  /// The functions
  std::vector<ChebFunction_sptr> m_funs;
};

} // Numeric

#endif // NUMERIC_CHEBFUNVECTOR_H
