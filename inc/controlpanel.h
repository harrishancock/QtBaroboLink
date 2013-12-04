#ifndef _CONTROLPANEL_H_
#define _CONTROLPANEL_H_

#include "ui_controlpanel.h"

class ControlPanelForm : public QWidget, private Ui::ControlPanelForm
{
  Q_OBJECT
  public:
    ControlPanelForm(QWidget* parent = 0);
};

#endif

