#ifndef LOGGEROUTPUT_H
#define LOGGEROUTPUT_H

#include <QObject>

class LoggerOutput : public QObject
{
    Q_OBJECT
public:
    explicit LoggerOutput(QObject *parent = 0);
    ~LoggerOutput();

signals:

public slots:
};

#endif // LOGGEROUTPUT_H
