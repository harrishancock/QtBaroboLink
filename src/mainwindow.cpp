#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "barobolink.h"
#include "controlpanel.h"
#include "connectdialog.h"
#include <QObject>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ControlPanelForm *controlPanel = new ControlPanelForm(ui->tab_control);
    controlPanel->show();

    ConnectDialogForm *connectDialog = new ConnectDialogForm();
    ui->layout_connectArea->addWidget(connectDialog);
    connectDialog->show();

    connectSignals();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSignals()
{
}
