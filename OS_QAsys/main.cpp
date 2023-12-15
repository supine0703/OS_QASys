#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setFont(QFont("Cascadia Mono", 16));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
