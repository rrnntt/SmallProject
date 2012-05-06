#include "DataLoader.h"

#include <QtNetwork/QNetworkAccessManager>

#include <iostream>

/**
 * Constructor.
 */
DataLoader::DataLoader(QNetworkAccessManager* manager, const QUrl& url):
m_manager(manager)
{
  QNetworkRequest request;
  request.setUrl(url);
  request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

  m_reply = m_manager->get(request);
  if ( m_reply )
  {
    std::cerr << "Reply OK: " + url.toString().toStdString() << std::endl;
  }
  connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
    this, SLOT(slotError(QNetworkReply::NetworkError)));
}

/**
 * Destructor.
 */
DataLoader::~DataLoader()
{
  std::cerr << "Loader deleted\n";
}

/**
 * Called when data become available
 */
void DataLoader::slotReadyRead()
{
  auto data = m_reply->readAll();
  load( data );
  m_reply->deleteLater();
  emit finished(this);
}

void DataLoader::slotError(QNetworkReply::NetworkError err)
{
  std::cerr << "Error " + QString::number(err).toStdString() << std::endl;
}

