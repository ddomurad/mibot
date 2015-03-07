#ifndef LOGGERCHANNEL_H
#define LOGGERCHANNEL_H

#include <QObject>

class LoggerChannel : public QObject
{
    Q_OBJECT
public:
    explicit LoggerChannel(QObject *parent = 0);
    ~LoggerChannel();

signals:

public slots:
};

#endif // LOGGERCHANNEL_H
