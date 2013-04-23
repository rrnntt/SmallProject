#include "Numeric/SchrodScale.h"
#include "Numeric/Chebfun.h"
#include "Numeric/UserFunction1D.h"
#include "Numeric/ChebOperator.h"
#include "Numeric/ChebFunction.h"

#include "API/AlgorithmFactory.h"
#include "API/TableWorkspace.h"
#include "API/TableColumn.h"
#include "API/NumericColumn.h"
#include "API/WorkspaceFactory.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

DECLARE_ALGORITHM(SchrodScale);

/// Constructor. Declare algorithm properties.
SchrodScale::SchrodScale()
{
  declareWorkspace("Table");
}

/// Execute algorithm.
void SchrodScale::exec()
{
  auto tws = API::TableWorkspace_ptr(dynamic_cast<API::TableWorkspace*>(
    API::WorkspaceFactory::instance().create("TableWorkspace"))
    );
  const size_t n = 100;
  const size_t nn = n + 1;
  const double startX = -10;
  const double endX = 10;
  chebfun V(n, startX, endX);
  chebfun V1( V );

  UserFunction1D uV;
  uV.setAttributeValue("Formula","x^2+0.01*x^3+1");
  UserFunction1D uV1;
  uV1.setAttributeValue("Formula","x^2+1");

  V.fit( uV );
  V1.fit( uV1 );

  chebfun a( V );
  std::vector<double> aa( nn );
  for(size_t i = 0; i < nn; ++i)
  {
    aa[i] = 1.0 / sqrt( V.ypoints()[i] / V1.ypoints()[i] );
  }
  a.setP( aa );

  ChebFunction_sptr chbA(new ChebFunction(a));
  //chbA->fun(0) = a;
  ChebPlus plus; 
  plus.add('+',new ChebDiff);
  plus.add('-',new ChebTimes(chbA));

  chebfun y( a );
  ThroughPoint bc(0, 1);
  plus.solve( y, bc );

  tws->setRowCount(nn);
  tws->addDoubleColumn("X",API::NumericColumn::X);
  tws->addDoubleColumn("V",API::NumericColumn::Y);
  tws->addDoubleColumn("V1",API::NumericColumn::Y);
  tws->addDoubleColumn("a",API::NumericColumn::Y);
  tws->addDoubleColumn("y",API::NumericColumn::Y);
  
  auto& xdata = tws->getDoubleData("X");
  auto& vdata = tws->getDoubleData("V");
  auto& v1data = tws->getDoubleData("V1");
  auto& adata = tws->getDoubleData("a");
  auto& ydata = tws->getDoubleData("y");

  for(size_t i = 0; i < nn; ++i)
  {
    xdata[i] = V.xpoints()[i];
    vdata[i] = V.ypoints()[i];
    v1data[i] = V1.ypoints()[i];
    adata[i] = a.ypoints()[i];
    ydata[i] = y.ypoints()[i];
  }

  setProperty("Table", tws);
}

} // namespace Numeric
