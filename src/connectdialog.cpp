
#include "connectdialog.h"

ConnectDialogForm::ConnectDialogForm(QWidget *parent)
  : QWidget(parent)
{
  setupUi(this);
  QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
  sizePolicy1.setHorizontalStretch(2);
  sizePolicy1.setVerticalStretch(0);
  sizePolicy1.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
  this->setSizePolicy(sizePolicy1);
}

ConnectDialogForm::~ConnectDialogForm()
{
}
