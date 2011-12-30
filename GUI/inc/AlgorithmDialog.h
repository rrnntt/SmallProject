#ifndef ALGORITHMDIALOG_H
#define ALGORITHMDIALOG_H

#include <QDialog>
#include <QMap>

#include <boost/shared_ptr.hpp>

class QLineEdit;

namespace API
{
  class Algorithm;
}

class AlgorithmDialog: public QDialog
{
    Q_OBJECT
public:
  AlgorithmDialog(QWidget *parent,const std::string& algName);
  boost::shared_ptr<API::Algorithm> getAlgorithm() {return m_algorithm;}
protected slots:
  void accept();
protected:

    boost::shared_ptr<API::Algorithm> m_algorithm;
    QMap<std::string,QLineEdit*> m_propertyMap;

};

#endif // ALGORITHMDIALOG_H
