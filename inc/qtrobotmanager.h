#ifndef _QTROBOTMANAGER_H_
#define _QTROBOTMANAGER_H_

#include <QStyle>
#include <QList>
#include <QWidget>
#include <QAbstractTableModel>
#include <QPoint>
#include "robotmanager.h"

/* Note: No need to instantiate this class explicitly. Use the robotManager()
 * access function. */
class QtRobotManager : public QAbstractTableModel, public CRobotManager
{
  Q_OBJECT
  public:
    QtRobotManager(QObject *parent);
    ~QtRobotManager();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  public slots:
    void displayContextMenu(const QPoint &p);
    inline void setActiveIndex(int index) {_activeIndex = index;}
    void setActiveIndex(const QModelIndex &index);
    void clicked(const QModelIndex &index);
    void connectActiveIndex();
    void disconnectActiveIndex();
    void removeActiveIndex();

  private:
    int _activeIndex;
};

QtRobotManager* robotManager();

#endif
