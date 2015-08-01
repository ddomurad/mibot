#include "MainWindow.h"
#include <QApplication>
#include <QTranslator>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef SHOW_SPLASH
    QPixmap splashPixMap(":/other/resources/rover.png");
    QSplashScreen splash(splashPixMap);
    splash.show();
    a.processEvents();
    QThread::sleep(5);
    splash.close();
#endif //SHOW_SPLASH

    QTranslator trans;
    trans.load("translation_pl");

    a.installTranslator(&trans);

    MainWindow w;
    w.show();

    return a.exec();
}
