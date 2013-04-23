#include "Kernel/Str.h"

namespace Str
{

  std::vector<std::string> split(const std::string& str,char dlm,bool ignoreEmpty, bool trim)
  {
    std::vector<std::string> res;
    size_t is = 0;
    size_t ie = 0;
    size_t ilast = str.size() - 1;

    for(ie=0;ie<str.size();ie++)
    {
      if (str[ie] == dlm || ie == ilast)
      {
        if (ie == is && str[ie] == dlm)
        {
          if ( !ignoreEmpty )
          {
            res.push_back("");
          }
        }//if (ie == is))
        else
        {
          size_t is1 = is;
          size_t ie1 = ie;
          if (ie == ilast && str[ie] != dlm) ie1++;
          if (trim)
          {
            while( isspace(str[is1]) && is1 < ie) is1++;
            while(ie1 > 0 && isspace(str[ie1-1]) && ie1 > is) ie1--;
            if (ie1 < is1) ie1 = is1 = is;
          }//trim
          if (is1 == ie1)
          {
            if ( !ignoreEmpty )
            {
              res.push_back("");
            }
          }
          else
          {
            res.push_back(str.substr(is1,ie1-is1));
          }
        }//(ie != is)
        is = ie + 1;
        if (ie == ilast && str[ie] == dlm && !ignoreEmpty)
        {
          res.push_back("");
        }
      }
    }// main loop

    return res;
  }

  void trim(std::string& str)
  {
    if (str.empty()) return;
    std::string::size_type i0 = str.find_first_not_of(" \t\n\r");
    std::string::size_type i1 = str.find_last_not_of(" \t\n\r");
    if (i1 >= i0)
    {
      if (i1 != 0 || i1 != str.size()-1)
      {
        str = str.substr(i0,i1-i0+1);
      }
    }
    else
    {
      str = "";
    }
  }

  /**
    * Find text in brackets.
    * @param bra The opening bracket
    * @para, ket The closing bracket
    * @return The text in brackets
    * @throw std::invalid_argument in case of bracket mismatch
    */
  std::string inBrackets(const std::string& str,const std::string& bra ,const std::string& ket)
  {
    return "";
  }

}// namespace Str

