#include "mainwindow.h"
#include "barobolink.h"
#include "qtrobotmanager.h"
#include <QApplication>
#include <mobot.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
