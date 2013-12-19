#ifndef CONNECTDIALOG_H_
#define CONNECTDIALOG_H_

#include "ui_connectpanel.h"

class ConnectDialogForm : public QWidget, public Ui::ConnectDialogForm
{
  Q_OBJECT
  public:
    explicit ConnectDialogForm(QWidget* parent = 0);
    ~ConnectDialogForm();

  public slots:
    void selectRow(const QModelIndex &index);
    void addRobotFromLineEdit();

  private:
    void connectSignals(void);
};

#endif

