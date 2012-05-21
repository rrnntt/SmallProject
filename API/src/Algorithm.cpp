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

  /// Send a progress report to the listeners
  void Algorithm::sendProgress(size_t current, size_t maximum)
  {
    for(auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
    {
      (**it).algorithmProgress(this, current, maximum);
    }
  }


} // API
