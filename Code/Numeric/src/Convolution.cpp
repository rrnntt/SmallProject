#include "Numeric/Convolution.h"
#include "Numeric/Chebfun.h"
#include "Numeric/IFunction.h"
#include "Numeric/ChebfunWorkspace.h"
#include "Numeric/FunctionFactory.h"

#include "API/AlgorithmFactory.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

DECLARE_ALGORITHM(Convolution);

/// Constructor. Declare algorithm properties.
Convolution::Convolution()
{
  declareClass("Resolution",&FunctionFactory::instance());
  declareClass("Model",&FunctionFactory::instance());
  declareWorkspace( "OutputWorkspace" );
  declareDouble( "StartX", -1.0 );
  declareDouble( "EndX", -1.0 );
  declareBool( "Inverse", false );
}

/// Execute algorithm.
void Convolution::exec()
{
  IFunction_sptr resolution = getClass("Resolution");
  IFunction_sptr model = getClass("Model");
  double start = get("StartX");
  double end = get("EndX");
  bool inverse = get("Inverse");
  chebfun work;
  work.convolution( *resolution, *model, start, end, inverse );
  ChebfunWorkspace *cws = new ChebfunWorkspace( work );
  setProperty("OutputWorkspace", boost::shared_ptr<ChebfunWorkspace>(cws) );
}

} // namespace Numeric
