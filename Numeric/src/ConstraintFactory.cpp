#include "Numeric/ConstraintFactory.h"
//#include "MantidAPI/Expression.h"
#include "Numeric/IConstraint.h"
#include "Kernel/Logger.h"
#include "Kernel/EParser.h"
#include "API/Framework.h"

namespace Numeric
{

    ConstraintFactory::ConstraintFactory() : Kernel::DynamicFactory<IConstraint>()
    {
    }

    ConstraintFactory& ConstraintFactory::instance()
    {
      API::Singleton* s = API::Framework::instance().getSingleton("WorkspaceFactory");
      if (s == nullptr)
      {
        ConstraintFactory *f = new ConstraintFactory();
        API::Framework::instance().registerSingleton("FunctionFactory",f);
        return *f;
      }
      else
      {
        return *static_cast<ConstraintFactory*>(s);
      }
    }

    IConstraint* ConstraintFactory::createInitialized(IFunction* fun, const std::string& input) const
    {
      Kernel::EParser expr;
      expr.parse(input);
      return createInitialized(fun,expr);
    }

    IConstraint* ConstraintFactory::createInitialized(IFunction* fun, const Kernel::EParser& expr) const
    {
      IConstraint* c = 0;
      if (expr.name() == "==")
      {
        c = create("BoundaryConstraint");
      }
      else
      {
        c = create(expr.name());
      }
      c->initialize(fun,expr);
      return c;
    }

} // namespace Numeric
