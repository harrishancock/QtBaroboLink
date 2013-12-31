#ifndef _SCANLIST_H_
#define _SCANLIST_H_

#include <QAbstractTableModel>

class BLORB : public QAbstractTableModel 
{
  Q_OBJECT
  public:
    BLORB(QObject* parent);
    ~BLORB();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  public slots:
    void newRobot(const QString & serialID);
    void clearAll(void);

  private:
    QList<QString> scannedIds_;
};

#endif
