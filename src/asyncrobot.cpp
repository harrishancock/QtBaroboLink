#include "asyncrobot.h"
#include <QThread>
#include <QDebug>

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
  qDebug() << "Moop!";
  enableJointSignals(false);
}

void AsyncRobot::disableAccelSignals()
{
  enableAccelSignals(false);
}

void AsyncRobot::driveJointTo(int joint, double angle)
{
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
          qDebug() << "Joint 1 changed. Was"<<lastJointAngles_[0]<<"now"<<jointAngles[0];
          emit joint1Changed(((int)jointAngles[0])%360);
        }
        if(jointAngles[1] != lastJointAngles_[1]) {
          emit joint2Changed(jointAngles[1]);
        }
        if(jointAngles[2] != lastJointAngles_[2]) {
          emit joint2Changed(jointAngles[2]);
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
    desiredJointAnglesLock_.unlock();

    mobotLock_.unlock();
    QThread::yieldCurrentThread();
  }
}
