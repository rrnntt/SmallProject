#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "DllExport.h"
#include <boost/shared_ptr.hpp>

#include <string>
#include <map>
#include <vector>
#include <typeinfo>
#include <iostream>

namespace Kernel
{

class KERNEL_EXPORT Notification
{
public:
  Notification(){}
  virtual ~Notification(){}
};

class ObserverHandlerBase
{
public:
  virtual void call(const Notification&) = 0;
};

class KERNEL_EXPORT NotificationObserver
{
public:
  NotificationObserver();
  virtual ~NotificationObserver();
  template <class Observer,class NotificationClass>
      void setHandler(Observer* observer,void (Observer::*)(const NotificationClass&));
  virtual void callHandler(boost::shared_ptr<const Notification> nt)
  {
    ObserverHandlerBase* h = m_handlers[std::string(typeid(*nt).name())];
    if (h)
    {
      h->call(*nt);
    }
  }
  int id()const{return m_id;}
private:
  friend class ObserverRegister;
  int m_id;
  //typedef std::map<const std::type_info*, ObserverHandlerBase*> HandlerMap;
  typedef std::map<std::string, ObserverHandlerBase*> HandlerMap;
  HandlerMap m_handlers;
};

class KERNEL_EXPORT NotificationCenter
{
public:
  void addObserver(NotificationObserver*);
  void removeObserver(NotificationObserver*);
  void removeObserverID(int);
  void send(Notification* nt);
private:
  std::vector<int> m_observers;
};

template<class Observer,class NotificationClass>
class ObserverHandler: public ObserverHandlerBase
{
  Observer* m_observer;
  void (Observer::*m_handler)(const NotificationClass&);
public:
  ObserverHandler(Observer* observer,void (Observer::*handler)(const NotificationClass&)):
      m_observer(observer),m_handler(handler){}
  void call(const Notification& nt)
  {
    (m_observer->*m_handler)(dynamic_cast<const NotificationClass&>(nt));
  }
};

// ------ Implementations ------ //

template <class Observer,class NotificationClass>
void NotificationObserver::setHandler(Observer* observer,void (Observer::*handler)(const NotificationClass&))
{
  m_handlers[typeid(NotificationClass).name()] = new ObserverHandler<Observer,NotificationClass>(observer,handler);
}

} // Kernel

#endif // NOTIFICATION_H
