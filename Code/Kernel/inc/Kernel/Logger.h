#ifndef LOGGER_H
#define LOGGER_H

#include "DllExport.h"
#include <map>
#include <string>
#include <iostream>


namespace Kernel
{

class KERNEL_EXPORT Logger
{
public:
  static Logger& get(const std::string& name);
  template<class T> Logger& operator<<(const T& out);
  void debug(const std::string&);
  void error(const std::string&);
  void information(const std::string&);
  void warning(const std::string&);
private:
  Logger(){}
  std::string id(const std::string& option = "")const;
  std::map<std::string,Logger*> m_map;
  std::string m_name;
};

template<class T>
Logger& Logger::operator<<(const T& out)
{
  std::cerr << id() << out;
  return *this;
}

} // Kernel

#endif // LOGGER_H
