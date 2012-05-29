#ifndef GOBLIN_ENERGYLIST_H
#define GOBLIN_ENERGYLIST_H

#include "Goblin/DllExport.h"
#include "Goblin/vjkg.h"
#include "API/TableWorkspace.h"

namespace Goblin
{

class GOBLIN_EXPORT EnergyList: public API::TableWorkspace
{
public:
  EnergyList();
  ~EnergyList(){}
  virtual std::string id()const {return "EnergyList";}
  /// Load from an asci file
  virtual void loadAscii(const std::string& fileName);
  size_t size() const {return rowCount();}
  double ener(size_t i) const {return i < size()? (*m_energy)[i]: 0.0;}
  const VJKG& q(size_t i)const{return i<m_q->size()?(*m_q)[i]:m_badQ;}
  size_t index(const VJKG& qq) const;
protected:
  void resetColumns();
  std::vector<double> *m_energy;
  std::vector<VJKG> *m_q;
  VJKG m_badQ;
};

typedef boost::shared_ptr<EnergyList> EnergyList_ptr;

} // namespace Goblin
#endif // GOBLIN_ENERGYLIST_H
