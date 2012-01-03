#include "Kernel/Notification.h"

#include <algorithm>

namespace Kernel
{

class ObserverRegister
{
  struct Record
  {
    NotificationObserver* obs;
    int refCount;
  };

public:
  static ObserverRegister& get()
  {
    static ObserverRegister reg;
    return reg;
  }
  void registerObserver(NotificationObserver* obs)
  {
    obs->m_id = int(m_map.size());
    Record rec;
    rec.obs = obs;
    rec.refCount = 0;
    m_map[obs->id()] = rec;
  }
  void deregisterObserver(NotificationObserver* obs)
  {
    std::map<int,Record>::iterator it = m_map.find(obs->id());
    if (it == m_map.end()) return;
    Record& rec = it->second;
    if (rec.refCount <= 0)
    {
      m_map.erase(it);
    }
    else
    {
      rec.obs = NULL;
    }
  }

  NotificationObserver* getObserver(int id)
  {
    std::map<int,Record>::iterator it = m_map.find(id);
    if (it == m_map.end()) return NULL;
    return it->second.obs;
  }

  /**
    * @return False if observer does not exist
    */
  bool addRef(int id)
  {
    std::map<int,Record>::iterator it = m_map.find(id);
    if (it == m_map.end()) return false;
    it->second.refCount++;
    return true;
  }
  void deleteRef(int id)
  {
    std::map<int,Record>::iterator it = m_map.find(id);
    if (it == m_map.end()) return;
    it->second.refCount--;
    if (it->second.refCount <= 0)
    {
      if (it->second.obs == NULL)
      {
        m_map.erase(it);
      }
      else
      {
        it->second.refCount = 0;
      }
    }
  }

private:
  std::map<int,Record> m_map;
};

NotificationObserver::NotificationObserver()
{
  ObserverRegister::get().registerObserver(this);
}

NotificationObserver::~NotificationObserver()
{
  ObserverRegister::get().deregisterObserver(this);
}

void NotificationCenter::addObserver(NotificationObserver* obs)
{
  if (ObserverRegister::get().addRef(obs->id()))
  {
    m_observers.push_back(obs->id());
  }
}

void NotificationCenter::removeObserver(NotificationObserver *obs)
{
  removeObserverID(obs->id());
}

void NotificationCenter::removeObserverID(int id)
{
  ObserverRegister::get().deleteRef(id);
  std::vector<int>::iterator it = std::find(m_observers.begin(),m_observers.end(),id);
  if (it != m_observers.end())
  {
    m_observers.erase(it);
  }
}

void NotificationCenter::send(Notification *nt)
{
  std::vector<int>::iterator it = m_observers.begin();
  for(; it != m_observers.end(); ++it)
  {
    NotificationObserver* obs = ObserverRegister::get().getObserver(*it);
    if (obs)
    {
      obs->callHandler(boost::shared_ptr<Notification>(nt));
    }
    else
    {
      removeObserverID(*it);
    }
  }
}

} // Kernel
