#ifndef KERNEL_FRAMEWORK_H
#define KERNEL_FRAMEWORK_H

#include "Kernel/DllExport.h"
#include "Kernel/Singleton.h"
#include "Kernel/LibraryManager.h"

//#include <boost/ptr_container/ptr_map.hpp>
#include <map>

namespace Kernel
{

class KERNEL_EXPORT Framework
{
public:
  ~Framework();
  static Framework& instance();
  void registerSingleton(const std::string& name, Singleton* singleton);
  void unRegisterSingleton(const std::string& name, bool del = true);
  Singleton* getSingleton(const std::string& name);
  void parseCommandLine(int argc, char** argv);
  const std::string binDirectory() const;
  void openAllLibraries();
  /// Load a given library
  bool openLibrary(const std::string & filepath);
  const std::string testDirectory() const;
protected:
  Framework();
  std::map<std::string,Singleton*> m_singletons;
  Kernel::LibraryManager m_libraryManager;
  std::string m_binDirectory;
};

} // Kernel

#endif // KERNEL_FRAMEWORK_H
