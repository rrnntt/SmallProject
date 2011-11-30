#include <iostream>

#include "Kernel/DllOpen.h"
#include "Kernel/LibraryManager.h"
#include "Kernel/Logger.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <set>

using namespace boost::filesystem;

namespace Kernel
{

  /// Constructor
  LibraryManager::LibraryManager() 
    : g_log(Logger::get("LibraryManager"))
  {
    g_log.debug( "LibraryManager created." );
  }

  /// Destructor
  LibraryManager::~LibraryManager()
  {
  }

  /** Opens all suitable DLLs on a given path.
  *  @param filePath :: The filepath to the directory where the libraries are.
  *  @param isRecursive :: Whether to search subdirectories.
  *  @return The number of libraries opened.
  */
  int LibraryManager::OpenAllLibraries(const std::string& filePath,
    bool isRecursive)
  {
    g_log.debug( "Opening all libraries in " + filePath );
    int libCount = 0;
    //validate inputs
    //Poco::File libPath;
    path libPath;
    try
    {
      libPath = path(filePath);
    }
    catch(...)
    {
      return libCount;
    }
    if ( exists(libPath) && is_directory(libPath) )
    {
      DllOpen::addSearchDirectory(filePath);
      // Iteratate over the available files
      directory_iterator end_itr;
      for (directory_iterator itr(libPath); itr != end_itr; ++itr)
      {
        const path & item = *itr;
        if ( is_directory(item) )
        {
          if (isRecursive)
          {
            libCount += OpenAllLibraries(item.string());
          }
        }
        else
        {
          if( skip(item.string()) ) continue;
          if( loadLibrary(item.string()) ) 
          {
            ++libCount;
          }
        }
      }
    }
    else
    {
      g_log.error("In OpenAllLibraries: " + filePath + " must be a directory.");
    }

    return libCount;
  }

  //-------------------------------------------------------------------------
  // Private members
  //-------------------------------------------------------------------------
  /**
  * Returns true if the name contains one of the strings given in the
  * 'plugins.exclude' variable. Each string from the variable is
  * searched for with the filename so an exact match is not necessary. This
  * avoids having to specify prefixes and suffixes for different platforms,
  * i.e. 'plugins.exclude = MantidKernel' will exclude libMantidKernel.so
  * @param filename :: A string giving the filename/file path
  * @return True if the library should be skipped
  */
  bool LibraryManager::skip(const std::string & filename)
  {
    return false;
  //  static std::set<std::string> excludes;
  //  static bool initialized(false);
  //  if( !initialized )
  //  {
  //    std::string excludeStr = ConfigService::Instance().getString("plugins.exclude");
  //    boost::split(excludes, excludeStr, boost::is_any_of(":;"), boost::token_compress_on);
  //    initialized = true;
  //  }
  //  bool skipme(false);
  //  for( std::set<std::string>::const_iterator itr = excludes.begin(); itr != excludes.end();
  //    ++itr)
  //  {
  //    if( filename.find(*itr) != std::string::npos)
  //    {
  //      skipme = true;
  //      break;
  //    }
  //  }
  //  return skipme;
  }

  /** 
  * Load a library
  * @param filepath :: The full path to a library as a string
  */
  bool LibraryManager::loadLibrary(const std::string & filepath)
  {
    // Get the name of the library.
    std::string libName = DllOpen::ConvertToLibName(path(filepath).filename().string());
    if( libName.empty() ) return false;
    // The wrapper will unload the library when it is deleted
    boost::shared_ptr<LibraryWrapper> dlwrap(new LibraryWrapper);
    std::string libNameLower = boost::algorithm::to_lower_copy(libName);

    //Check that a libray with this name has not already been loaded
    if (OpenLibs.find(libNameLower) == OpenLibs.end())
    {
      path directory = path(filepath).parent_path();
      g_log.debug( "Trying to open library: " + libName + " from " + directory.string() + " ..." );
      //Try to open the library
      if (dlwrap->OpenLibrary(libName, directory.string()))
      {
        //Successfully opened, so add to map
        g_log.debug("Opened library: " + libName + ".\n");
        OpenLibs.insert(std::pair< std::string, boost::shared_ptr<LibraryWrapper> >(libName, dlwrap) );
        return true;
      }
      else
      {
        return false;
      }
    }
    else
    {
      g_log.debug( libName + " already opened, skipping load\n" );
    }
    return false;
  }

} // namespace Kernel
