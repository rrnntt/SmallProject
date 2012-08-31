#ifndef NUMERIC_MAKEQUADRATURESCHEME_H
#define NUMERIC_MAKEQUADRATURESCHEME_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"

namespace Numeric
{

/**
   \class MakeQuadratureScheme.
   \brief Creates a quadrature scheme for a weight function on an interval
 */
class NUMERIC_EXPORT MakeQuadratureScheme: public API::Algorithm
{
public:
  MakeQuadratureScheme();
  virtual std::string name() const {return "MakeQuadratureScheme";}
protected:
  virtual void exec();
  double readDouble(const std::string& str) const;
};

} // NUMERIC

#endif // NUMERIC_MAKEQUADRATURESCHEME_H
