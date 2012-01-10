#include "API/Algorithm.h"
#include "Kernel/Logger.h"

#include <stdexcept>
#include <algorithm>

namespace API
{
  Algorithm::Algorithm():g_log(Kernel::Logger::get("Algorithm"))
  {
  }
  void Algorithm::execute()
  {
    try
    {
      this->exec();
    }
    catch(std::exception& e)
    {
      std::cerr << "Algorithm error: " << e.what() << std::endl;
    }
  }

} // API
