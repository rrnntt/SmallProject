#ifndef NUMERIC_ADDFUNCTIONVALUESTOTABLE_H
#define NUMERIC_ADDFUNCTIONVALUESTOTABLE_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"
#include "API/NumericColumn.h"
#include "API/TableWorkspace.h"

namespace Numeric
{

// Forwrad declaration.
class IFunction;

class NUMERIC_EXPORT AddFunctionValuesToTable: public API::Algorithm
{
public:
  AddFunctionValuesToTable();
  virtual std::string name() const {return "AddFunctionValuesToTable";}
  static void addValuesToWorkspace(
    API::TableWorkspace_ptr tws, 
    const std::string& xColumn,
    const std::string& yColumn,
    const IFunction& fun,
    API::NumericColumn::PlotRole role
    );
protected:
  virtual void exec();
};

} // NUMERIC

#endif // NUMERIC_ADDFUNCTIONVALUESTOTABLE_H
