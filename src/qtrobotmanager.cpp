#include "qtrobotmanager.h"
#include <QVariant>
#include <QDebug>
#include <QMenu>
#include <QCursor>

QtRobotManager::QtRobotManager(QObject *parent)
  :QAbstractTableModel(parent)
{
}

QtRobotManager::~QtRobotManager()
{
}

int QtRobotManager::rowCount(const QModelIndex & ) const
{
  return numEntries();
  return 0;
}

int QtRobotManager::columnCount(const QModelIndex & ) const
{
  return 2;
}

QVariant QtRobotManager::data(const QModelIndex &index, int role) const
{
  if(role == Qt::DisplayRole) {
    if(index.column() == 0) {
      if(isConnected(index.row())) {
        return QIcon(":/images/orb_green.png");
      } else {
        return QIcon(":/images/orb_red.png");
      }
    } else if (index.column() == 1) {
      return QString(getEntry(index.row()));
    }
  }
  if(role == Qt::DecorationRole) {
    if(index.column() == 0) {
      if(isConnected(index.row())) {
        return QIcon(":/images/orb_green.png");
      } else {
        return QIcon(":/images/orb_red.png");
      }
    } 
  }
  if(role == Qt::ToolTipRole) {
    if(isConnected(index.row())) {
      return QString("Robot Connected.");
    } else {
      return QString("Robot Not Connected.");
    }
  }
  return QVariant();
}

void QtRobotManager::displayContextMenu(const QPoint &)
{
  qDebug() << "bonk.\n";
  QMenu menu;
  QAction *connectaction = menu.addAction("Connect");
  QAction *disconnectaction = menu.addAction("Disconnect");
  QAction *removeaction = menu.addAction("Remove");
  if(isConnected(_activeIndex)) {
    connectaction->setEnabled(false);
  } else {
    disconnectaction->setEnabled(false);
  }
  QObject::connect(connectaction, SIGNAL(triggered()), this, SLOT(connectActiveIndex()));
  QObject::connect(disconnectaction, SIGNAL(triggered()), this, SLOT(disconnectActiveIndex()));
  QObject::connect(removeaction, SIGNAL(triggered()), this, SLOT(removeActiveIndex()));
  qDebug() << menu.exec(QCursor::pos());
}

void QtRobotManager::clicked(const QModelIndex &)
{
  qDebug() << "click.";
}

void QtRobotManager::setActiveIndex(const QModelIndex &index)
{
  _activeIndex = index.row();
  qDebug() << _activeIndex << " selected as active index.";
}

void QtRobotManager::connectActiveIndex()
{
  qDebug() << "Connect to index " << _activeIndex;
  connectIndex(_activeIndex);
  emit dataChanged(createIndex(_activeIndex, 0), createIndex(_activeIndex, 1));
}

void QtRobotManager::disconnectActiveIndex()
{
  qDebug() << "Disconnect from index " << _activeIndex;
  CRobotManager::disconnect(_activeIndex);
}

void QtRobotManager::removeActiveIndex()
{
  disconnectActiveIndex();
  ConfigFile::remove(_activeIndex);
  ConfigFile::write();
  emit dataChanged(createIndex(_activeIndex, 0), createIndex(_activeIndex, 1));
}

void QtRobotManager::addEntry(QString entry)
{
  ConfigFile::addEntry(entry.toStdString());
  ConfigFile::write();
  emit dataChanged(createIndex(_activeIndex, 0), createIndex(_activeIndex, 1));
}

QtRobotManager* robotManager()
{
  static QtRobotManager *instance = 0;
  if(0 == instance) {
    instance = new QtRobotManager(0);
    instance->read( Mobot_getConfigFilePath() );
  }
  return instance;
}
