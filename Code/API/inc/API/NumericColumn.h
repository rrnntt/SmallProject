#ifndef API_NUMERICCOLUMN_H
#define API_NUMERICCOLUMN_H

#include "API/DllExport.h"
#include "API/Column.h"

#include <vector>
#include <algorithm>

namespace API
{

class API_EXPORT NumericColumn
{
public:
  NumericColumn():m_plotRole(Unset){}
  enum PlotRole { Unset = 0, X, Y, Z, xError, yError };
  virtual double getDouble(int index)const = 0;
  virtual void setDouble(int index, double value) = 0;
//protected:
  virtual std::vector<double>* getDoubleVector() {return nullptr;}
  virtual std::vector<float>* getFloatVector() {return nullptr;}
  virtual std::vector<int>* getIntVector() {return nullptr;}
  virtual const std::vector<double>* getDoubleVector() const {return nullptr;}
  virtual const std::vector<float>* getFloatVector() const {return nullptr;}
  virtual const std::vector<int>* getIntVector() const {return nullptr;}
  template<typename T>
  void fill(std::vector<T>& vec)const;
  PlotRole getPlotRole() const {return m_plotRole;}
  void setPlotRole(PlotRole role) {m_plotRole = role;}
protected:
  PlotRole m_plotRole; ///< Role of the column in a plot
};

template<typename T>
void NumericColumn::fill(std::vector<T>& vec)const
{
  const std::vector<double>* vd = getDoubleVector();
  if (vd)
  {
    vec.assign(vd->begin(),vd->end());
    return;
  }
  const std::vector<float>* fv = getFloatVector();
  if (fv)
  {
    vec.assign(fv->begin(),fv->end());
    return;
  }
  const std::vector<int>* iv = getIntVector();
  if (iv)
  {
    vec.assign(iv->begin(),iv->end());
    return;
  }
}

} // API

#endif // API_NUMERICCOLUMN_H
