#ifndef NUMERIC_CHEBFUNTOTABLE_H
#define NUMERIC_CHEBFUNTOTABLE_H

#include "Numeric/DllExport.h"

#include "API/Algorithm.h"

namespace Numeric
{

class NUMERIC_EXPORT ChebfunToTable: public API::Algorithm
{
public:
  ChebfunToTable();
  virtual std::string name() const {return "ChebfunToTable";}

protected:
  void exec();
  void fromChebWorkspace();
  void fromChebVector();
};

} // Numeric

#endif // NUMERIC_CHEBFUNTOTABLE_H
