
#include "QCusFluentApplication.h"
#include "QCusFluentStyleSheet.h"
#include "mainwindow.h"


int main(int argc, char* argv[])
{
    QCusFluentApplication a(argc, argv);
    a.setStyleSheet(QCusFluenStyleSheet::defaultStyleSheet());
    MainWindow window;
    window.resize(1260, 780);
    window.show();

    return a.exec();
}
