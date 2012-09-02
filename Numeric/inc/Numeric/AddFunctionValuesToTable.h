#ifndef NUMERIC_ADDFUNCTIONVALUESTOTABLE_H
#define NUMERIC_ADDFUNCTIONVALUESTOTABLE_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"

namespace Numeric
{

class NUMERIC_EXPORT AddFunctionValuesToTable: public API::Algorithm
{
public:
  AddFunctionValuesToTable();
  virtual std::string name() const {return "AddFunctionValuesToTable";}
protected:
  virtual void exec();
};

} // NUMERIC

#endif // NUMERIC_ADDFUNCTIONVALUESTOTABLE_H
