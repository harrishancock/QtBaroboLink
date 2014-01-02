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

    /* Set up control panel */
    controlPanel_ = new ControlPanelForm(ui->tab_control);
    controlPanel_->show();

    /* Set up connect panel */
    connectDialog_ = new ConnectDialogForm();
    ui->layout_connectArea->addWidget(connectDialog_);
    connectDialog_->show();

    /* Start comms forwarding */
    comms_ = new CommsForwarding();
    comms_->start();

    connectSignals();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSignals()
{
  //QObject::connect(connectDialog_->tableView_Robots, SIGNAL(pressed(const QModelIndex &)),
      //controlPanel_, SLOT(setActiveRobot(const QModelIndex)));
  //QObject::connect(connectDialog_, SIGNAL(pressed(const QModelIndex &)),
      //robotManager(), SLOT(setActiveIndex(const QModelIndex)));
  QObject::connect(connectDialog_->tableView_Robots, SIGNAL(pressed(const QModelIndex &)),
      controlPanel_, SLOT(setActiveRobot(const QModelIndex)));
}
