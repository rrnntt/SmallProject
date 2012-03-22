#include "Numeric/CostFunctionFactory.h"
#include "Numeric/ICostFunction.h"
#include "Kernel/Logger.h"

namespace Numeric
{
    CostFunctionFactory::CostFunctionFactory() : Kernel::DynamicFactory<ICostFunction>(), g_log(Kernel::Logger::get("CostFunctionFactory"))
    {
    }

} // namespace Numeric
