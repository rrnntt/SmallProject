#include "DataObjects/CreateUniformRandomData.h"

#include "API/TableWorkspace.h"
#include "API/NumericColumn.h"
#include "API/AlgorithmFactory.h"
#include "API/WorkspaceFactory.h"
#include "Kernel/CommonProperties.h"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>

namespace DataObjects
{

  DECLARE_ALGORITHM(CreateUniformRandomData);

  CreateUniformRandomData::CreateUniformRandomData()
  {
    declareWorkspace("Workspace");
    declareDouble("MinValue");
    declareDouble("MaxValue");
    declareString("Column");
  }

  void CreateUniformRandomData::exec()
  {
    API::TableWorkspace_ptr tws = getClass("Workspace");

    if (!tws)
    {
      throw std::runtime_error("Workspace property is not a TableWorkspace");
    }

    if (tws->columnCount() == 0)
    {
      throw std::runtime_error("Empty table found");
    }

    double minValue = get("MinValue");
    double maxValue = get("MaxValue");

    if (minValue == maxValue)
    {
      minValue = 0.0;
      maxValue = 1.0;
    }
    std::cerr << "min=" << minValue << std::endl;
    std::cerr << "max=" << maxValue << std::endl;

    std::string colName = get("Column");

    API::Column_ptr column;
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

    auto numColumn = boost::dynamic_pointer_cast<API::NumericColumn>(column);

    // random number generator
    boost::mt19937 rand_gen;
    boost::uniform_real<> distribution(minValue,maxValue);

    size_t n = column->size();
    for(size_t i = 0; i < n; ++i)
    {
      numColumn->setDouble((int)i,distribution(rand_gen));
    }

  }

}  // DataObjects

