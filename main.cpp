#include <QApplication>
#include "mainwindow.h"

int main( int argc, char* argv[])
{
    QApplication PicSim(argc, argv);
    MainWindow window;
    window.show();

    return PicSim.exec();
}
