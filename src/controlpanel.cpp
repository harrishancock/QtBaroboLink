#include "controlpanel.h"
#include <QDebug>
#include <QThread>
#include <QModelIndex>
#include <math.h>
#include "asyncrobot.h"
#include "recordmobot.h"
#include "qtrobotmanager.h"

ControlPanelForm::ControlPanelForm(QWidget *parent)
  : QWidget(parent)
{
  setupUi(this);
  asyncrobot_ = new AsyncRobot();
  mobotthread_ = new QThread();
  asyncrobot_->moveToThread(mobotthread_);
  mobotthread_->start();

  QObject::connect(this->dial_j1, SIGNAL(valueChanged(int)),
      this, SLOT(driveJoint1To(int)));    
  QObject::connect(this->dial_j2, SIGNAL(valueChanged(int)),
      this, SLOT(driveJoint2To(int)));    
}
 
#define DEG2RAD(x) ((x)*M_PI/180.0)
void ControlPanelForm::driveJoint1To(int angle)
{
  asyncrobot_->driveJointTo(1, DEG2RAD(angle));
}
void ControlPanelForm::driveJoint2To(int angle)
{
  asyncrobot_->driveJointTo(1, DEG2RAD(angle));
}
#undef DEG2RAD

void ControlPanelForm::setActiveRobot(int index)
{
  /* Get the mobot object */
  RecordMobot *mobot;
  mobot = robotManager()->getMobotIndex(index);
  if(mobot != NULL) {
    asyncrobot_->bindMobot(mobot);
    asyncrobot_->enableJointSignals(true);
    QMetaObject::invokeMethod(asyncrobot_, "doWork", Qt::QueuedConnection);
  }
}

void ControlPanelForm::setActiveRobot(const QModelIndex &index)
{
  setActiveRobot(index.row());
}
