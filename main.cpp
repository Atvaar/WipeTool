#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(argc, argv);
    w.setWindowTitle("Echo Wipe Utility");
    w.show();

    return a.exec();
}
