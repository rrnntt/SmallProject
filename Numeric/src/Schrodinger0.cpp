#include "Numeric/Schrodinger0.h"
#include "Numeric/ChebfunWorkspace.h"
#include "Numeric/ChebOperator.h"
#include "Numeric/ICostFunction.h"

#include "API/AlgorithmFactory.h"
#include "API/WorkspaceProperty.h"
#include "API/TableWorkspace.h"
#include "API/TableColumn.h"
#include "API/NumericColumn.h"
#include "API/WorkspaceFactory.h"

#include "Kernel/CommonProperties.h"
#include "Kernel/EParser.h"

#include <boost/lexical_cast.hpp>

namespace Numeric
{

class Schrodinger0_Function: public ICostFunction 
{
public:
  Schrodinger0_Function(size_t n, double startX, double endX, ChebOperator& oper):
  m_poly(n,startX,endX),
  m_a(m_poly.coeffs()),
  m_sig(1.0),
  m_oper(oper)
  {
    m_a[0] = 1.0;
    m_poly.calcP();
  }

  /// Get name of minimizer
  virtual std::string name() const {return "Schrodinger0_Function";}

  /// Get i-th parameter
  /// @param i :: Index of a parameter
  /// @return :: Value of the parameter
  virtual double getParameter(size_t i)const
  {
    if ( i == 0 ) return m_sig;
    return m_a[i - 1];
  }
  /// Set i-th parameter
  /// @param i :: Index of a parameter
  /// @param value :: New value of the parameter
  virtual void setParameter(size_t i, const double& value)
  {
    if ( i == 0 ) m_sig = value;
    else
      m_a[i - 1] = value;
  }
  /// Number of parameters
  virtual size_t nParams()const {return m_a.size() + 1;}

  /// Calculate value of cost function
  virtual double val() const
  {
    m_poly.calcP();
    chebfun f(m_poly);
    auto& x = f.xpoints();
    std::vector<double> y(x.size());
    const double c = (m_poly.startX() + m_poly.endX())/2;
    for(size_t i = 0; i < x.size(); ++i)
    {
      const double xx = x[i] - c;
      y[i] = exp(-m_sig*xx*xx);
    }
    f.setP( y );
    f *= m_poly;

    chebfun ff(m_poly);
    m_oper.apply(f, ff);
    ff *= f;
    f.square();
    return ff.integr() / f.integr();
  }

  /// Calculate the derivatives of the cost function
  /// @param der :: Container to output the derivatives
  virtual void deriv(std::vector<double>& der) const 
  {
    const double v0 = val();
    der.resize(nParams());
    double p = m_sig;
    double dp = p * 0.00001;
    m_sig += dp;
    der[0] = ( val() - v0 ) / dp;
    m_sig = p;
    for(size_t i = 0; i < m_a.size(); ++i)
    {
      double p = m_a[i];
      double dp = 0.000001;
      if ( p != 0 ) dp *= p;
      m_a[i] += dp;
      der[i+1] = ( val() - v0 ) / dp;
      m_a[i] = p;
    }
  }

  /// Calculate the value and the derivatives of the cost function
  /// @param der :: Container to output the derivatives
  /// @return :: The value of the function
  virtual double valAndDeriv(std::vector<double>& der) const
  {
    double v = val();
    deriv( der );
  }

protected:
  mutable chebfun m_poly;
  mutable std::vector<double>& m_a;
  mutable double m_sig;
  ChebOperator& m_oper;
};


DECLARE_ALGORITHM(Schrodinger0);

/// Constructor. Declare algorithm properties.
Schrodinger0::Schrodinger0()
{
  declare("Operator",new Kernel::StringProperty);
  declare("StartX",new Kernel::DoubleProperty);
  declare("EndX",new Kernel::DoubleProperty);
  declare("N",new Kernel::IntProperty);
  declare("Chebfun",new API::WorkspaceProperty(Kernel::Property::Output));
  declare("Table",new API::WorkspaceProperty(Kernel::Property::Output));
}

/// Execute algorithm.
void Schrodinger0::exec()
{
  API::WorkspaceProperty wsProp = get("Chebfun").as<API::WorkspaceProperty>();
  double startX = get("StartX").to<double>();
  double endX = get("EndX").to<double>();
  if (endX <= startX)
  {
    throw std::invalid_argument("StartX must be <= EndX");
  }
  int n = get("N").to<int>();
  if (n < 2)
  {
    throw std::invalid_argument("N must be > 1");
  }
  std::string opStr = get("Operator");
  auto oper = ChebOperator::create( opStr );

  ChebfunWorkspace *cws = new ChebfunWorkspace;
  auto& y = cws->fun(0);
  y.set(n,startX,endX);

  GSLMatrix L;
  //size_t n = y.n() + 1;
  oper->createMatrix( y.getBase(), L );

  GSLVector d;
  GSLMatrix v;
  L.diag( d, v );

  size_t imin = 0;
  double dmin = d[0];
  for(size_t i = 1; i < d.size(); ++i)
  {
    const double tmp = d[i];
    if ( tmp < dmin )
    {
      dmin = tmp;
      imin = i;
    }
    std::cerr << i << ' ' << tmp << std::endl;
  }

  //imin = 29;
  y.setP( v, imin );

  wsProp = boost::shared_ptr<ChebfunWorkspace>(cws);

  auto tws = API::TableWorkspace_ptr(dynamic_cast<API::TableWorkspace*>(
    API::WorkspaceFactory::instance().create("TableWorkspace"))
    );
  tws->setRowCount(n);

  tws->addColumn("double","X");
  auto xColumn = static_cast<API::TableColumn<double>*>(tws->getColumn("X").get());
  xColumn->asNumeric()->setPlotRole(API::NumericColumn::X);
  auto& xc = xColumn->data();
  for(size_t j = 0; j < n; ++j)
  {
    xc[j] = y.xpoints()[j];
  }

  for(size_t i = 0; i < n; ++i)
  {
    std::string colName = "Y" + boost::lexical_cast<std::string>( i );
    tws->addColumn("double",colName);
    auto yColumn = static_cast<API::TableColumn<double>*>(tws->getColumn(colName).get());
    yColumn->asNumeric()->setPlotRole(API::NumericColumn::Y);
    auto& yc = yColumn->data();
    for(size_t j = 0; j < n; ++j)
    {
      yc[j] = v.get(j, i);
    }
  }

  get("Table").as<API::WorkspaceProperty>() = tws;
}

} // namespace Numeric