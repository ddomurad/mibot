#include "MainWindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator trans;
    trans.load("translation_pl");

    a.installTranslator(&trans);

    MainWindow w;
    w.show();

    return a.exec();
}
