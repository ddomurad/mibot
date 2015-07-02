#include "inc/mibSettingsValue.h"

using namespace mibot;

SettingsValue::SettingsValue(QString name):
    _name(name)
{}

SettingsValue::~SettingsValue()
{}

QString SettingsValue::Name()
{
    return _name;
}

IntSettingsValue::IntSettingsValue(QString name):
    SettingsValue(name)
{}

void IntSettingsValue::SetFromJson(QJsonValue &val)
{
    value = val.toInt(0);
}

QJsonValue IntSettingsValue::GetJson()
{
    return QJsonValue(value);
}

QString IntSettingsValue::Dump()
{
    return QString("%1=%2").arg(Name()).arg(value);
}


DoubleSettingsValue::DoubleSettingsValue(QString name):
    SettingsValue(name)
{}

DoubleSettingsValue::~DoubleSettingsValue(){}

void DoubleSettingsValue::SetFromJson(QJsonValue &val)
{
    value = val.toDouble(0);
}

QJsonValue DoubleSettingsValue::GetJson()
{
    return QJsonValue(value);
}

QString DoubleSettingsValue::Dump()
{
    return QString("%1=%2").arg(Name()).arg(value);
}

BoolSettingsValue::BoolSettingsValue(QString name):
    SettingsValue(name)
{}

void mibot::BoolSettingsValue::SetFromJson(QJsonValue &val)
{
    value = val.toBool(false);
}

QJsonValue BoolSettingsValue::GetJson()
{
    return QJsonValue(value);
}

QString BoolSettingsValue::Dump()
{
    return QString("%1=%2").arg(Name()).arg(value);
}

StringSettingsValue::StringSettingsValue(QString name):
    SettingsValue(name)
{}

StringSettingsValue::~StringSettingsValue(){}

void StringSettingsValue::SetFromJson(QJsonValue &val)
{
    value = val.toString();
}

QJsonValue StringSettingsValue::GetJson()
{
    return QJsonValue(value);
}

QString StringSettingsValue::Dump()
{
    return QString("%1=%2").arg(Name()).arg(value);
}

