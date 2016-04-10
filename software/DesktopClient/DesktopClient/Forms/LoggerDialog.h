#ifndef LOGGERDIALOG_H
#define LOGGERDIALOG_H

#include <QListWidget>
#include <QWidget>

class LoggerDialog : public QWidget
{
    Q_OBJECT
public:
    static LoggerDialog * get();
    ~LoggerDialog();

    void Info(QString operation, QString msg);
    void Warning(QString operation, QString message);
    void Error(QString opeartion, QString msg);
    void Write(QString message);
signals:

public slots:

private:
    static LoggerDialog * _dialog;
    explicit LoggerDialog(QWidget *parent = 0);

    QListWidget * _list;
};

#endif // LOGGERDIALOG_H
