#include "qtrobotmanager.h"
#include <QCursor>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QVariant>

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
}

int QtRobotManager::columnCount(const QModelIndex & ) const
{
  return 2;
}

QVariant QtRobotManager::data(const QModelIndex &index, int role) const
{
  if(role == Qt::DisplayRole) {
    if(index.column() == 0) {
      RecordMobot* mobot;
      mobot = robotManager()->getMobotIndex(index.row());
      if(mobot == NULL) {
        return QIcon(":/images/orb_red.png");
      }
      switch(mobot->connectStatus()) {
        case RMOBOT_NOT_CONNECTED:
          return QIcon(":/images/orb_red.png");
          break;
        case RMOBOT_CONNECTING:
          return QIcon(":/images/orb_gray.png");
          break;
        case RMOBOT_CONNECTED:
          return QIcon(":/images/orb_green.png");
          break;
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
  /*
  if(role == Qt::ForegroundRole) {
    if(index.column() == 1) {
      return QBrush(QColor("gray"));
    }
  }
  */
  return QVariant();
}

void QtRobotManager::displayContextMenu(const QPoint &)
{
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
  menu.exec(QCursor::pos());
}

void QtRobotManager::clicked(const QModelIndex &)
{
  qDebug() << "click.";
}

void QtRobotManager::setActiveIndex(const QModelIndex &index)
{
  _activeIndex = index.row();
  qDebug() << _activeIndex << " selected as active index in QtRobotManager.";
}

int QtRobotManager::disconnectIndex(int index)
{
  return CRobotManager::disconnect(index);
}

void QtRobotManager::connectActiveIndex()
{
  qDebug() << "Connect to index " << _activeIndex;
  int rc = connectIndex(_activeIndex);
  if(rc) {
    QMessageBox box;
    box.setText("Could not connect to robot. Please make sure there is a dongle plugged in and the robot is on.");
    box.exec();
  }
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
  emit layoutChanged();
}

void QtRobotManager::addEntry(QString entry)
{
  ConfigFile::addEntry(entry.toStdString());
  ConfigFile::write();
  emit layoutChanged();
}

void QtRobotManager::toggleConnection(const QModelIndex &index)
{
  qDebug() << "toggle connection.";
  if(isConnected(index.row())) {
    QtRobotManager::disconnectIndex(index.row());
  } else {
    RobotConnectWorker *w = new RobotConnectWorker(this, index.row());
    int rc = connectIndex(index.row());
    if(rc) {
      QMessageBox box;
      box.setText("Could not connect to robot. Please make sure there is a dongle plugged in and the robot is on.");
      box.exec();
    }
  }
  emit layoutChanged();
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

RobotConnectWorker::RobotConnectWorker(QObject *parent, int connectIndex)
  :QObject(parent),connectIndex_(connectIndex)
{
}

RobotConnectWorker::~RobotConnectWorker()
{
}

void RobotConnectWorker::run()
{
  int rc;
  emit connectInitiated();
  rc = robotManager()->connectIndex(connectIndex_);
  if(rc) emit connectFailed();
  else emit connectSuccess();
}

