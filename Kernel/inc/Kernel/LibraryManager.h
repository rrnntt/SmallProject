#ifndef MANTID_KERNEL_LIBRARY_MANAGER_H_
#define MANTID_KERNEL_LIBRARY_MANAGER_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

#include "Kernel/DllExport.h"
#include "Kernel/LibraryWrapper.h"

namespace Kernel
{
  class Logger;
  /** 
  Class for opening shared libraries.

  */
  class KERNEL_EXPORT LibraryManager
  {
  public:
    ///Private Constructor
    LibraryManager();
    ///Private Destructor
    virtual ~LibraryManager();
    //opens all suitable libraries on a given path
    int OpenAllLibraries(const std::string&, bool isRecursive=false);
  private:

    /// Private copy constructor - NO COPY ALLOWED
    LibraryManager(const LibraryManager&);
    /// Private assignment operator - NO ASSIGNMENT ALLOWED
    LibraryManager& operator = (const LibraryManager&);

    /// Load a given library
    bool loadLibrary(const std::string & filepath);
    /// Returns true if the library is to be loaded
    bool skip(const std::string & filename);
    ///Storage for the LibraryWrappers.
    std::map< const std::string, boost::shared_ptr<Kernel::LibraryWrapper> > OpenLibs;

    /// static reference to the logger class
    Logger& g_log;
  };


} // namespace Kernel

#endif //MANTID_KERNEL_LIBRARY_MANAGER_H_
