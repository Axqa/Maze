#include "mainwindow.h"

#include <QApplication>
#include "mazegenerator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MazeGenerator mg;
    MainWindow w(&mg);
    w.show();
    return a.exec();
}
