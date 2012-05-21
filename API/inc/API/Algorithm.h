#ifndef API_ALGORITHM_H
#define API_ALGORITHM_H

#include "API/DllExport.h"
#include "Kernel/PropertyManager.h"

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

namespace Kernel
{
  class Logger;
}

namespace API
{

  class AlgorithmListener;

  /**
   * Base class for an algorithm.
   */
  class API_EXPORT Algorithm: public Kernel::PropertyManager
  {
  public:
    Algorithm();
    virtual ~Algorithm(){}
    virtual std::string name() const = 0;
    /// Execute the algorithm.
    void execute();
    /// Add a listener who will be listening to progress messages
    /// @param listener :: Pointer to a listener. The pointer must be valid during life-time
    /// of the algorithm. 
    void addListener(AlgorithmListener* listener){m_listeners.push_back(listener);}
  protected:
    virtual void exec() = 0;
    /// Send a progress report to the listeners
    void sendProgress(size_t current, size_t maximum);

    std::vector<AlgorithmListener*> m_listeners;
    Kernel::Logger& g_log;
  };
  
  /**
   * Base class for objects who want to listen to algorithm progress reports.
   */
  class AlgorithmListener
  {
  public:
    /// Virtual destructor
    virtual ~AlgorithmListener();
    /// Method that receives a progress report
    virtual void algorithmProgress(Algorithm* alg, size_t current, size_t maximum) = 0;
  };

  typedef boost::shared_ptr<Algorithm> Algorithm_ptr;

} // API

#endif // API_ALGORITHM_H
