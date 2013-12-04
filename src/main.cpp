#include "mainwindow.h"
#include "barobolink.h"
#include "qtrobotmanager.h"
#include <QApplication>
#include <mobot.h>

QtRobotManager *g_robotManager;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    g_robotManager = new QtRobotManager(0);
    g_robotManager->read( Mobot_getConfigFilePath() );
    MainWindow w;
    w.show();

    return a.exec();
}
