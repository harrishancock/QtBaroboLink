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
    /*
    ui->tableView_Robots->setModel(g_robotManager);
    ui->tableView_Robots->setColumnWidth(0, 30);
    ui->tableView_Robots->setColumnWidth(1, 170);
    */

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
  /* Set up robot tableView signals */
  /*
  ui->tableView_Robots->setContextMenuPolicy(Qt::CustomContextMenu);
  QObject::connect(ui->tableView_Robots, SIGNAL(customContextMenuRequested(const QPoint&)),
      g_robotManager, SLOT(displayContextMenu(const QPoint)));
  QObject::connect(ui->tableView_Robots, SIGNAL(pressed(const QModelIndex &)),
      g_robotManager, SLOT(setActiveIndex(const QModelIndex)));
      */
}
