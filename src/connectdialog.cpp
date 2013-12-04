#include "barobolink.h"
#include "connectdialog.h"
#include <QAbstractItemView>

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

  connectSignals();
}

ConnectDialogForm::~ConnectDialogForm()
{
}

void ConnectDialogForm::selectRow(const QModelIndex &index)
{
  tableView_Robots->selectRow(index.row());
}

void ConnectDialogForm::connectSignals(void)
{
  /* Set up robot tableView signals */
  tableView_Robots->setContextMenuPolicy(Qt::CustomContextMenu);
  QObject::connect(tableView_Robots, SIGNAL(customContextMenuRequested(const QPoint&)),
      robotManager(), SLOT(displayContextMenu(const QPoint)));
  QObject::connect(tableView_Robots, SIGNAL(pressed(const QModelIndex &)),
      robotManager(), SLOT(setActiveIndex(const QModelIndex)));
}
