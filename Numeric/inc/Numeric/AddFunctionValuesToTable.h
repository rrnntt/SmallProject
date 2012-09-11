#ifndef NUMERIC_ADDFUNCTIONVALUESTOTABLE_H
#define NUMERIC_ADDFUNCTIONVALUESTOTABLE_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"
#include "API/TableWorkspace.h"
#include "API/NumericColumn.h"

namespace Numeric
{

<<<<<<< HEAD
// Forwrad declaration.
=======
// Forward declaration
>>>>>>> Add values to Table
class IFunction;

class NUMERIC_EXPORT AddFunctionValuesToTable: public API::Algorithm
{
public:
  AddFunctionValuesToTable();
  virtual std::string name() const {return "AddFunctionValuesToTable";}
  static void addColumn(API::TableWorkspace_ptr tws, 
    const std::string& xColumn,
    const std::string& yColumn,
    const IFunction& fun,
    API::NumericColumn::PlotRole role = API::NumericColumn::Y);
protected:
  virtual void exec();
};

} // NUMERIC

#endif // NUMERIC_ADDFUNCTIONVALUESTOTABLE_H
