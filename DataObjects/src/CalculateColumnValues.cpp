#include "DataObjects/CalculateColumnValues.h"
#include "API/TableWorkspace.h"
#include "API/NumericColumn.h"

#include "API/AlgorithmFactory.h"

#include "Kernel/CommonProperties.h"

#include "Formula/Expression.h"
#include "Formula/Scalar.h"

namespace DataObjects
{

DECLARE_ALGORITHM(CalculateColumnValues);

CalculateColumnValues::CalculateColumnValues()
{
  declareClass("Workspace","WorkspaceManager");//(Kernel::Property::InOut));
  declareString("Column");
  declareString("Formula");
}

/**
* Retrieve the input table workspace from its property and validate it.
*/
boost::shared_ptr<API::TableWorkspace> CalculateColumnValues::getTableWorkspace()
{
  API::TableWorkspace_ptr tws = getClass("Workspace");
  //std::string wsName = static_cast<std::string>(wsProp);
  //if (wsName.empty())
  //{// TODO: this must be done by a validator
  //  throw std::runtime_error("Property Workspace was not set");
  //}
  //auto tws = wsProp.to<API::TableWorkspace>();

  if (!tws)
  {
    throw std::runtime_error("Workspace property is not a TableWorkspace");
  }

  if (tws->columnCount() == 0)
  {
    throw std::runtime_error("Empty table found");
  }
  return tws;
}

void CalculateColumnValues::exec()
{
  API::TableWorkspace_ptr tws = getTableWorkspace();
  std::string columnName = get("Column");
  std::string formula = get("Formula");
  //tws->fillColumn(columnName,formula);
  // exception is thrown if the column doesn't exist
  boost::shared_ptr<API::NumericColumn> numColumn = boost::dynamic_pointer_cast<API::NumericColumn>(
    tws->getColumn(columnName)
    );
  if ( !numColumn)
  {
    throw std::runtime_error("Cannot fill non-numeric column using Expression");
  }

  Formula::Namespace_ptr ns(new Formula::Namespace);
  double row = 0.0;
  ns->addVariable(Formula::Variable_ptr(new Formula::Scalar(&row)),"row");
  ns->addVariable(Formula::Variable_ptr(new Formula::Scalar(&row)),"i");
  ns->addVariable(Formula::Variable_ptr(new Formula::Scalar(2*acos(0.0))),"pi");
  ns->addVariable(Formula::Variable_ptr(new Formula::Scalar(exp(1.0))),"e");

  // define vars referencing values in all num columns
  std::vector<double> columnVars(tws->columnCount());
  std::vector<size_t> columnIndex;
  //for(auto col = m_columns.begin(); col != m_columns.end(); ++col)
  for(size_t col = 0; col < tws->columnCount(); ++col)
  {
    boost::shared_ptr<API::NumericColumn> nc = boost::dynamic_pointer_cast<API::NumericColumn>(
      tws->getColumn(col));
    if (nc)
    {
      ns->addVariable(Formula::Variable_ptr(new Formula::Scalar(&columnVars[col])),tws->getColumn(col)->name());
      columnIndex.push_back(col);
    }
  }

  try
  {
    Formula::Expression e(ns,formula);
    size_t n = tws->rowCount();
    for(size_t i = 0; i < n; ++i)
    {
      row = static_cast<double>(i);
      // update the variables
      for(auto col = columnIndex.begin(); col != columnIndex.end(); ++col)
      {
        columnVars[*col] = tws->getColumn(*col)->asNumeric()->getDouble(row);
      }
      double value = e.eval().as<Formula::Scalar>();
      numColumn->setDouble(i,value);
    }
  }
  catch(std::exception& ex)
  {
    throw std::runtime_error(std::string("fillColumn failed: ") + ex.what() );
  }
}

} // namespace DataObjects
