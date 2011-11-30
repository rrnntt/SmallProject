#include "Kernel/Logger.h"

namespace Kernel
{

Logger& Logger::get(const std::string& name)
{
  static Logger root;
  Logger* log = root.m_map[name];
  if (!log)
  {
    log = new Logger;
    log->m_name = name;
    root.m_map[name] = log;
  }
  return *log;
}

std::string Logger::id(const std::string& option)const
{
  if (option.empty())
  {
    return "[" + m_name + "] - ";
  }
  else
  {
    return "[" + m_name + " - " + option + "] - ";
  }
}

void Logger::debug(const std::string& msg)
{
  std::cerr << id("Debug") << msg << '\n';
}

void Logger::error(const std::string& msg)
{
  std::cerr << id("Error") << msg << '\n';
}

void Logger::information(const std::string& msg)
{
  std::cerr << id() << msg << '\n';
}

void Logger::warning(const std::string& msg)
{
  std::cerr << id("Warning") << msg << '\n';
}

} // Kernel
