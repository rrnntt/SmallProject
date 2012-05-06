#ifndef WEBGUI_WEBPLUGINFACTORY_H
#define WEBGUI_WEBPLUGINFACTORY_H

#include "DataLoader.h"

#include <QtWebKit/QWebPluginFactory>
#include <QtCore/QList>

//---------------------------------------
//  Forward declarations
//---------------------------------------
class QNetworkAccessManager;

namespace QtAPI
{
  class Table;
  class Plot;
}

/**
 * Factory for creating plugins.
 */
class WebPluginFactory: public QWebPluginFactory
{
  Q_OBJECT
public:
  WebPluginFactory( QObject* parent = NULL );
  ~WebPluginFactory();
  QObject *	create ( const QString & mimeType, const QUrl & url, const QStringList & argumentNames, const QStringList & argumentValues ) const ;
  QList<Plugin>	plugins () const ;
protected slots:
  /// Dispose of a used loader
  void finishedLoader(DataLoader*);
protected:
  /// Add a loader to the list
  void addLoader(DataLoader* loader) const;
  /// Create a Table plugin
  QtAPI::Table* createTable(const QUrl & url, const QStringList & argumentNames, const QStringList & argumentValues) const;
  /// Return desription of the Table plugin;
  Plugin tablePlugin() const;
  /// Create a Plot plugin
  QtAPI::Plot* createPlot(const QUrl & url, const QStringList & argumentNames, const QStringList & argumentValues) const;
  /// Return desription of the Plot plugin;
  Plugin plotPlugin() const;

  /// Manager for downloading data form network
  QNetworkAccessManager* m_manager;
  /// List of active DataLoaders
  mutable QList<DataLoader*> m_loaders;
};

#endif // WEBGUI_WEBPLUGINFACTORY_H
