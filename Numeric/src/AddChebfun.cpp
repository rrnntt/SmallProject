#include "Numeric/AddChebfun.h"
#include "Numeric/ChebfunWorkspace.h"

#include "API/AlgorithmFactory.h"
#include "API/WorkspaceProperty.h"
#include "API/WorkspaceFactory.h"

#include "Kernel/CommonProperties.h"

namespace Numeric
{

  DECLARE_ALGORITHM(AddChebfun);

/**
 * Constructor.
 */
AddChebfun::AddChebfun()
{
  declare("Chebfun",new API::WorkspaceProperty(Kernel::Property::InOut));
  declare("Other",new API::WorkspaceProperty(Kernel::Property::Input));
  declare("Operator",new Kernel::StringProperty);
}

void AddChebfun::exec()
{
  API::WorkspaceProperty cheb = get("Chebfun").as<API::WorkspaceProperty>();
  auto cws = cheb.to<ChebfunWorkspace>();
  if (!cws)
  {
    throw std::runtime_error("Chebfun property is not a ChebfunWorkspace");
  }

  API::WorkspaceProperty other = get("Other").as<API::WorkspaceProperty>();
  auto cws1 = other.to<ChebfunWorkspace>();
  if (!cws1)
  {
    throw std::runtime_error("Other property is not a ChebfunWorkspace");
  }

  std::string op = get("Operator");
  if (op.empty()) op = "+";

  if (op == "+")
  {
    *cws += *cws1;
  }
  else if (op == "-")
  {
    *cws -= *cws1;
  }
  else if (op == "*")
  {
    *cws *= *cws1;
  }
  else if (op == "/")
  {
    *cws /= *cws1;
  }
}

} // Numeric
