#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    if (argc != 3) {
        printf("usage: %s <port> <ip>\n", argv[0]);
        exit(1);
    }

    w.show();

    return a.exec();
}

