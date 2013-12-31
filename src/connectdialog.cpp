#include "barobolink.h"
#include "connectdialog.h"
#include <QAbstractItemView>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <mobot.h>

class ConnectDialogForm * g_ConnectDialogForm = 0;

ConnectDialogForm::ConnectDialogForm(QWidget *parent)
  : QWidget(parent)
{
  setupUi(this);
  QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
  sizePolicy1.setHorizontalStretch(2);
  sizePolicy1.setVerticalStretch(0);
  sizePolicy1.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
  this->setSizePolicy(sizePolicy1);

  tableView_Robots->setModel(robotManager());
  tableView_Robots->setColumnWidth(0, 30);
  tableView_Robots->setColumnWidth(1, 170);
  tableView_Robots->setSelectionBehavior(QAbstractItemView::SelectRows);

  scanDialog_ = new ScanDialog();
  scanList_ = new ScanList(0);
  scanDialog_->scannedListView->setModel(scanList_);

  g_ConnectDialogForm = this;

  connectSignals();
}

ConnectDialogForm::~ConnectDialogForm()
{
}

void ConnectDialogForm::scanCallbackWrapper(const char* serialID)
{
  qDebug() << "Received scan info from: " << serialID;
  if(NULL != g_ConnectDialogForm) {
    g_ConnectDialogForm->scanCallback(serialID);
  }
}

void ConnectDialogForm::scanCallback(const char* serialID)
{
  /* Add the new entry into the scanlist */
  scanList_->newRobot(QString(serialID));
}

void ConnectDialogForm::selectRow(const QModelIndex &index)
{
  tableView_Robots->selectRow(index.row());
}

void ConnectDialogForm::addRobotFromLineEdit()
{
  QString robotID;
  robotID = edit_robotID->text();
  qDebug() << "Add robot " << robotID;
  robotManager()->addEntry(robotID);
}

void ConnectDialogForm::scanRobots()
{
  static mobot_t* dongle = NULL;
  if(NULL == dongle) {
    dongle = Mobot_getDongle();
    if(NULL == dongle) {
      Mobot_initDongle();
      dongle = Mobot_getDongle();
      if(NULL == dongle) {
        QMessageBox msgBox;
        msgBox.setText(
            "ERROR: No Linkbot dongle detected. Please attach a Linkbot or "
            "Linkbot Dongle to the computer and try again.");
        msgBox.exec();
        return;
      }
    }
  }
  Mobot_registerScanCallback(dongle, scanCallbackWrapper);
  Mobot_queryAddresses(dongle);
  //scanList_->clearAll();
  scanDialog_->exec();
}

void ConnectDialogForm::connectSignals(void)
{
  /* Set up robot tableView signals */
  tableView_Robots->setContextMenuPolicy(Qt::CustomContextMenu);
  QObject::connect(tableView_Robots, SIGNAL(customContextMenuRequested(const QPoint&)),
      robotManager(), SLOT(displayContextMenu(const QPoint)));
  QObject::connect(tableView_Robots, SIGNAL(pressed(const QModelIndex &)),
      robotManager(), SLOT(setActiveIndex(const QModelIndex)));

  /* Connect the signals for adding new robots */
  QObject::connect(edit_robotID, SIGNAL(returnPressed()),
      this, SLOT(addRobotFromLineEdit()));
  QObject::connect(button_addRobot, SIGNAL(clicked()),
      this, SLOT(addRobotFromLineEdit()));
  QObject::connect(button_scan, SIGNAL(clicked()),
      this, SLOT(scanRobots()));
}
