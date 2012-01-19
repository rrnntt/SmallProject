#ifndef GOBLIN_OBJECT_H
#define GOBLIN_OBJECT_H

#include "Goblin/DllExport.h"

namespace Goblin
{

  struct cmd_res
  {
    bool ok;
    bool _repaint;
    cmd_res(bool OK = false):ok(OK),_repaint(false){}
    void repaint(bool yes){_repaint = yes;}
  };

  struct fun_res
  {
    bool ok;
    fun_res(bool OK):ok(OK){}
  };

  struct rgb
  {
    char r,g,b;
    rgb(char a1,char a2, char a3):r(a1),g(a2),b(a3){}
  };

class GOBLIN_EXPORT object
{
public:
  object();
  ~object(){}
};

} // namespace Goblin

#endif // GOBLIN_OBJECT_H
