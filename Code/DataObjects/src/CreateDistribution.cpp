#include "DataObjects/CreateDistribution.h"
#include "API/TableWorkspace.h"
#include "API/NumericColumn.h"

#include "Numeric/Distribution.h"

#include "API/AlgorithmFactory.h"
#include "API/WorkspaceFactory.h"

#include "Kernel/CommonProperties.h"

#include <algorithm>

namespace DataObjects
{

  DECLARE_ALGORITHM(CreateDistribution);

  CreateDistribution::CreateDistribution()
  {
    declareWorkspace("Workspace");
    declareString("Column");
    declareString("Distribution");
    declareDouble("Mean");
    declareDouble("Sigma");
  }

  void CreateDistribution::exec()
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

    double mean = get("Mean");
    double sigma = get("Sigma");

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
    size_t n = column->size();

    // random number generator

    Numeric::Distribution* distribution = nullptr;

    std::string distr = get("Distribution");
    std::transform(distr.begin(),distr.end(),distr.begin(),tolower);

    if (distr == "poisson")
    {
      distribution = new Numeric::PoissonDistribution(mean);
    }
    else //if (distr == "normal")
    {
      distribution = new Numeric::NormalDistribution(mean,sigma);
    }

    if (distribution)
    {
      for(size_t i = 0; i < n; ++i)
      {
        numColumn->setDouble((int)i,distribution->random());
      }
      delete distribution;
    }

  }

}  // DataObjects

