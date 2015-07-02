#include "inc/mibLoggerStandardSinks.h"
#include <cstdio>
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

using namespace mibot;

LoggerConsoleSink::LoggerConsoleSink(LogLevel level, LoggerFormater * formater ):
    LoggerSink(level,formater)
{}

LoggerConsoleSink::~LoggerConsoleSink()
{}

void mibot::LoggerConsoleSink::Write(QString message)
{
    qDebug() << message;
    //printf("%s", message.toStdString().c_str());
}

LoggerFileSink::LoggerFileSink(QFile *file, LogLevel level, LoggerFormater * formater):
    LoggerSink(level,formater), _output_file(file)
{}

LoggerFileSink::LoggerFileSink(QString fileName, LogLevel level, LoggerFormater * formater):
    LoggerSink(level,formater), _output_file(new QFile(fileName))
{}

LoggerFileSink::~LoggerFileSink()
{
    delete _output_file;
}

void LoggerFileSink::Write(QString message)
{
    if(_output_file->open(QIODevice::Append) == false)
    {
        qDebug() << _cant_open_file_for_writing;
    }

    _output_file->write(message.toUtf8());
    _output_file->write("\n");
    _output_file->close();
}

QString LoggerFileSink::_cant_open_file_for_writing = "!!! FATAL_FILE_OUTPUT: Can't open file for writing.";


LoggerPSQLSink::LoggerPSQLSink(LogLevel level, QString sender) :
     LoggerSink(level,nullptr),
     _sender(sender)
{}

LoggerPSQLSink::~LoggerPSQLSink()
{
    _db.close();
}

bool LoggerPSQLSink::Open(QJsonObject &config, QString sender)
{
    _db = QSqlDatabase::addDatabase( config["Driver"].toString());

    _db.setHostName( config["Host"].toString() );
    _db.setPort( config["Port"].toInt() );
    _db.setDatabaseName( config["Base"].toString());
    _db.setUserName( config["User"].toString() );
    _db.setPassword( config["Password"].toString() );

    _sender = sender;

    QString logStr = QString("Connecting to database server: (h='%1:%4', u='%2', db='%3')")
                    .arg(_db.hostName(), _db.userName(), _db.databaseName())
                    .arg(_db.port());

    qDebug() << logStr;
        if(!_db.open())
        qDebug() << "PSql database connection failure.";
    else
        qDebug() << "PSql database connection success.";

    return _db.isOpen();
}

void LoggerPSQLSink::Write(QString)
{}

void LoggerPSQLSink::WriteLog(LogLevel level, QString file, QString function, qint32 line, QString message)
{

    message = message.replace('\'',"''");

    QString query_string = "INSERT INTO logger_sink (\"datetime\",\"file\",\"method\",\"line\",\"type\",\"message\",\"sender\") VALUES( now(), '%1','%2',%6,'%3','%4','%5');";
    //qDebug() << query_string;
    query_string = query_string.arg(file,function, LoggerSimpleStringFormater::LogLevel2Str(level), message,_sender).arg(line);

    QSqlQuery query;

    if( !query.exec( query_string ) )
    {
        qDebug() << "loging to db. failed.";
        qDebug() << query.lastError().text();
    }

}
