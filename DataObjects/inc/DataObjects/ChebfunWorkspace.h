#ifndef DATAOBJECTS_CHEBFUNWORKSPACE_H
#define DATAOBJECTS_CHEBFUNWORKSPACE_H

#include "DataObjects/DllExport.h"
#include "API/Workspace.h"
#include "Numeric/Chebfun.h"

namespace DataObjects
{

class DATAOBJECTS_EXPORT ChebfunWorkspace: public API::Workspace
{
public:
  ChebfunWorkspace();
  ~ChebfunWorkspace(){}
  virtual std::string id()const {return "ChebfunWorkspace";}
  int n()const{return m_fun->n();}
  void set(int n,const double& startX = -1,const double& endX = 1){m_fun->set(n,startX,endX);}
  double startX()const{return m_fun->startX();}
  void setStartX(const double& d){m_fun->setStartX(d);}
  double endX()const{return m_fun->endX();}
  void setEndX(const double& d){m_fun->setEndX(d);}
  void setRange(const double& s,const double& e){m_fun->setRange(s,e);}
  double& param(size_t i){return m_fun->param(i);}
  const std::vector<double>& xpoints()const{return m_fun->xpoints();}
  Numeric::chebfun& fun() {return *m_fun;}
protected:
  Numeric::chebfun_ptr m_fun;
};

} // namespace DataObjects
#endif // DATAOBJECTS_CHEBFUNWORKSPACE_H
