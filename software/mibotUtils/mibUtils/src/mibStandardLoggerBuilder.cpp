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

        if(key == "Sinks")
        {
            if(!iter.value().isArray())
            {
                LogProcess(LOG_TYPE::ERROR,"LoggerSink property is a non-array element.");
                continue;
            }

            QJsonArray array = iter.value().toArray();
            for(int i=0; i< array.count(); i++)
            {
                if(!array[i].isObject())
                {
                    LogProcess(LOG_TYPE::ERROR,"LoggerSink array element is a non-object type.");
                    continue;
                }

                QJsonObject obj = array[i].toObject();
                BuildSink(obj);
            }
        }
        else
            LogProcess(LOG_TYPE::ERROR, QString("Unsuported param '%1'.").arg(key));

        iter++;
    }
    while(iter != json.end());

    LogProcess(LOG_TYPE::OK, "Json reading done.");
}

void StandardLoggerBuilder::BuildSink(QJsonObject &json)
{
    if(json.value("LogLevel").isUndefined())
        { LogProcess(LOG_TYPE::ERROR,"Undefined LoggerSink LogLevel param."); return; }

    if(!json.value("LogLevel").isString())
        { LogProcess(LOG_TYPE::ERROR,"LoggerSink LogLevel param is not a string."); return; }

    if(json.value("Type").isUndefined())
        { LogProcess(LOG_TYPE::ERROR,"Undefined LoggerOutput Type param."); return; }

    if(!json.value("Type").isString())
        { LogProcess(LOG_TYPE::ERROR,"LoggerSink Type param is not a string."); return; }

    QString sinkType = json.value("Type").toString();

    LogLevel level = LoggerSimpleStringFormater::Str2LogLevel(json.value("LogLevel").toString());


    if(sinkType.toLower() == "console")
    {
        if(json.value("Formater").isUndefined())
            { LogProcess(LOG_TYPE::ERROR,"Undefined LoggerOutput Formater object."); return; }

        if(!json.value("Formater").isObject())
            { LogProcess(LOG_TYPE::ERROR,"LoggerSink Formater is not an object."); return; }

        auto jsonObject = json.value("Formater").toObject();
        auto formater = CreateFormater( jsonObject );
        if(formater != nullptr)
        {
            LoggerManager::instance()->AddSink( new LoggerConsoleSink(level, formater) );
        }

        LogProcess(LOG_TYPE::OK, QString("Console LoggerSink added."));
    }
    else if(sinkType.toLower() == "db")
    {
        //auto jsonObject = json.value("Database").toObject();
        LoggerPSQLSink * output = new LoggerPSQLSink(level,json["Sender"].toString());
        LoggerManager::instance()->AddSink( output );

        /*if(!output->Open(jsonObject,json["Sender"].toString() ))
        {
            delete output;
            LogProcess(LOG_TYPE::ERROR, QString("Database LoggerSink NOT added."));
        }
        else
        {
            LoggerManager::instance()->AddSink( output );
            LogProcess(LOG_TYPE::OK, QString("Database LoggerSink added."));
        }*/
    }
    else
    {
        LogProcess(LOG_TYPE::ERROR,
                   QString("Unknown LoggerSink type '%1'.").arg(sinkType));
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

    if(formaterType == "SimpleConsoleFormater")
    {
        LogProcess(LOG_TYPE::OK, QString("Creating SimpleConsoleFormater."));
        return new LoggerSimpleConsoleFormater();
    }

    LogProcess(LOG_TYPE::ERROR,
               QString("Unknown LoggerFormater type '%1'.").arg(formaterType));
    return nullptr;
}

