#ifndef SCANLIST_H_
#define SCANLIST_H_

#include <QAbstractListModel>

class ScanList : public QAbstractListModel {
  public:
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

  public slots:
    void newRobot(const QString & serialID);
    void clearAll(void);

  private:
    QList<QString> scannedIds_;
};

#endif
