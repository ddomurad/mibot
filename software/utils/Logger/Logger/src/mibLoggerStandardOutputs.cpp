#include "inc/mibLoggerStandardOutputs.h"
#include <cstdio>

using namespace mibot;

LoggerConsoleOutput::LoggerConsoleOutput(LoggerFormater * formater ):
    LoggerOutput(formater)
{}

LoggerConsoleOutput::~LoggerConsoleOutput()
{}

void mibot::LoggerConsoleOutput::Write(QString message)
{
    printf("%s", message.toStdString().c_str());
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
        printf("%s\n",_cant_open_file_for_writing.toStdString().c_str());
    }

    _output_file->write(message.toUtf8());
    _output_file->close();
}

QString LoggerFileOutput::_cant_open_file_for_writing = "!!! FATAL_FILE_OUTPUT: Can't open file for writing.";
