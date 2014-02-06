#include "mainwindow.h"
#include "barobolink.h"
#include "qtrobotmanager.h"
#include <QApplication>
#include <QMutex>
#include <mobot.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = mainWindow();
    w->show();

    return a.exec();
}

MainWindow* mainWindow()
{
  static MainWindow * instance = NULL;
  static QMutex mutex;
  if(NULL == instance) {
    instance = new MainWindow();
    instance->init();
  }
  return instance;
}
