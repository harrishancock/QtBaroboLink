#include "mainwindow.h"
#include "barobolink.h"
#include "qtrobotmanager.h"
#include <QApplication>

QtRobotManager *g_robotManager;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    g_robotManager = new QtRobotManager(0);
    w.show();

    return a.exec();
}
