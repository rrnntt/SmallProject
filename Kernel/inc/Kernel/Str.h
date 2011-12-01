#ifndef STR_H
#define STR_H

#include "Kernel/DllExport.h"

#include <cstring>
#include <string>
#include <vector>
#include <cctype>

namespace Str
{

  std::vector<std::string> KERNEL_EXPORT split(const std::string& str,char dlm,bool ignoreEmpty = true, bool trim = true);
  void KERNEL_EXPORT trim(std::string& str);
  std::string KERNEL_EXPORT inBrackets(const std::string& str,
                         const std::string& bra,
                         const std::string& ket,int& start,int& end);

}// namespace Str
#endif // STR_H
