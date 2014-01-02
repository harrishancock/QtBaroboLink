#ifndef SCANDIALOG_H_
#define SCANDIALOG_H_

#include <QDialog>
#include "ui_scandialog.h"

class ScanDialog : public QDialog, public Ui::scanRobotsDialog
{
  Q_OBJECT
  public:
    explicit ScanDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

  public slots:
    void addSelectedRobots(void);
};

#endif
