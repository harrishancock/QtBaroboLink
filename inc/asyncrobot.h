#ifndef ASYNCMOBOT_H_
#define ASYNCMOBOT_H_

#include <QObject>
#include <QMutex>
#include <QTimer>
#include <linkbot.h>

class AsyncRobot:public QObject
{
  Q_OBJECT
  public:
    AsyncRobot();
    ~AsyncRobot();
    void bindMobot(CLinkbot* mobot);

  public slots:
    void driveJointTo(int joint, double angle);
    /*
    void moveJoint(int joint, int direction);
    void stop();
    void moveForward();
    void moveBackward();
    void turnLeft();
    void turnRight();
    */
    void doWork(); // Worker thread
    void setState(int state);
    void enableJointSignals(bool enable = true);
    void enableAccelSignals(bool enable = true);
    void disableJointSignals();
    void disableAccelSignals();
    void acquireJointControl();
    void releaseJointControl();
    void startWork();
    void stopWork();

  signals:
    void jointAnglesChanged(double angle1, double angle2, double angle3);
    void joint1Changed(double angle);
    void joint2Changed(double angle);
    void joint1Changed(int angle);
    void joint2Changed(int angle);
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
    bool jointControl_;
    QTimer *timer_;
};

#endif
