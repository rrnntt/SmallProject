#include "Numeric/AddFunctionValuesToTable.h"
#include "Numeric/IFunction.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"
#include "Numeric/FunctionFactory.h"

#include "API/AlgorithmFactory.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

DECLARE_ALGORITHM(AddFunctionValuesToTable);

/// Constructor. Declare algorithm properties.
AddFunctionValuesToTable::AddFunctionValuesToTable()
{
    declareClass( "Function", &FunctionFactory::instance(), Kernel::Property::Input );
    declareWorkspace("Workspace");
    declareString("XColumn");
    declareString("YColumn");
    declareString("Role","Y");
}

/// Execute algorithm.
void AddFunctionValuesToTable::exec()
{
    API::TableWorkspace_ptr tws = getClass("Workspace");
    Numeric::IFunction_sptr fun = getClass("Function");
    std::string xColumn = get("XColumn");
    std::string yColumn = get("YColumn");
    std::string srole = get("Role");

    API::NumericColumn::PlotRole role = API::NumericColumn::Unset;
    if ( srole == "X")
    {
        role = API::NumericColumn::X;
    }
    else if ( srole == "Y")
    {
        role = API::NumericColumn::Y;
    }
    else if ( srole == "xError")
    {
        role = API::NumericColumn::xError;
    }
    else if ( srole == "yError")
    {
        role = API::NumericColumn::yError;
    }

    this->addColumn(tws, xColumn, yColumn, *fun, role);
}

/**
* Add a column to a table workspace filled with double values
* calculated with a function wich uses another numeric column as its x values.
* @param tws :: The table workspace.
* @param xColumn :: Name of the column with the x-values. Must exist and be numeric
* @param yColumn :: Name of a column to store the calculated values. If a column with
*  this name exists it will be overwritten if numeric or exception thrown otherwise.
*  If the column doesn't exist it will be created.
* @param fun :: A function to calculate the values.
*/
void AddFunctionValuesToTable::addColumn(API::TableWorkspace_ptr tws,
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
