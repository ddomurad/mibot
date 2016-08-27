#include "./Forms/MainWindow.h"
#include <QApplication>
#include <QTranslator>
#include <QSplashScreen>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setStyle(QStyleFactory::create("Fusion"));
#ifdef SHOW_SPLASH
    QPixmap splashPixMap(":/other/resources/rover.png");
    QSplashScreen splash(splashPixMap);
    splash.show();

    for(int i=0;i<5;i++)
    {
        a.processEvents();
        QThread::sleep(1);
    }

    splash.close();
#endif //SHOW_SPLASH

    QTranslator trans;
    trans.load("translation_pl");

    a.installTranslator(&trans);

    MainWindow w;
    w.show();

    return a.exec();
}
