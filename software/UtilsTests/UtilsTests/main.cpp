#include <QDebug>
#include <QCoreApplication>
#include <mibSettingsClient.h>
#include <mibSettingsObject.h>
#include <mibLogger.h>
#include <mibLoggerStandardSinks.h>

#include <pthread.h>

class MyUser : public mibot::SettingsObject
{
public:
    MyUser(QString res):
        SettingsObject(res, true)
    {
        alias = new mibot::StringSettingsValue("alias");
        enabled = new mibot::BoolSettingsValue("enabled");

        AddValue( alias );
        AddValue( enabled );
    }

    mibot::StringSettingsValue *alias;
    mibot::BoolSettingsValue *enabled;
};

void TestFunction()
{
    QStringList resList = mibot::SettingsClient::GetResourceList("./users/", 10 * 60 * 1000);
    QList<MyUser*> users;
    for(QString res : resList)
    {
        qDebug() << "GET: " << res;
        users.append(mibot::SettingsClient::CreateReource<MyUser>( "./users/" + res ));
    }

    for(MyUser * user : users)
        if(!user->Sync(1000,false))
            LOG_WARNING("Can't sync: " + user->Resource());

    for(MyUser * user : users)
        if(user->Exists())
            qDebug() << user->Resource()<< user->alias->value << user->enabled->value;

    while(true)
    {
        int cnt = users.count();
        for(MyUser * u : users)
            if(u->enabled->value == false)
                cnt--;

        if(cnt == 0) break;
        QThread::msleep(500);
        LOG_INFO("Tick ...");
    }

    for(MyUser * u : users)
    {
        u->enabled->value = true;
        if(!u->Upload(1000))
            LOG_ERROR("Can't upload object: " + u->Resource());
    }

    for(MyUser * user : users)
        user->Release();
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    mibot::LoggerManager::instance()->AddSink(
                new mibot::LoggerConsoleSink(
                mibot::LogLevel::Debug,
                new mibot::LoggerSimpleConsoleFormater()));

    LOG_INFO("Test log ...");
    QJsonObject settings;
    settings.insert("cert",QJsonValue("/home/work/Projects/praca_mgr/mibot/software/tools/SettingsManager/debug/certs/manager"));
    settings.insert("caCerts",QJsonValue("/home/work/Projects/praca_mgr/mibot/software/tools/SettingsManager/debug/certs/trusted.pem"));
    settings.insert("addr",QJsonValue("localhost"));
    settings.insert("port",QJsonValue(20400));
    settings.insert("peerName",QJsonValue("SettingsServer"));

    if(!mibot::SettingsClient::StartClient( settings , 10 * 60 * 1000))
    {
        LOG_ERROR("Can't start settigns cloient !");
        return 1;
    }

    TestFunction();

    QThread::sleep(1);
    mibot::SettingsClient::StopClient();


    return app.exec();
}
