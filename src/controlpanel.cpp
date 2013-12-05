#include "controlpanel.h"
#include <QDebug>

ControlThread::ControlThread(QObject *parent):QThread(parent)
{
}

void ControlThread::run()
{
}

void ControlThread::setActiveRobot(int index)
{
}

void ControlThread::driveJoint1To(int degrees)
{
  qDebug() << "Drive joint 1 to " << degrees;
}

void ControlThread::driveJoint2To(int degrees)
{
  qDebug() << "Drive joint 2 to " << degrees;
}

ControlPanelForm::ControlPanelForm(QWidget *parent)
  : QWidget(parent)
{
  setupUi(this);
  _workerThread = new ControlThread();

  QObject::connect(this->dial_j1, SIGNAL(valueChanged(int)),
      _workerThread, SLOT(driveJoint1To(int)));    
  QObject::connect(this->dial_j2, SIGNAL(valueChanged(int)),
      _workerThread, SLOT(driveJoint2To(int)));    
}
    

void ControlPanelForm::setActiveRobot(int index){
}
