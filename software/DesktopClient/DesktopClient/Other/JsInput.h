#ifndef JSINPUT_H
#define JSINPUT_H

#include <QObject>
#include <QThread>
#include <QFile>

class JsInput : public QThread
{
    Q_OBJECT
public:
    JsInput(QObject *parent);
    ~JsInput();

    void Start(QString dev);
    void Stop();

    bool Status();

    int axis[8];
    bool btn[16];
    // QThread interface
protected:
    void run();
    bool status;
    bool shall_run;

    int js_dev;
};

#endif // JSINPUT_H
