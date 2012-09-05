#include "Goblin/Diatom.h"

#include "Numeric/Constants.h"
#include "Numeric/IFunction.h"
#include "Numeric/FunctionFactory.h"
#include "Numeric/ChebFunction.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"

#include "API/AlgorithmFactory.h"
#include "API/TableWorkspace.h"

#include "Kernel/CommonProperties.h"

#include <stdexcept>

using namespace Numeric;

namespace Goblin
{

DECLARE_ALGORITHM(Diatom);

/// Constructor. Declare algorithm properties.
Diatom::Diatom()
{
  declareDouble("mu"); // reduced mass in mass_p
  declareString("Vpot"); // in cm-1 as a function of r in angstroms
  declareInt("nmax",10);
  declareInt("Jmax",10);
  declareDouble("rmin",0.01);
  declareDouble("rmax",20.0); // the integration range [rmin,rmax] in angstrom
}

/// Execute algorithm.
void Diatom::exec()
{
  double mu = get("mu");
  if ( mu <= 0 )
  {
    throw std::invalid_argument("mu cannot be negative or zero");
  }
  std::string funStr = get("Vpot");
  Numeric::IFunction_sptr Vpot = Numeric::FunctionFactory::instance().createFitFunction(funStr);
  int nmax = get("nmax");
  int Jmax = get("Jmax");
  const double rmin = get("rmin");
  const double rmax = get("rmax");

  std::cerr << planck_hbar * planck_hbar / 2 / mass_p << std::endl;
  std::cerr << planck_h * light_speed * 100 * angstrom * angstrom << std::endl;

  // convert hbar^2/mu -> beta in cm-1 and r in angstroms

  double beta = planck_hbar * planck_hbar / mass_p / mu / (planck_h * light_speed * 100 * angstrom * angstrom);

  // equation now is -0.5*beta*d2/dr2 + ... where beta in cm-1 and r in angstroms

  std::cerr << "beta=" << beta << std::endl;

  // find the potential minimum

  nmax = 4;
  ChebFunction v((size_t)nmax, rmin, rmax);
  v.bestFit( *Vpot );
  double r0 = 0;
  double alpha = 0;
  std::cerr << "n=" << v.cfun(0).n() << std::endl;
  {
    ChebFunction dv;
    dv.fromDerivative( v );
    //FunctionDomain1DVector domain(rmin,rmax,100);
    //FunctionValues values( domain );
    //dv.function( domain, values );
    //for(size_t i = 0; i < domain.size(); ++i)
    //{
    //  std::cerr << domain[i] << ' ' << values[i] << std::endl;
    //}

    std::vector<double> r;
    dv.roots( r );
    if ( r.size() != 1 )
    {
      throw std::runtime_error("Failed to find the potential minimum");
    }
    r0 = r[0];
    std::cerr << "Minimum at r0=" << r0 << std::endl;
    ChebFunction dv2;
    dv2.fromDerivative( dv );
    dv2.function1D(&alpha,&r0,1);
    std::cerr << "Alpha=" << alpha << std::endl;
    std::cerr << "E0=" << 0.5*sqrt(alpha*beta) << std::endl;
  }

  // compute the ground state 
  auto alg = API::AlgorithmFactory::instance().createAlgorithm("MakeQuadratureScheme");
  alg->setProperty("N",5);
  alg->setProperty("Interval","0, 12");
  alg->setProperty("Function","UserFunction1D(Formula=exp(-x^2))");
}

} // namespace Goblin
