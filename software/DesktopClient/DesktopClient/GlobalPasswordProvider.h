#ifndef GLOBALPASSWORDPROVIDER_H
#define GLOBALPASSWORDPROVIDER_H

#include <QString>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QGridLayout>

class PassDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PassDialog(QWidget * parent);
    QString GetPass();
private:
    QLineEdit * passwordLineEdit;
};

class GlobalPasswordProvider
{
public:
    GlobalPasswordProvider();
    ~GlobalPasswordProvider();

    static QString Get();
    static void Clear();
private:
    static QString pass;
};

#endif // GLOBALPASSWORDPROVIDER_H
