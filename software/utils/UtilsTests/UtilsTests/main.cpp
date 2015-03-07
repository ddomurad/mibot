#include <QCoreApplication>
#include <mibLogger.h>
#include <mibStandardLoggerBuilder.h>
#include <cstdio>

#include <QIODevice>
#include <QFile>


QJsonObject GetJsonObjectFromFile(QString path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
        exit(1);

    QByteArray arr = file.readAll();
    file.close();
    QJsonDocument jsonDoc;
    return jsonDoc.fromJson(arr).object();
}

int main(int argc, char *argv[])
{   
    mibot::StandardLoggerBuilder buildier;

    QJsonObject jsonObj = GetJsonObjectFromFile("./json_file");

    buildier.BuildLogger(jsonObj);
    if(buildier.AreErrors())
        for(auto log : buildier.GetBuildLog(false))
        {
            if(log.type == mibot::StandardLoggerBuilder::LOG_TYPE::ERROR)
                printf("ERROR: ");
            else printf("OK: ");

            printf("%s\n", log.message.toStdString().c_str());
        }

}
