#include "inc/mibLoggerStandardOutputs.h"
#include <cstdio>
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

using namespace mibot;

LoggerConsoleOutput::LoggerConsoleOutput(LoggerFormater * formater ):
    LoggerOutput(formater)
{}

LoggerConsoleOutput::~LoggerConsoleOutput()
{}

void mibot::LoggerConsoleOutput::Write(QString message)
{
    qDebug() << message;
    //printf("%s", message.toStdString().c_str());
}

LoggerFileOutput::LoggerFileOutput(QFile *file, LoggerFormater * formater):
    LoggerOutput(formater), _output_file(file)
{}

LoggerFileOutput::LoggerFileOutput(QString fileName, LoggerFormater * formater):
    LoggerOutput(formater), _output_file(new QFile(fileName))
{}

LoggerFileOutput::~LoggerFileOutput()
{
    delete _output_file;
}

void LoggerFileOutput::Write(QString message)
{
    if(_output_file->open(QIODevice::Append) == false)
    {
        qDebug() << _cant_open_file_for_writing;
    }

    _output_file->write(message.toUtf8());
    _output_file->close();
}

QString LoggerFileOutput::_cant_open_file_for_writing = "!!! FATAL_FILE_OUTPUT: Can't open file for writing.";


LoggerPSQLOutput::LoggerPSQLOutput() :
     LoggerOutput(nullptr)
{}

LoggerPSQLOutput::~LoggerPSQLOutput()
{

}

bool LoggerPSQLOutput::Open(QJsonObject &config, QString sender)
{
    _db = QSqlDatabase::addDatabase("QPSQL");

    _db.setHostName( config["Host"].toString() );
    _db.setPort( config["Port"].toInt() );
    _db.setDatabaseName( config["Base"].toString());
    _db.setUserName( config["User"].toString() );
    _db.setPassword( config["Password"].toString() );

    _sender = sender;

    QString logStr = QString("Connecting to mysql database server: (h='%1:%4', u='%2', db='%3')")
                    .arg(_db.hostName(), _db.userName(), _db.databaseName())
                    .arg(_db.port());

    qDebug() << logStr;
        if(!_db.open())
        qDebug() << "PSql database connection failure.";
    else
        qDebug() << "PSql database connection success.";

    return _db.isOpen();
}

void LoggerPSQLOutput::Write(QString)
{}

void LoggerPSQLOutput::WriteLog(LogLevel level, QString file, QString function, qint32 line, QString message)
{

    message = message.replace('\'',"''");

    QString query_string = "INSERT INTO logger_sink (\"datetime\",\"file\",\"method\",\"line\",\"type\",\"message\",\"sender\") VALUES( now(), '%1','%2',%6,'%3','%4','%5');";
    query_string = query_string.arg(file,function, LoggerSimpleStringFormater::LogLevel2Str(level), message,_sender).arg(line);

    QSqlQuery query;

    if( !query.exec( query_string ) )
    {
        qDebug() << "loging to db. failed.";
        qDebug() << query.lastError().text();
    }

}
