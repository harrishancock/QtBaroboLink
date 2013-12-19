#ifndef _ASYNCMOBOT_H_
#define _ASYNCMOBOT_H_

#include <QObject>
#include <QMutex>
#include <mobot.h>

class AsyncMobot:public QObject
{
  Q_OBJECT
  public:
    AsyncMobot();
    ~AsyncMobot();
    void bindMobot(mobot_t* mobot);
    void enableJointSignals(bool enable);
    void enableAccelSignals(bool enable);

  public slots:
    void driveJointTo(int joint, double angle);

  signals:
    void jointAnglesChanged(double angle1, double angle2, double angle3, double angle4);
    void accelChanged(double x, double y, double z);

  protected:
    void doWork(); // Worker thread

  private:
    mobot_t* _mobot;
    bool _jointSignalEnable;
    bool _accelSignalEnable;
    QMutex _mobotLock; // Protects 3 above members

    double _lastJointAngles[4];
    double _lastAccel[3];
    double _desiredJointAngles[4];
    unsigned int _anglesDirtyMask;
    QMutex _desiredJointAnglesLock;
};

#endif
