#include <QCoreApplication>
#include <mibLogger.h>
#include <cstdio>

#include <QIODevice>
#include <QFile>



int main(int argc, char *argv[])
{   
    qDebug() << "da";
    CREATE_LOGGER("test", mibot::LogLevel::Debug);

    DEF_LOGGER +=
    {
            new mibot::LoggerConsoleOutput(),
            new mibot::LoggerFileOutput("./test")
    };

    DEFLOG_MESSAGE(" -----------------  START --------------- \n");
    DEFLOG_ERROR("Error");
    DEFLOG_WARNING("Warnign");
    DEFLOG_IMPORTANT("Important");
    DEFLOG_INFO("Info");
    DEFLOG_DEBUG("Debug");
}
