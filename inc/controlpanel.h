#ifndef _CONTROLPANEL_H_
#define _CONTROLPANEL_H_

#include "ui_controlpanel.h"
#include <QThread>

class ControlThread : public QThread
{
  Q_OBJECT
  public:
    ControlThread(QObject *parent = 0);
    void run();

  public slots:
    void setActiveRobot(int index);
    void driveJoint1To(int degrees);
    void driveJoint2To(int degrees);
};

class ControlPanelForm : public QWidget, private Ui::ControlPanelForm
{
  Q_OBJECT
  public:
    ControlPanelForm(QWidget* parent = 0);

  public slots:
    void setActiveRobot(int index);

  private:
    ControlThread *_workerThread;
};

#endif

