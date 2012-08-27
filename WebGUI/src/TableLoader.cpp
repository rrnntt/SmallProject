#include "TableLoader.h"

#include <QtCore/QTemporaryFile>

/**
 * Constructor.
 */
TableLoader::TableLoader(QNetworkAccessManager* manager, const QUrl& url, API::TableWorkspace_ptr ws):
DataLoader(manager,url),
m_workspace(ws)
{
}

/**
 * Destructor.
 */
TableLoader::~TableLoader()
{
}

/**
 * Called when data become available
 */
void TableLoader::load( const QByteArray& data)
{
  QTemporaryFile file;
  if (file.open()) {
    // file.fileName() returns the unique file name
    file.write( data );
    file.close();
    m_workspace->loadAscii( file.fileName().toStdString() );
    m_workspace->modified();
  }
}
