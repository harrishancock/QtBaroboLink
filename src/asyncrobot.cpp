#include "asyncrobot.h"
#include <QThread>
#include <QDebug>
#include <QCoreApplication>

AsyncRobot::AsyncRobot() 
{
  jointSignalEnable_ = false;
  accelSignalEnable_ = false;
}

AsyncRobot::~AsyncRobot() {}

void AsyncRobot::bindMobot(CLinkbot* mobot)
{
  mobotLock_.lock();
  mobot_ = mobot;
  mobotLock_.unlock();
}

void AsyncRobot::enableJointSignals(bool enable)
{
  qDebug() << "enable Joint Signals: " << enable;
  mobotLock_.lock();
  jointSignalEnable_ = enable;
  mobotLock_.unlock();
}

void AsyncRobot::enableAccelSignals(bool enable)
{
  mobotLock_.lock();
  accelSignalEnable_ = enable;
  mobotLock_.unlock();
}

void AsyncRobot::disableJointSignals()
{
  qDebug() << "Joint Signals Disabled.";
  enableJointSignals(false);
}

void AsyncRobot::disableAccelSignals()
{
  enableAccelSignals(false);
}

void AsyncRobot::acquireJointControl()
{
  qDebug() << "Joint control acquired.";
  jointControl_ = true;
}

void AsyncRobot::releaseJointControl()
{
  qDebug() << "Joint control released.";
  jointControl_ = false;
}

void AsyncRobot::driveJointTo(int joint, double angle)
{
  if(false == jointControl_) return;
  qDebug() << "Drive joint " << joint << " to " << angle;
  desiredJointAnglesLock_.lock();
  desiredJointAngles_[joint-1] = angle;
  anglesDirtyMask_ |= 1<<(joint-1);
  desiredJointAnglesLock_.unlock();
}

void AsyncRobot::doWork()
{
  double jointAngles[3];
  double accel[3];
  while(1) {
    mobotLock_.lock();
    if(mobot_ == NULL) {
      mobotLock_.unlock();
      break;
    }
    if(jointSignalEnable_) {
      mobot_->getJointAngles( 
          jointAngles[0], 
          jointAngles[1], 
          jointAngles[2]);
      if(memcmp(jointAngles, lastJointAngles_, sizeof(double)*3)) {
        emit jointAnglesChanged(jointAngles[0], jointAngles[1], jointAngles[2]);
        if(jointAngles[0] != lastJointAngles_[0]) {
          emit joint1Changed(((int)-jointAngles[0])%360);
        }
        if(jointAngles[1] != lastJointAngles_[1]) {
          /* Negative angles because the directionality of the Qt dials is
           * opposite of our joint directions. */
          emit joint2Changed(((int)-jointAngles[1])%360);
        }
        if(jointAngles[2] != lastJointAngles_[2]) {
          emit joint2Changed(((int)-jointAngles[2])%360);
        }
        memcpy(lastJointAngles_, jointAngles, sizeof(double)*3);
      }
    }
    if(accelSignalEnable_) {
      mobot_->getAccelerometerData(accel[0], accel[1], accel[2]);
      if(memcmp(accel, lastAccel_, sizeof(double)*3)) {
        emit accelChanged(accel[0], accel[1], accel[2]);
        memcpy(lastAccel_, accel, sizeof(double)*3);
      }
    }

    desiredJointAnglesLock_.lock();
    int i;
    for(i = 0; i < 4; i++) {
      if(anglesDirtyMask_ & (1<<i)) {
        mobot_->driveJointToDirectNB( (robotJointId_t)(i+1), desiredJointAngles_[i]);
      }
    }
    anglesDirtyMask_ = 0;
    desiredJointAnglesLock_.unlock();

    mobotLock_.unlock();
    /* Need an explicit call to processEvents() because our doWork() loop
     * rarely returns. Thus, we should process events inside the doWork() loop.
     * */
    QCoreApplication::processEvents();
    QThread::yieldCurrentThread();
  }
}
