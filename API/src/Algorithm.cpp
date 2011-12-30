#include "API/Algorithm.h"

#include <stdexcept>
#include <algorithm>

namespace API
{

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
