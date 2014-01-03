#ifndef CONTROLPANEL_H__
#define CONTROLPANEL_H__

#include <QModelIndex>
#include "ui_controlpanel.h"
#include "asyncrobot.h"

class ControlPanelForm : public QWidget, private Ui::ControlPanelForm
{
  Q_OBJECT
  public:
    ControlPanelForm(QWidget* parent = 0);

  public slots:
    void setActiveRobot(int index);
    void setActiveRobot(const QModelIndex &index);
    void driveJoint1To(int);
    void driveJoint2To(int);
    void enable(int state);

  private:
    void startControlThread();
    int enabled_;
    AsyncRobot *asyncrobot_;
    QThread *mobotthread_;
};

#endif

