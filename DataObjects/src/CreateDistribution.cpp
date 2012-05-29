#include "DataObjects/CreateDistribution.h"
#include "API/TableWorkspace.h"
#include "API/NumericColumn.h"

#include "Numeric/Distribution.h"

#include "API/AlgorithmFactory.h"
#include "API/WorkspaceProperty.h"
#include "API/WorkspaceFactory.h"

#include "Kernel/CommonProperties.h"

#include <algorithm>

namespace DataObjects
{

  DECLARE_ALGORITHM(CreateDistribution);

  CreateDistribution::CreateDistribution()
  {
    declare("Workspace",new API::WorkspaceProperty(Kernel::Property::InOut));
    declare("Column",new Kernel::StringProperty);
    declare("Distribution",new Kernel::StringProperty);
    declare("Mean",new Kernel::DoubleProperty);
    declare("Sigma",new Kernel::DoubleProperty);
  }

  void CreateDistribution::exec()
  {
    API::WorkspaceProperty wsProp = get("Workspace").as<API::WorkspaceProperty>();
    std::string wsName = static_cast<std::string>(wsProp);
    if (wsName.empty())
    {// TODO: this must be done by a validator
      throw std::runtime_error("Property Workspace was not set");
    }
    auto tws = wsProp.to<API::TableWorkspace>();

    if (!tws)
    {
      throw std::runtime_error("Workspace property is not a TableWorkspace");
    }

    if (tws->columnCount() == 0)
    {
      throw std::runtime_error("Empty table found");
    }

    double mean = get("Mean").to<double>();
    double sigma = get("Sigma").to<double>();

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
        numColumn->setDouble(i,distribution->random());
      }
      delete distribution;
    }

  }

}  // DataObjects

