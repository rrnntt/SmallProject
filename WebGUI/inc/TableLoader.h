#ifndef WEBGUI_TABLELOADER_H
#define WEBGUI_TABLELOADER_H

#include "DataLoader.h"

#include "API/TableWorkspace.h"

class TableLoader: public DataLoader
{
public:
  TableLoader(QNetworkAccessManager* manager, const QUrl& url, API::TableWorkspace_ptr ws);
  ~TableLoader();
protected:
  /// Do the loading
  void load( const QByteArray& data);
  /// The workspace to fill the data in
  API::TableWorkspace_ptr m_workspace;
};

#endif // WEBGUI_TABLELOADER_H
