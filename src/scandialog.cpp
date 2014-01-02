#include "scandialog.h"
#include "qtrobotmanager.h"

ScanDialog::ScanDialog(QWidget *parent, Qt::WindowFlags f)
  : QDialog(parent, f)
{
  setupUi(this);
  scannedListView->setSelectionMode(QAbstractItemView::MultiSelection);
  QObject::connect(button_addRobots, SIGNAL(clicked()),
      this, SLOT(addSelectedRobots()));
}

void ScanDialog::addSelectedRobots(void)
{
  QItemSelectionModel *selectModel;
  selectModel = scannedListView->selectionModel();
  QModelIndexList selected;
  selected = selectModel->selectedIndexes();
  QAbstractItemModel *model = scannedListView->model();
  for(int i = 0; i < selected.size(); i++) {
    robotManager()->addEntry(model->data(selected.at(i)).toString());
  }
}
