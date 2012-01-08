#include "DataObjects/CreateUniformRandomData.h"
#include "DataObjects/TableWorkspace.h"
#include "DataObjects/NumericColumn.h"
#include "API/AlgorithmFactory.h"
#include "API/WorkspaceProperty.h"
#include "API/WorkspaceFactory.h"
#include "Kernel/CommonProperties.h"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>

namespace DataObjects
{

  DECLARE_ALGORITHM(CreateUniformRandomData);

  CreateUniformRandomData::CreateUniformRandomData()
  {
    declare("Workspace",new API::WorkspaceProperty(Kernel::Property::InOut));
    declare("MinValue",new Kernel::DoubleProperty);
    declare("MaxValue",new Kernel::DoubleProperty);
    declare("Column",new Kernel::StringProperty);
  }

  void CreateUniformRandomData::exec()
  {
    API::WorkspaceProperty wsProp = get("Workspace").as<API::WorkspaceProperty>();
    std::string wsName = static_cast<std::string>(wsProp);
    if (wsName.empty())
    {// TODO: this must be done by a validator
      throw std::runtime_error("Property Workspace was not set");
    }
    auto tws = wsProp.to<TableWorkspace>();

    if (!tws)
    {
      throw std::runtime_error("Workspace property is not a TableWorkspace");
    }

    if (tws->columnCount() == 0)
    {
      throw std::runtime_error("Empty table found");
    }

    double minValue = get("MinValue").to<double>();
    double maxValue = get("MaxValue").to<double>();

    if (minValue == maxValue)
    {
      minValue = 0.0;
      maxValue = 1.0;
    }
    std::cerr << "min=" << minValue << std::endl;
    std::cerr << "max=" << maxValue << std::endl;

    std::string colName = get("Column");

    Column_ptr column;
    if (colName.empty())
    {
      column = tws->getColumn(0);
      colName = column->name();
    }
    column = tws->getColumn(colName);
    if (!column->isNumeric())
    {
      throw std::runtime_error("Column " + colName + " is not numeric");
    }

    auto numColumn = boost::dynamic_pointer_cast<NumericColumn>(column);

    // random number generator
    boost::mt19937 rand_gen;
    boost::uniform_real<> distribution(minValue,maxValue);

    size_t n = column->size();
    for(size_t i = 0; i < n; ++i)
    {
      numColumn->setDouble(i,distribution(rand_gen));
    }

  }

}  // DataObjects

