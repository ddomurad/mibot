#ifndef LOGGERCONSOLEOUTPUT_H
#define LOGGERCONSOLEOUTPUT_H

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

}
#endif // LOGGERCONSOLEOUTPUT_H
