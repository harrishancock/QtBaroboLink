#include "scanlist.h"

int ScanList::rowCount(const QModelIndex & parent) const
{
  return scannedIds_.size();
}

QVariant ScanList::data(const QModelIndex & index, int role) const
{
  switch(role) {
    case Qt::DisplayRole:
      return scannedIds[index.row()];
      break;
    default:
      return QVariant();
  }
}

void ScanList::newRobot(const QString & serialID)
{
  scannedIds_.prepend(serialID);
  qSort(scannedIds_);
  emit layoutChanged();
}
