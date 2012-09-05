#include "Numeric/AddChebfun.h"
#include "Numeric/ChebfunWorkspace.h"

#include "API/AlgorithmFactory.h"
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
  declareWorkspace("Chebfun");
  declareWorkspace("Other");
  declareString("Operator");
}

void AddChebfun::exec()
{
  ChebfunWorkspace_sptr cws = getClass("Chebfun");

  ChebfunWorkspace_sptr cws1 = getClass("Other");

  std::string op = get("Operator");
  if (op.empty()) op = "+";

  if (op == "+")
  {
    cws->fun() += cws1->fun();
  }
  else if (op == "-")
  {
    cws->fun() -= cws1->fun();
  }
  else if (op == "*")
  {
    cws->fun() *= cws1->fun();
  }
  else if (op == "/")
  {
    cws->fun() /= cws1->fun();
  }
}

} // Numeric
