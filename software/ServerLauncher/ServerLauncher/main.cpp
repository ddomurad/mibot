#include <QCoreApplication>
#include "ServerLauncher.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);
    QStringList args;
    for(int i=0;i<argc; i++)
        args << argv[i];

    ServerLauncher * launcher = new ServerLauncher(&application);
    if(!launcher->Init(args))
    {
        qDebug() << "Server lauchning error.";
        return 1;
    }

    return application.exec();
}
