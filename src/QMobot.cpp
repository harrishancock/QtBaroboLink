#include "QMobot.h"
#include "recordmobot.h"
#include <QDebug>

QMobot::QMobot(QObject *parent)
  :QObject(parent)
{
}

QMobot::~QMobot()
{
}

int QMobot::connectWithAddress(const char address[], int channel)
{
  _connectStatus = RMOBOT_CONNECTING;
  emit connectStatusChanged(RMOBOT_CONNECTING);
  int rc = RecordMobot::connectWithAddress(address, channel);
  if(rc) {
    emit connectStatusChanged(RMOBOT_NOT_CONNECTED);
  } else {
    emit connectStatusChanged(RMOBOT_CONNECTED);
  }
}
