#include "scanlist.h"
#include <QDebug>

BLORB::BLORB(QObject *parent) 
  :QAbstractTableModel(parent)
{
  qDebug() << "Fuck this goddam shit.";
}

BLORB::~BLORB()
{
  qDebug() << "Again: wtf?";
}

int BLORB::rowCount(const QModelIndex & ) const
{
  return scannedIds_.size();
}

int BLORB::columnCount(const QModelIndex &) const
{
  return 1;
}

QVariant BLORB::data(const QModelIndex & index, int role) const
{
  qDebug() << "data";
  switch(role) {
    case Qt::DisplayRole:
      return scannedIds_[index.row()];
      break;
    default:
      return QVariant();
  }
}

void BLORB::newRobot(const QString & serialID)
{
  qDebug() << "Adding new robot: " << serialID;
  emit layoutAboutToBeChanged();
  scannedIds_.prepend(QString(serialID));
  qSort(scannedIds_);
  //emit dataChanged(createIndex(0, 0), createIndex(scannedIds_.size(), 0));
  emit layoutChanged();
}

void BLORB::clearAll(void)
{
  scannedIds_.clear();
  emit layoutChanged();
}
