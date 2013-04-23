#ifndef QTAPI_ALGORITHMINPUTHISTORY_H
#define QTAPI_ALGORITHMINPUTHISTORY_H

#include "QtAPI/DllExport.h"
#include "Kernel/Singleton.h"

#include <QString>

namespace QtAPI
{

class QTAPI_EXPORT AlgorithmInputHistory: protected Kernel::Singleton
{
public:
  virtual std::string name() const {return "AlgorithmInputHistory";}
  static AlgorithmInputHistory& instance();
  /// Return saved property value
  QString getPropertyValue(const std::string& algName, const std::string& propName) const;
  /// Return saved property value
  QString getPropertyValue(const QString& algName, const QString& propName) const;
  /// Save property value
  void savePropertyValue(const std::string& algName, const std::string& propName, const std::string& propValue) const;
  /// Save property value
  void savePropertyValue(const QString& algName, const QString& propName, const QString& propValue) const;
protected:
  AlgorithmInputHistory();
};

} // QTAPI

#endif // QTAPI_ALGORITHMINPUTHISTORY_H
