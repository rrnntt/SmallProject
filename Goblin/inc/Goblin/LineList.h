#ifndef GOBLIN_LINELIST_H
#define GOBLIN_LINELIST_H

#include "Goblin/DllExport.h"
#include "Goblin/vjkg.h"
#include "DataObjects/TableWorkspace.h"

namespace Goblin
{

class GOBLIN_EXPORT LineList: public DataObjects::TableWorkspace
{
public:
  LineList();
  ~LineList(){}
  virtual std::string id()const {return "LineList";}
  /// Load from an asci file
  virtual void loadAscii(const std::string& fileName);
  size_t size() const {return rowCount();}
  double line(size_t i){return i<m_line->size()?(*m_line)[i]:0.0;}
  double height(size_t i){return i<m_height->size()?(*m_height)[i]:0.0;}
  const VJKG& q(size_t i)const{return (m_q && i<m_q->size())?(*m_q)[i]:m_badQ;}
  const VJKG& q0(size_t i)const{return (m_q0 && i<m_q0->size())?(*m_q0)[i]:m_badQ;}
  bool isAssigned()const {return m_q && m_q0;}
  void assign(size_t i, const VJKG& qup, const VJKG& qlo);
protected:
  void resetColumns();
  void makeQs();
  std::vector<double> *m_line;
  std::vector<double> *m_height;
  std::vector<VJKG> *m_q;
  std::vector<VJKG> *m_q0;
  VJKG m_badQ;
};

typedef boost::shared_ptr<LineList> LineList_ptr;

} // namespace Goblin
#endif // GOBLIN_LINELIST_H
