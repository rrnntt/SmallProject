#include "Numeric/AddFunctionValuesToTable.h"
#include "Numeric/IFunction.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"

#include "API/AlgorithmFactory.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

DECLARE_ALGORITHM(AddFunctionValuesToTable);

/// Constructor. Declare algorithm properties.
AddFunctionValuesToTable::AddFunctionValuesToTable()
{
}

/// Execute algorithm.
void AddFunctionValuesToTable::exec()
{
}

/**
 * Add values calculated by a IFunction1D to a table workspace.
 * @param tws :: Table workspace to modify.
 * @param xColumn :: Name of the column with the x-values. Must exist and be double type.
 * @param yColumn :: Name of a column to store the calculated values. If a column with 
 *  this name exists it will be overwritten if numeric or exception thrown otherwise.
 *  If the column doesn't exist it will be created.
 * @param fun :: A function to calculate the values.
 */
void AddFunctionValuesToTable::addValuesToWorkspace(API::TableWorkspace_ptr tws, 
    const std::string& xColumn,
    const std::string& yColumn,
    const IFunction& fun,
    API::NumericColumn::PlotRole role
    )
  {
    auto& x = tws->getDoubleData( xColumn );
    if ( tws->hasColumn( yColumn ) )
    {
      if ( !tws->getColumn( yColumn )->isNumeric() )
        throw std::runtime_error("Column "+yColumn+" isn't numeric");
    }
    else
    {
      tws->addColumn( "double", yColumn );
    }
    tws->getColumn( yColumn )->asNumeric()->setPlotRole(role);
    auto& y = tws->getDoubleData( yColumn );

    FunctionDomain1DView domain( x.data(), x.size() );
    FunctionValues values( domain );
    fun.function( domain, values );
    for(size_t i = 0; i < y.size(); ++i)
    {
      y[i] = values.getCalculated(i);
    }
  }

} // namespace Numeric
