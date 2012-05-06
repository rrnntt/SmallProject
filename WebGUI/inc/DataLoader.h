#ifndef WEBGUI_DATALOADER_H
#define WEBGUI_DATALOADER_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QByteArray>
#include <QtNetwork/QNetworkReply>

class QNetworkAccessManager;

class DataLoader: public QObject
{
  Q_OBJECT
public:
  DataLoader(QNetworkAccessManager* manager, const QUrl& url);
  ~DataLoader();
signals:
  void finished(DataLoader*);
protected slots:
  /// Called when some data were read
  void slotReadyRead();
  void slotError(QNetworkReply::NetworkError);
protected:
  /// Do the loading
  virtual void load( const QByteArray& data) = 0;

  /// Manager for downloading data form network
  QNetworkAccessManager* m_manager;
  /// Object that receives the data
  QNetworkReply* m_reply;
};

#endif // WEBGUI_DATALOADER_H
