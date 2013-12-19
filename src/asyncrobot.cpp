#include "asyncrobot.h"
#include <QThread>

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

void AsyncRobot::driveJointTo(int joint, double angle)
{
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
        memcpy(lastJointAngles_, jointAngles, sizeof(double)*4);
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
