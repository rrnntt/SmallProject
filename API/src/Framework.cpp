#include "API/Framework.h"

#include <boost/filesystem.hpp>
#include <iostream>

using namespace boost::filesystem;

namespace API
{
    struct APIStuff
  {
    ~APIStuff()
    {
      std::cerr << "API unloaded\n";
    }
  };

  static APIStuff stuff;

Framework& Framework::instance()
{
  static Framework framework;
  return framework;
}

Framework::Framework():
m_binDirectory(".")
{
}

/**
 * Destructor: nothing to do.
 */
Framework::~Framework()
{
  for(auto s = m_singletons.begin(); s != m_singletons.end(); ++s)
  {
    delete s->second;
  }
  m_singletons.clear();
  std::cerr << "Framework deleted\n";
}

void Framework::openAllLibraries()
{
  m_libraryManager.OpenAllLibraries(m_binDirectory);
}

void Framework::registerSingleton(const std::string& name, Singleton* singleton)
{
  if (singleton == nullptr)
  {
    throw std::runtime_error("Cannot register a null singleton");
  }
  if (m_singletons.find(name) == m_singletons.end())
  {
    //m_singletons.insert(name,std::auto_ptr<Singleton>(singleton));
    m_singletons.insert(std::make_pair(name,singleton));
  }
  else
  {
    throw std::runtime_error("Singleton " + singleton->name() + " already registered");
  }
}

void Framework::unRegisterSingleton(const std::string& name, bool del)
{
  auto s = m_singletons.find(name);
  if (s != m_singletons.end())
  {
    if (del)
    {
      delete s->second;
    }
    m_singletons.erase(s);
  }
}

Singleton* Framework::getSingleton(const std::string& name) 
{
  if (m_singletons.empty())
  {
    return nullptr;
  }
  else
  {
    auto s = m_singletons.find(name);
    if (s != m_singletons.end())
    {
      return s->second;
    }
    return nullptr;
  }
}

void Framework::parseCommandLine(int argc, char** argv)
{
  path binPath(argv[0]);
  m_binDirectory = binPath.parent_path().string()+"/";
  std::cerr << "bin directory: " << m_binDirectory << std::endl;
}

const std::string Framework::binDirectory() const
{
  if (m_binDirectory.empty())
  {
    throw std::runtime_error("Bin directory is not known");
  }
  return m_binDirectory;
}

} // API
