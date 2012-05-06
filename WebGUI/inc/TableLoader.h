#ifndef WEBGUI_TABLELOADER_H
#define WEBGUI_TABLELOADER_H

#include "DataLoader.h"

#include "DataObjects/TableWorkspace.h"

class TableLoader: public DataLoader
{
public:
  TableLoader(QNetworkAccessManager* manager, const QUrl& url, DataObjects::TableWorkspace_ptr ws);
  ~TableLoader();
protected:
  /// Do the loading
  void load( const QByteArray& data);
  /// The workspace to fill the data in
  DataObjects::TableWorkspace_ptr m_workspace;
};

#endif // WEBGUI_TABLELOADER_H
