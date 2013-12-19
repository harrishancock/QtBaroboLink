#ifndef ASYNCMOBOT_H_
#define ASYNCMOBOT_H_

#include <QObject>
#include <QMutex>
#include <linkbot.h>

class AsyncMobot:public QObject
{
  Q_OBJECT
  public:
    AsyncMobot();
    ~AsyncMobot();
    void bindMobot(CLinkbot* mobot);
    void enableJointSignals(bool enable);
    void enableAccelSignals(bool enable);

  public slots:
    void driveJointTo(int joint, double angle);
    void doWork(); // Worker thread

  signals:
    void jointAnglesChanged(double angle1, double angle2, double angle3);
    void accelChanged(double x, double y, double z);

  private:
    CLinkbot* mobot_;
    bool jointSignalEnable_;
    bool accelSignalEnable_;
    QMutex mobotLock_; // Protects 3 above members

    double lastJointAngles_[3];
    double lastAccel_[3];
    double desiredJointAngles_[3];
    unsigned int anglesDirtyMask_;
    QMutex desiredJointAnglesLock_;
};

#endif
