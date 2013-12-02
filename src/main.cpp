#include "mainwindow.h"
#include "barobolink.h"
#include "robotmanager.h"
#include <QApplication>

CRobotManager *g_robotManager;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
