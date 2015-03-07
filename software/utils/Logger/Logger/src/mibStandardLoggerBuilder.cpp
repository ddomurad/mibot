#include "inc/mibStandardLoggerBuilder.h"
#include "inc/mibLogger.h"

using namespace mibot;

StandardLoggerBuilder::~StandardLoggerBuilder()
{}

void mibot::StandardLoggerBuilder::BuildLogger(QJsonObject &json)
{
    QJsonObject::iterator iter = json.begin();

    QString defaultChannelName;
    do
    {
        QString key = iter.key();

        if(key == "Channels")
        {
            if(!iter.value().isArray())
            {
                LogProcess(LOG_TYPE::ERROR,"LoggerChannel property is a non-array element.");
                continue;
            }

            QJsonArray array = iter.value().toArray();
            for(int i=0; i< array.count(); i++)
            {
                if(!array[i].isObject())
                {
                    LogProcess(LOG_TYPE::ERROR,"LoggerChannels array element is a non-object type.");
                    continue;
                }

                QJsonObject obj = array[i].toObject();
                BuildChannel(obj);
            }
        }
        else if(key == "DefaultChannel")
        {
            if(!iter.value().isString())
            {
                LogProcess(LOG_TYPE::ERROR,"DefaultLoggerChannel name property is not a strign.");
                continue;
            }
            defaultChannelName = iter.value().toString();
        }
        else
            LogProcess(LOG_TYPE::ERROR, QString("Unsuported param '%1'.").arg(key));

        iter++;
    }
    while(iter != json.end());

    LogProcess(LOG_TYPE::OK, "Json reading done.");

    if(!defaultChannelName.isEmpty())
    {
        SET_DEF_LOGGER(defaultChannelName);
        LogProcess(LOG_TYPE::OK,
                   QString("Default channel name set to: '%1'.")
                   .arg(defaultChannelName));
    }
}

void StandardLoggerBuilder::BuildChannel(QJsonObject &json)
{
    if(json.value("Name").isUndefined())
        { LogProcess(LOG_TYPE::ERROR,"Undefined LoggerChannel Name param."); return; }

    if(!json.value("Name").isString())
        { LogProcess(LOG_TYPE::ERROR,"LoggerChannel Name param is not a string."); return; }

    if(json.value("LogLevel").isUndefined())
        { LogProcess(LOG_TYPE::ERROR,"Undefined LoggerChannel LogLevel param."); return; }

    if(!json.value("LogLevel").isString())
        { LogProcess(LOG_TYPE::ERROR,"LoggerChannel LogLevel param is not a string."); return; }

    if(json.value("Outputs").isUndefined())
        { LogProcess(LOG_TYPE::ERROR,"Undefined LoggerChannel Outputs array."); return; }

    if(!json.value("Outputs").isArray())
        { LogProcess(LOG_TYPE::ERROR,"LoggerChannel Outputs param is not an array."); return; }

    QString channelName = json.value("Name").toString();
    QJsonArray outputs = json.value("Outputs").toArray();
    LogLevel level = LoggerSimpleStringFormater::Str2LogLevel(json.value("LogLevel").toString());

    LoggerChannel * channel = new LoggerChannel(level);

    for(int i=0;i <outputs.count(); i++)
    {
        if(!outputs[i].isObject())
        {
            LogProcess(LOG_TYPE::ERROR, "One of LoggerChannel Outputs array elemens is an non-object element.");
            continue;
        }
        auto jsonObject = outputs[i].toObject();
        AddOputput(jsonObject, channel);
    }

    ADD_LOGGER(channelName, channel);
    LogProcess(LOG_TYPE::OK, QString("Channel added '%1'.").arg(channelName));
}

void StandardLoggerBuilder::AddOputput(QJsonObject &json, LoggerChannel *channel)
{
    if(json.value("Type").isUndefined())
        { LogProcess(LOG_TYPE::ERROR,"Undefined LoggerOutput Type param."); return; }

    if(!json.value("Type").isString())
        { LogProcess(LOG_TYPE::ERROR,"LoggerOutput Type param is not a string."); return; }

    if(json.value("Formater").isUndefined())
        { LogProcess(LOG_TYPE::ERROR,"Undefined LoggerOutput Formater object."); return; }

    if(!json.value("Formater").isObject())
        { LogProcess(LOG_TYPE::ERROR,"LoggerOutput Formater is not an object."); return; }

    QString outputType = json.value("Type").toString();

    if(outputType.toLower() == "console")
    {
        auto jsonObject = json.value("Formater").toObject();
        auto formater = CreateFormater( jsonObject );
        if(formater != nullptr)
            *channel += new LoggerConsoleOutput();

        LogProcess(LOG_TYPE::OK, QString("Console ChannelOutput added."));
    }
    else
    {
        LogProcess(LOG_TYPE::ERROR,
                   QString("Unknown LoggerOutput type '%1'.").arg(outputType));
    }
}

LoggerFormater *StandardLoggerBuilder::CreateFormater(QJsonObject &json)
{
    if(json.value("Type").isUndefined())
        { LogProcess(LOG_TYPE::ERROR,"Undefined LoggerFormater Type param."); return nullptr; }

    if(!json.value("Type").isString())
        { LogProcess(LOG_TYPE::ERROR,"LoggerFormater Type is not a string."); return nullptr; }

    QString formaterType = json.value("Type").toString();
    if(formaterType == "SimpleStringFormater")
    {
        LogProcess(LOG_TYPE::OK, QString("Creating SimpleStringFormater."));
        return new LoggerSimpleStringFormater();
    }

    LogProcess(LOG_TYPE::ERROR,
               QString("Unknown LoggerFormater type '%1'.").arg(formaterType));
    return nullptr;
}

