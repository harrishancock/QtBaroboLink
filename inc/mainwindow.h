#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controlpanel.h"
#include "connectdialog.h"
#include "tcpcomms.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();

public slots:
    void errorDialog(const QString & msg);

private:
    Ui::MainWindow *ui;
    ControlPanelForm *controlPanel_;
    ConnectDialogForm *connectDialog_;
    CommsForwarding *comms_;
    void connectSignals();
};

MainWindow* mainWindow();
#endif // MAINWINDOW_H
