#ifndef ALGORITHMDIALOG_H
#define ALGORITHMDIALOG_H

#include <QDialog>

#include <boost/shared_ptr.hpp>

namespace API
{
  class Algorithm;
}

class AlgorithmDialog: public QDialog
{
    Q_OBJECT
public:
    AlgorithmDialog(QWidget *parent,const QString& algName);

protected:

    boost::shared_ptr<API::Algorithm> m_algorithm;

};

#endif // ALGORITHMDIALOG_H
