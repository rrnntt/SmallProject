#include "DataObjects/CreateDistribution.h"
#include "DataObjects/TableWorkspace.h"
#include "DataObjects/NumericColumn.h"
#include "API/AlgorithmFactory.h"
#include "API/WorkspaceProperty.h"
#include "API/WorkspaceFactory.h"
#include "Kernel/CommonProperties.h"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/poisson_distribution.hpp>

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
    auto tws = wsProp.to<TableWorkspace>();

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
    size_t n = column->size();

    // random number generator
    //boost::mt19937 rand_gen;
    boost::rand48 rand_gen;

    std::string distr = get("Distribution");
    std::transform(distr.begin(),distr.end(),distr.begin(),tolower);

    if (distr == "poisson")
    {
      boost::poisson_distribution<> distribution(mean);
      for(size_t i = 0; i < n; ++i)
      {
        numColumn->setDouble(i,distribution(rand_gen));
      }
    }
    else //if (distr == "normal")
    {
      boost::normal_distribution<double> distribution(mean,sigma);
      std::cerr << "normal " << distribution.mean() << ' ' << distribution.sigma() << std::endl;
      for(size_t i = 0; i < n; ++i)
      {
        numColumn->setDouble(i,distribution(rand_gen));
      }
    }

  }

}  // DataObjects

