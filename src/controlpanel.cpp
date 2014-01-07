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
  QObject::connect(asyncrobot_, SIGNAL(joint1Changed(double)),
      this, SLOT(setJ1Label(double)));
  QObject::connect(asyncrobot_, SIGNAL(joint2Changed(int)),
      this->dial_j2, SLOT(setValue(int)));
  QObject::connect(asyncrobot_, SIGNAL(joint2Changed(double)),
      this, SLOT(setJ2Label(double)));

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
  setJ1Label(-angle);
}

void ControlPanelForm::driveJoint2To(int angle)
{
  asyncrobot_->driveJointTo(2, -angle);
  asyncrobot_->driveJointTo(3, -angle);
  setJ2Label(-angle);
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

void ControlPanelForm::setJ1Label(int value)
{
  setJ1Label(QString("%1").arg(value));
}

void ControlPanelForm::setJ2Label(int value)
{
  setJ2Label(QString("%1").arg(value));
}

void ControlPanelForm::setJ1Label(double value)
{
  setJ1Label(QString("%1").arg(value, 0, 'f', 1));
}

void ControlPanelForm::setJ2Label(double value)
{
  setJ2Label(QString("%1").arg(value, 0, 'f', 1));
}

void ControlPanelForm::setJ1Label(const QString &value)
{
  this->label_j1_angle->setText(value);  
}

void ControlPanelForm::setJ2Label(const QString &value)
{
  this->label_j2_angle->setText(value);  
}

void ControlPanelForm::setActiveRobot(int index)
{
  /* Get the mobot object */
  qDebug() << index << " selected as active robot in control panel.";
  RecordMobot *mobot;
  mobot = robotManager()->getMobotIndex(index);
  if(mobot != NULL) {
    qDebug() << "Starting robot control thread!";
    this->setEnabled(true);
    asyncrobot_->bindMobot(mobot);
    asyncrobot_->enableJointSignals(true);
    QMetaObject::invokeMethod(asyncrobot_, "startWork", Qt::QueuedConnection);
  } else {
    /* Stop the thread if it is running */
    asyncrobot_->stopWork();
    this->setEnabled(false);
  }
}

void ControlPanelForm::setActiveRobot(const QModelIndex &index)
{
  setActiveRobot(index.row());
}
