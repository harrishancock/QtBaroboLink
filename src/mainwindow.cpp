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
    controlPanel_->setEnabled(false);
    controlPanel_->show();

    /* tab_control needs a QLayout for it to be able to manage its
     * controlPanel_ widget correctly (auto-expand, etc.). Couldn't set one
     * in QtCreator for some reason, so we have to set it here. Passing
     * tab_control to the QLayout constructor transfers ownership of the
     * QLayout object to tab_control (by calling tab_control->setLayout()). */
    QVBoxLayout* layout = new QVBoxLayout (ui->tab_control);
    layout->addWidget(controlPanel_);

    /* Looks kinda stupid with a margin. */
    layout->setContentsMargins(0, 0, 0, 0);

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
      controlPanel_, SLOT(setActiveRobot(const QModelIndex&)));
  QObject::connect(connectDialog_, SIGNAL(activeRobotSelected(const QModelIndex &)),
      controlPanel_, SLOT(setActiveRobot(const QModelIndex&)));
}
