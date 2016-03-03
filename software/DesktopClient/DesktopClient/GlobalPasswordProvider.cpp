#include "GlobalPasswordProvider.h"

GlobalPasswordProvider::GlobalPasswordProvider()
{
    pass = "";
}

GlobalPasswordProvider::~GlobalPasswordProvider()
{

}

QString GlobalPasswordProvider::Get()
{
    if(pass.isEmpty())
    {
        auto dialog = new PassDialog(nullptr);
        dialog->exec();
        pass = dialog->GetPass();
        delete dialog;
    }

    return pass;
}

void GlobalPasswordProvider::Clear()
{
    pass = "";
}


QString GlobalPasswordProvider::pass = "";


PassDialog::PassDialog(QWidget *parent) :
    QDialog(parent)
{
    auto layout = new QBoxLayout(QBoxLayout::TopToBottom,this);
    layout->addWidget(new QLabel("Password", this));
    passwordLineEdit = new QLineEdit(this);

    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);

    auto btn_ok = new QPushButton("Ok", this);
    auto btn_cancle = new QPushButton("Cancle", this);

    layout->addWidget(passwordLineEdit);
    layout->addWidget(btn_ok);
    layout->addWidget(btn_cancle);

    connect(btn_ok, SIGNAL(clicked()), this, SLOT(close()));
    connect(btn_cancle, SIGNAL(clicked()), passwordLineEdit, SLOT(clear()));
    connect(btn_cancle, SIGNAL(clicked()), this, SLOT(close()));

    this->setLayout(layout);
}

QString PassDialog::GetPass()
{
    return passwordLineEdit->text();
}
