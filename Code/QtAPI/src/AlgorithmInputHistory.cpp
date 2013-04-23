#include "QtAPI/AlgorithmInputHistory.h"
#include "Kernel/Framework.h"

#include <QSettings>

namespace QtAPI
{

/**
 * Constructor.
 */
AlgorithmInputHistory::AlgorithmInputHistory()
{
}

/**
 * Return reference to the instance of the singleton. On first call the singleton 
 * is registered with Kernel::Framework.
 */
AlgorithmInputHistory& AlgorithmInputHistory::instance()
{
  Kernel::Singleton* s = Kernel::Framework::instance().getSingleton("AlgorithmInputHistory");
  if (s == nullptr)
  {
    AlgorithmInputHistory *f = new AlgorithmInputHistory();
    Kernel::Framework::instance().registerSingleton("AlgorithmInputHistory",f);
    return *f;
  }
  else
  {
    return *static_cast<AlgorithmInputHistory*>(s);
  }
}

/**
 * Return saved property value.
 * @param algName :: The algorithm name.
 * @param propName :: The property name.
 */
QString AlgorithmInputHistory::getPropertyValue(const QString& algName, const QString& propName) const
{
  QSettings settings;
  return settings.value("Algorithms/"+algName+"/"+propName, "").toString();
}

/**
 * Return saved property value.
 * @param algName :: The algorithm name.
 * @param propName :: The property name.
 */
QString AlgorithmInputHistory::getPropertyValue(const std::string& algName, const std::string& propName) const
{
  return getPropertyValue( QString::fromStdString(algName), QString::fromStdString(propName) );
}

/**
 * Save property value.
 * @param algName :: The algorithm name.
 * @param propName :: The property name.
 * @param propValue :: A value to save.
 */
void AlgorithmInputHistory::savePropertyValue(const QString& algName, const QString& propName, const QString& propValue) const
{
  QSettings settings;
  settings.setValue("Algorithms/"+algName+"/"+propName, propValue);
}

/**
 * Save property value.
 * @param algName :: The algorithm name.
 * @param propName :: The property name.
 * @param propValue :: A value to save.
 */
void AlgorithmInputHistory::savePropertyValue(const std::string& algName, const std::string& propName, const std::string& propValue) const
{
  savePropertyValue( QString::fromStdString(algName), QString::fromStdString(propName), QString::fromStdString(propValue) );
}

} // QtAPI

