#ifndef SETTINGSVALUE_H
#define SETTINGSVALUE_H

#include <QtCore>

namespace mibot
{

class SettingsValue
{
public:
    virtual ~SettingsValue();

    QString Name();

    virtual void SetFromJson(QJsonValue &value) = 0;
    virtual QJsonValue GetJson() = 0;

    virtual QString Dump() = 0;
protected:
    SettingsValue(QString Name);
private:
    QString _name;
};


class IntSettingsValue : public SettingsValue
{
public:
    IntSettingsValue(QString Name);
    ~IntSettingsValue(){}

    int value;

    void SetFromJson(QJsonValue &val);
    QJsonValue GetJson();

    QString Dump();
};

class DoubleSettingsValue : public SettingsValue
{
public:
    DoubleSettingsValue(QString Name);
    ~DoubleSettingsValue();

    double value;

    void SetFromJson(QJsonValue &val);

    QJsonValue GetJson();
    QString Dump();
};

class BoolSettingsValue : public SettingsValue
{
public:
    BoolSettingsValue(QString Name);
    ~BoolSettingsValue(){}

    bool value;

    void SetFromJson(QJsonValue &val);

    QJsonValue GetJson();
    QString Dump();
};

class StringSettingsValue : public SettingsValue
{
public:
    StringSettingsValue(QString Name);
    ~StringSettingsValue();

    QString value;

    void SetFromJson(QJsonValue &val);

    QJsonValue GetJson();
    QString Dump();
};

}
#endif // SETTINGSVALUE_H
