#ifndef _SCANLIST_H_
#define _SCANLIST_H_

#include <QAbstractTableModel>

class ScanList : public QAbstractTableModel 
{
  Q_OBJECT
  public:
    ScanList(QObject* parent);
    ~ScanList();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  public slots:
    void newRobot(const QString & serialID);
    void clearAll(void);

  signals:
    void bloob();

  private:
    QList<QString> scannedIds_;
};

#endif
