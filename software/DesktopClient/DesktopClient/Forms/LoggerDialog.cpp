#include <QBoxLayout>
#include <QPushButton>
#include "LoggerDialog.h"

LoggerDialog::LoggerDialog(QWidget *parent) : QWidget(parent)
{
    auto layout = new QBoxLayout(QBoxLayout::TopToBottom);
    this->setLayout(layout);

    auto clearButton = new QPushButton("Clear", this);
    _list = new QListWidget(this);

    connect(clearButton, SIGNAL(clicked()),_list,SLOT(clear()));

    layout->addWidget(clearButton);
    layout->addWidget(_list);
}

LoggerDialog *LoggerDialog::get()
{
    if(_dialog == nullptr)
    {
        _dialog = new LoggerDialog(nullptr);
        _dialog->show();
    }

    return _dialog;
}

LoggerDialog::~LoggerDialog()
{

}

void LoggerDialog::Info(QString operation, QString message)
{
    _list->addItem( QString("[INF] %1:%2").arg(operation, message) );
}

void LoggerDialog::Warning(QString operation, QString message)
{
    _list->addItem( QString("[WARN] %1:%2").arg(operation, message) );
}

void LoggerDialog::Error(QString operation, QString message)
{
    _list->addItem( QString("[ERR] %1:%2").arg(operation, message) );
}


LoggerDialog *LoggerDialog::_dialog = nullptr;
