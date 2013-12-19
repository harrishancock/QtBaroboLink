#include "asyncmobot.h"
#include <QThread>

AsyncMobot::AsyncMobot() 
{
  _jointSignalEnable = false;
  _accelSignalEnable = false;
}

AsyncMobot::~AsyncMobot() {}

void AsyncMobot::bindMobot(mobot_t* mobot)
{
  _mobotLock.lock();
  _mobot = mobot;
  _mobotLock.unlock();
}

void AsyncMobot::enableJointSignals(bool enable)
{
  _mobotLock.lock();
  _jointSignalEnable = enable;
  _mobotLock.unlock();
}

void AsyncMobot::enableAccelSignals(bool enable)
{
  _mobotLock.lock();
  _accelSignalEnable = enable;
  _mobotLock.unlock();
}

void AsyncMobot::driveJointTo(int joint, double angle)
{
  _desiredJointAnglesLock.lock();
  _desiredJointAngles[joint-1] = angle;
  _anglesDirtyMask |= 1<<(joint-1);
  _desiredJointAnglesLock.unlock();
}

void AsyncMobot::doWork()
{
  double jointAngles[4];
  double accel[3];
  while(1) {
    _mobotLock.lock();
    if(_mobot == NULL) {
      break;
    }
    if(_jointSignalEnable) {
      Mobot_getJointAngles(
          _mobot, 
          &jointAngles[0], 
          &jointAngles[1], 
          &jointAngles[2], 
          &jointAngles[3]);
      if(memcmp(jointAngles, _lastJointAngles, sizeof(double)*4)) {
        emit jointAnglesChanged(jointAngles[0], jointAngles[1], jointAngles[2], jointAngles[3]);
        memcpy(_lastJointAngles, jointAngles, sizeof(double)*4);
      }
    }
    if(_accelSignalEnable) {
      Mobot_getAccelerometerData(_mobot, &accel[0], &accel[1], &accel[2]);
      if(memcmp(accel, _lastAccel, sizeof(double)*3)) {
        emit accelChanged(accel[0], accel[1], accel[2]);
        memcpy(_lastAccel, accel, sizeof(double)*3);
      }
    }

    _desiredJointAnglesLock.lock();
    int i;
    for(i = 0; i < 4; i++) {
      if(_anglesDirtyMask & (1<<i)) {
        Mobot_driveJointToDirectNB(_mobot, (robotJointId_t)(i+1), _desiredJointAngles[i]);
      }
    }
    _desiredJointAnglesLock.unlock();

    _mobotLock.unlock();
    QThread::yieldCurrentThread();
  }
}
