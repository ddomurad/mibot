#ifndef LOCALDBREADER_H
#define LOCALDBREADER_H

#include "mibAbstractDataBaseHandler.h"

namespace mibot
{
class LocalDBHandler : public AbstractDataBaseHandler
{
public:
    LocalDBHandler();
    ~LocalDBHandler();

    bool InitReader(QJsonObject & settings);
public:
    bool SetResource(QString resource, QJsonObject *obj, QTcpSocket * caller, QString * outErrror);
    bool GetResource(QString resource, QJsonObject *obj, QTcpSocket * caller, QString * outErrror);
    bool RemoveResource(QString resource, QTcpSocket * caller, QString * outErrror);
    bool ListResources(QString resourceDir, QJsonObject *obj, QTcpSocket *caller, QString *outErrror);
private:
    QString _dbDir;

    QString getGlobalPath(QString path);
    QString getCallerInfo(QTcpSocket * caller);
    bool    createDirectoryForResource(QString resource);

    bool pathExists(QString path);
    bool getObjectFromResource(QString resource, QJsonObject *obj, QString *error);
    bool setObjectToResource(QString resource, QJsonObject *obj, QString *error);
    bool removeResource(QString resource, QString *error);
    bool listResources(QString resource, QJsonObject * obj, QString *error);
};
}
#endif // LOCALDBREADER_H
