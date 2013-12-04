#ifndef _CONNECTDIALOG_H_
#define _CONNECTDIALOG_H_

#include "ui_connectpanel.h"

class ConnectDialogForm : public QWidget, private Ui::ConnectDialogForm
{
  Q_OBJECT
  public:
    explicit ConnectDialogForm(QWidget* parent = 0);
    ~ConnectDialogForm();
};

#endif

