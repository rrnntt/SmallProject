#include "Numeric/Fit.h"
#include "Numeric/FunctionFactory.h"
#include "Numeric/IFunction.h"
#include "Numeric/LeastSquares.h"
#include "Numeric/LevenbergMarquardt.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/ChebfunWorkspace.h"
#include "Numeric/GSLMatrix.h"

#include "API/AlgorithmFactory.h"
#include "API/TableWorkspace.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

DECLARE_ALGORITHM(Fit);

//======================================================================
//     Private namespace
//======================================================================
namespace
{
  void createTableData(
    FunctionDomain_sptr& domain,
    FunctionValues_sptr& values,
    API::TableWorkspace_ptr tws,
    const std::string& xColumn,
    const std::string& yColumn,
    const std::string& errColumn
    );
  void createChebWSData(
    FunctionDomain_sptr& domain,
    FunctionValues_sptr& values,
    ChebfunWorkspace_sptr cws
    );
}

//======================================================================
/// Constructor. Declare algorithm properties.
Fit::Fit()
{
  // FunctionFcatory init string for a fitting function
  declareClass("Function",&FunctionFactory::instance());
  // A workspace with the data
  declareWorkspace("InputWorkspace");
  declareString("XColumn");
  declareString("YColumn");
  declareString("ErrColumn");
}

/**----------------------------------------------------------------------
 * Execute algorithm.
 */
void Fit::exec()
{
  IFunction_sptr fun = getClass("Function");
  API::Workspace_ptr ws = getClass("InputWorkspace");

  if ( fun->nParams() == 0 )
  {
    std::cerr << "Function has no fitting parameters." << std::endl;
    return;
  }

  FunctionDomain_sptr domain;
  FunctionValues_sptr values;

  if ( auto tws = boost::dynamic_pointer_cast<API::TableWorkspace>(ws) )
  {
    const std::string xColumn = get("XColumn");
    const std::string yColumn = get("YColumn");
    const std::string errColumn = get("ErrColumn");
    createTableData(domain,values,tws,xColumn,yColumn,errColumn);
  }
  else if ( auto cws = boost::dynamic_pointer_cast<ChebfunWorkspace>(ws) )
  {
    createChebWSData(domain,values,cws);
  }
  else
  {
    throw std::invalid_argument("Unsupported workspace type.");
  }

  LeastSquares* leastSquares = new LeastSquares;
  leastSquares->setFittingFunction(fun,domain,values);
  LevenbergMarquardt minimizer;
  minimizer.initialize(ICostFunction_sptr(leastSquares));

  minimizer.minimize();

  // calculate the covariance matrix
  GSLMatrix covar;
  leastSquares->calActiveCovarianceMatrix( covar );
}

//======================================================================
//     Private namespace
//======================================================================
namespace
{
  /**-------------------------------------------------------------------
   * Create fitting domain and values from a table workspace.
   * @param domain :: Created domain.
   * @param values :: Created and filled with fitting data and errors values struct.
   * @param tws :: The source table workspace.
   */
  void createTableData(
    FunctionDomain_sptr& domain,
    FunctionValues_sptr& values,
    API::TableWorkspace_ptr tws,
    const std::string& xColumn,
    const std::string& yColumn,
    const std::string& errColumn
    )
  {
    auto& x = tws->getDoubleData(xColumn);
    auto& y = tws->getDoubleData(yColumn);
    domain.reset( new FunctionDomain1DView( x ) );
    values.reset( new FunctionValues( *domain ) );
    values->setFitData( y );
    if ( !errColumn.empty() )
    {
      auto& err = tws->getDoubleData(errColumn);
      std::vector<double> wgts(err.size());
      for(size_t i = 0; i < wgts.size(); ++i)
      {
        if ( err[i] > 0 )
        {
          wgts[i] = 1.0 / err[i];
        }
        else
        {
          wgts[i] = 1.0;
        }
      }
      values->setFitWeights( wgts );
    }
    else
    {
      values->setFitWeights(1.0);
    }
  }

  /**-------------------------------------------------------------------
   * Create fitting domain and values from a chebfun workspace.
   * @param domain :: Created domain.
   * @param values :: Created and filled with fitting data and errors values struct.
   * @param cws :: The source chebfun workspace.
   */
  void createChebWSData(
    FunctionDomain_sptr& domain,
    FunctionValues_sptr& values,
    ChebfunWorkspace_sptr cws
    )
  {
    domain = cws->fun().createDomainFromXPoints();
    values.reset( new FunctionValues( *domain ) );
    FunctionValues tmp(*domain);
    cws->fun().function(*domain,tmp);
    values->setFitDataFromCalculated(tmp);
    values->setFitWeights(1.0);
  }

} // namespace

} // namespace Numeric
