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
  QObject::connect(this->dial_j1, SIGNAL(sliderPressed()),
      asyncrobot_, SLOT(disableJointSignals()));    
  QObject::connect(this->dial_j1, SIGNAL(sliderPressed()),
      asyncrobot_, SLOT(acquireJointControl()));    
  QObject::connect(this->dial_j1, SIGNAL(sliderReleased()),
      asyncrobot_, SLOT(enableJointSignals()));    
  QObject::connect(this->dial_j1, SIGNAL(sliderReleased()),
      asyncrobot_, SLOT(releaseJointControl()));    

  QObject::connect(this->dial_j2, SIGNAL(valueChanged(int)),
      this, SLOT(driveJoint2To(int)));    
  QObject::connect(this->dial_j2, SIGNAL(sliderPressed()),
      asyncrobot_, SLOT(disableJointSignals()));    
  QObject::connect(this->dial_j2, SIGNAL(sliderPressed()),
      asyncrobot_, SLOT(acquireJointControl()));    
  QObject::connect(this->dial_j2, SIGNAL(sliderReleased()),
      asyncrobot_, SLOT(enableJointSignals()));    
  QObject::connect(this->dial_j2, SIGNAL(sliderReleased()),
      asyncrobot_, SLOT(releaseJointControl()));    

  QObject::connect(asyncrobot_, SIGNAL(joint1Changed(int)),
      this->dial_j1, SLOT(setValue(int)));
  QObject::connect(asyncrobot_, SIGNAL(joint2Changed(int)),
      this->dial_j2, SLOT(setValue(int)));

  QObject::connect(this->checkBox_enable, SIGNAL(stateChanged(int)), 
      this, SLOT(enable(int)));
  QObject::connect(this->checkBox_enable, SIGNAL(stateChanged(int)),
      asyncrobot_, SLOT(setState(int)));
}
 
void ControlPanelForm::driveJoint1To(int angle)
{
  /* Negative angles because the directionality of the Qt dials is opposite of
   * our joint directions. */
  asyncrobot_->driveJointTo(1, -angle);
}

void ControlPanelForm::driveJoint2To(int angle)
{
  asyncrobot_->driveJointTo(2, -angle);
  asyncrobot_->driveJointTo(3, -angle);
}

void ControlPanelForm::enable(int state)
{
  enabled_ = state;
  bool enable;
  if(state) {
    enable = true;
  } else {
    enable = false;
  }
  this->groupBox->setEnabled(enable);
  this->groupBox_2->setEnabled(enable);
  this->groupBox_3->setEnabled(enable);
  this->groupBox_4->setEnabled(enable);
  this->groupBox_5->setEnabled(enable);
}

void ControlPanelForm::setActiveRobot(int index)
{
  /* Get the mobot object */
  qDebug() << index << " selected as active robot in control panel.";
  RecordMobot *mobot;
  mobot = robotManager()->getMobotIndex(index);
  if(mobot != NULL) {
    qDebug() << "Starting robot control thread!";
    asyncrobot_->bindMobot(mobot);
    asyncrobot_->enableJointSignals(true);
    QMetaObject::invokeMethod(asyncrobot_, "startWork", Qt::QueuedConnection);
  }
}

void ControlPanelForm::setActiveRobot(const QModelIndex &index)
{
  setActiveRobot(index.row());
}
