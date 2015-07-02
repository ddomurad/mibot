#ifndef LOGGERCONSOLEOUTPUT_H
#define LOGGERCONSOLEOUTPUT_H

#include <QtSql/QSqlDatabase>
#include "mibUtilsGlobal.h"
#include "mibLoggerSink.h"

#include "mibLoggerStandardFormaters.h"

namespace mibot
{

class MIBUTILSSHARED_EXPORT LoggerConsoleSink : public LoggerSink
{
public:
    LoggerConsoleSink(LogLevel level,LoggerFormater * formater = new LoggerSimpleConsoleFormater());
    ~LoggerConsoleSink();

    // LoggerOutput interface
public:
    void Write(QString message);
};


class MIBUTILSSHARED_EXPORT LoggerFileSink : public LoggerSink
{
public:
    LoggerFileSink(QFile *file,LogLevel level, LoggerFormater * formater = new LoggerSimpleStringFormater());
    LoggerFileSink(QString fileName, LogLevel level, LoggerFormater * formater = new LoggerSimpleStringFormater());
    ~LoggerFileSink();

    // LoggerOutput interface
public:
    void Write(QString message);

private:
    QFile *_output_file;
    static QString _cant_open_file_for_writing;
};


class MIBUTILSSHARED_EXPORT LoggerPSQLSink : public LoggerSink
{
public:
    LoggerPSQLSink(LogLevel level, QString sender);
    ~LoggerPSQLSink();

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
