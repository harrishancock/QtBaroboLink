#include "scanlist.h"
#include <QDebug>

ScanList::ScanList(QObject *parent) 
  :QAbstractTableModel(parent)
{
  qDebug() << "Fuck this goddam shit.";
}

ScanList::~ScanList()
{
  qDebug() << "Again: wtf?";
}

int ScanList::rowCount(const QModelIndex & ) const
{
  return scannedIds_.size();
}

int ScanList::columnCount(const QModelIndex &) const
{
  return 1;
}

QVariant ScanList::data(const QModelIndex & index, int role) const
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

void ScanList::newRobot(const QString & serialID)
{
  qDebug() << "Adding new robot: " << serialID;
  emit layoutAboutToBeChanged();
  scannedIds_.prepend(QString(serialID));
  qSort(scannedIds_);
  //emit dataChanged(createIndex(0, 0), createIndex(scannedIds_.size(), 0));
  emit layoutChanged();
}

void ScanList::clearAll(void)
{
  scannedIds_.clear();
  emit layoutChanged();
}
