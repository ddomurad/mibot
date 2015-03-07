#ifndef LOGGERFORMATER_H
#define LOGGERFORMATER_H

#include <QObject>

class LoggerFormater : public QObject
{
    Q_OBJECT
public:
    explicit LoggerFormater(QObject *parent = 0);
    ~LoggerFormater();

signals:

public slots:
};

#endif // LOGGERFORMATER_H
