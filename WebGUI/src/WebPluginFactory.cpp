#include "WebPluginFactory.h"
#include "TableLoader.h"

#include "API/WorkspaceFactory.h"
#include "API/TableWorkspace.h"
#include "QtAPI/Table.h"
#include "QtAPI/Plot.h"

#include <QtCore/QUrl>
#include <QtGui/QPushButton>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

/**
 * Constructor.
 */
WebPluginFactory::WebPluginFactory(QObject* parent):
QWebPluginFactory(parent),
m_manager(new QNetworkAccessManager)
{
}

/**
 * Destructor.
 */
WebPluginFactory::~WebPluginFactory()
{
  foreach(DataLoader* ld, m_loaders)
  {
    delete ld;
  }
}

QObject *	WebPluginFactory::create ( const QString & mimeType, const QUrl & url, const QStringList & argumentNames, const QStringList & argumentValues ) const
{
  if ( mimeType == "sm/test" )
  {
    auto label = new QPushButton("Test plugin: " + url.toString()+"\n"+argumentNames[0]+":"+argumentValues[0]);
    return label;
  }
  else if ( mimeType == "sm/table" )
  {
    return createTable(url, argumentNames, argumentValues);
  }
  else if ( mimeType == "sm/plot" )
  {
    return createPlot(url, argumentNames, argumentValues);
  }
  return nullptr;
}

QList<WebPluginFactory::Plugin>	WebPluginFactory::plugins () const
{
  Plugin plugin;
  plugin.description = "Test plugin";
  MimeType mimeType;
  mimeType.description = "Test mime type.";
  mimeType.name = "sm/test";
  QList<MimeType> mimeTypeList;
  mimeTypeList << mimeType;
  plugin.mimeTypes = mimeTypeList;

  QList<Plugin> pluginList;
  pluginList << plugin << tablePlugin() << plotPlugin();
  return pluginList;
}

/**
 * Return desription of the Table plugin;
 */
WebPluginFactory::Plugin WebPluginFactory::tablePlugin() const
{
  Plugin plugin;
  plugin.description = "Displays a Table";
  MimeType mimeType;
  mimeType.description = "Table.";
  mimeType.name = "sm/table";
  QList<MimeType> mimeTypeList;
  mimeTypeList << mimeType;
  plugin.mimeTypes = mimeTypeList;
  return plugin;
}


/**
 * Create a Table plugin.
 * @param url :: URL of a table file or empty to create a new or use in memory TableWorkspace.
 * @param argumentNames :: Object tag argument names. Arguments are: TableName, 
 * @param argumentValues :: Object tag argument values
 */
QtAPI::Table* WebPluginFactory::createTable(const QUrl & url, const QStringList & argumentNames, const QStringList & argumentValues) const
{
  QString tableName;
  if ( argumentNames.contains("TableName") )
  {
    tableName = argumentValues[argumentNames.indexOf("TableName")];
  }
  API::TableWorkspace_ptr ws( dynamic_cast<API::TableWorkspace*>(
    API::WorkspaceFactory  ::instance().create("TableWorkspace")
    ));
  if ( tableName.isEmpty() )
  {
    tableName = "Table_" + QString::number(API::WorkspaceManager::instance().size() + 1);// TODO: not robust
  }
  if ( !url.isEmpty() )
  {
    addLoader( new TableLoader(m_manager,url,ws) );
  }
  else
  {
    ws->addColumn("double","X");
    ws->addColumn("double","Y");
    ws->setRowCount(10);
  }
  API::WorkspaceManager::instance().addOrReplace(tableName.toStdString(),ws);
  QtAPI::Table* table = new QtAPI::Table(ws);
  return table;
}

/**
 * Add a loader to the list
 * @param loader :: New data loader
 */
void WebPluginFactory::addLoader(DataLoader* loader) const
{
  m_loaders.append(loader);
  connect(loader,SIGNAL(finished(DataLoader*)),this,SLOT(finishedLoader(DataLoader*)));
}

/**
 * Dispose of a used loader
 */
void WebPluginFactory::finishedLoader(DataLoader* loader)
{
  m_loaders.removeOne(loader);
  loader->deleteLater();
}

/**
 * Create a Plot plugin
 */
QtAPI::Plot* WebPluginFactory::createPlot(const QUrl & url, const QStringList & argumentNames, const QStringList & argumentValues) const
{
  auto plot = new QtAPI::Plot();
  if ( argumentNames.contains("Title") )
  {
    QString title = argumentValues[argumentNames.indexOf("Title")];
    plot->setTitle(title);
  }
  return plot;
}

/**
 * Return desription of the Plot plugin;
 */
WebPluginFactory::Plugin WebPluginFactory::plotPlugin() const
{
  Plugin plugin;
  plugin.description = "Displays a Plot";
  MimeType mimeType;
  mimeType.description = "Plot.";
  mimeType.name = "sm/plot";
  QList<MimeType> mimeTypeList;
  mimeTypeList << mimeType;
  plugin.mimeTypes = mimeTypeList;
  return plugin;
}
