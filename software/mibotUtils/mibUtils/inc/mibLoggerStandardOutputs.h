#ifndef LOGGERCONSOLEOUTPUT_H
#define LOGGERCONSOLEOUTPUT_H

#include <QtSql/QSqlDatabase>
#include "mibUtilsGlobal.h"
#include "mibLoggerOutput.h"

#include "mibLoggerStandardFormaters.h"

namespace mibot
{

class MIBUTILSSHARED_EXPORT LoggerConsoleOutput : public LoggerOutput
{
public:
    LoggerConsoleOutput(LoggerFormater * formater = new LoggerSimpleConsoleFormater());
    ~LoggerConsoleOutput();

    // LoggerOutput interface
public:
    void Write(QString message);
};


class MIBUTILSSHARED_EXPORT LoggerFileOutput : public LoggerOutput
{
public:
    LoggerFileOutput(QFile *file, LoggerFormater * formater = new LoggerSimpleStringFormater());
    LoggerFileOutput(QString fileName, LoggerFormater * formater = new LoggerSimpleStringFormater());
    ~LoggerFileOutput();

    // LoggerOutput interface
public:
    void Write(QString message);

private:
    QFile *_output_file;
    static QString _cant_open_file_for_writing;
};


class MIBUTILSSHARED_EXPORT LoggerPSQLOutput : public LoggerOutput
{
public:
    LoggerPSQLOutput();
    ~LoggerPSQLOutput();

    bool Open(QJsonObject &config, QString sender);

    // LoggerOutput interface
public:
    void Write(QString);
    void WriteLog(LogLevel level, QString file, QString function, qint32 line, QString message);

private:
    QSqlDatabase _db;
    QString _sender;
};


}
#endif // LOGGERCONSOLEOUTPUT_H
