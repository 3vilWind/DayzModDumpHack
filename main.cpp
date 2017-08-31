#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Luós.tm");
    QCoreApplication::setApplicationName("DumpHack");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
