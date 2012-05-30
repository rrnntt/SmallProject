#include "Numeric/IFuncMinimizer.h"

#include <boost/lexical_cast.hpp>

namespace Numeric
{

/**
 * Do minimization of the set function.
 * @return :: true if successful, false otherwise. Call getError() to see the error message string.
 */
bool IFuncMinimizer::minimize(size_t maxIterations) 
{
  m_errorString = ""; // iterate() may modify it
  size_t iter = 0;
  bool success = false;
  do
  {
    iter++;
    if ( !iterate() )
    {
      success = m_errorString.empty() || m_errorString == "success";
      m_errorString = "success";
      break;
    }
  }
  while (iter < maxIterations);

  if (iter >= maxIterations)
  {
    success = false;
    if ( !m_errorString.empty() )
    {
      m_errorString += '\n';
    }
    m_errorString += "Failed to converge after " + boost::lexical_cast<std::string>(maxIterations) + " iterations.";
  }

  std::cerr << "Stopped after " << iter << " iterations" << std::endl;

  return success;

}

} // Numeric
