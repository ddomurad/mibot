#include <QSslSocket>
#include <QHostAddress>
#include <mibLogger.h>
#include "inc/mibLocalDBHandler.h"


using namespace mibot;
LocalDBHandler::LocalDBHandler()
{

}

LocalDBHandler::~LocalDBHandler()
{

}

bool LocalDBHandler::InitReader(QJsonObject &settings)
{
    if(settings["Dir"].isUndefined())
    {
        LOG_ERROR("No Dir setting.");
        return false;
    }

    _dbDir = settings["Dir"].toString();
    return true;
}

bool LocalDBHandler::SetResource(QString resource, QJsonObject *obj, QTcpSocket * caller, QString *outErrror)
{
    LOG_INFO(QString("Set from : '%1'").arg(getCallerInfo(caller)));

    if(setObjectToResource(resource,obj,outErrror))
    {
        notifySubscribers(resource,*obj);
        return true;
    }

    return false;
}

bool LocalDBHandler::GetResource(QString resource, QJsonObject *obj, QTcpSocket * caller, QString * outErrror)
{
    LOG_INFO(QString("Get from : '%1'").arg(getCallerInfo(caller)));
    return getObjectFromResource(resource, obj, outErrror);
}

bool LocalDBHandler::RemoveResource(QString resource, QTcpSocket *caller, QString *outErrror)
{
    LOG_INFO(QString("Remove from : '%1'").arg(getCallerInfo(caller)));
    if(removeResource(resource, outErrror))
    {
        UnsubscribeAll(resource);
        return true;
    }

    return false;
}

bool LocalDBHandler::ListResources(QString resourceDir, QJsonObject *obj, QTcpSocket *caller, QString *outErrror)
{
    LOG_INFO(QString("List from : '%1'").arg(getCallerInfo(caller)));

    return listResources(resourceDir, obj, outErrror);
}

QString LocalDBHandler::getGlobalPath(QString path)
{
    if(_dbDir.endsWith("/"))
        return _dbDir + path;
    else
        return QString("%1/%2").arg(_dbDir).arg(path);
}

QString LocalDBHandler::getCallerInfo(QTcpSocket *caller)
{
    if(caller == nullptr)
        return QString("NULL");

    QSslSocket * sslCaller = qobject_cast<QSslSocket *>(caller);
    if(sslCaller == nullptr)
        return caller->peerAddress().toString();

    QList<QString> peerCommonNames = sslCaller->peerCertificate().subjectInfo(QSslCertificate::CommonName);
    if(peerCommonNames.count() == 0 )
        return QString("%1:NO_NAME").arg(caller->peerAddress().toString());

    return QString("%1:%2").arg(caller->peerAddress().toString(), peerCommonNames[0]);
}

bool LocalDBHandler::createDirectoryForResource(QString resource)
{
    QFileInfo file(resource);
    resource = file.path();

    QString gpathToDir = getGlobalPath(resource);
    if(pathExists(gpathToDir)) return true;

    QDir dir(_dbDir);
    if(!dir.mkpath(resource))
    {
        QString errStr = QString("Can't create resource dir: '%1'").arg(resource);
        LOG_ERROR(errStr);
        return false;
    }

    return true;
}

bool LocalDBHandler::pathExists(QString path)
{
    if(path.endsWith(".json"))
        return QFile(path).exists();
    else
        return QDir(path).exists();
}

bool LocalDBHandler::getObjectFromResource(QString resource, QJsonObject * obj, QString * error)
{
    QString gpath = getGlobalPath(resource);
    *obj = QJsonObject();
    if(!pathExists(gpath))
    {
        LOG_WARNING( QString("Given resource don't exists: '%1'").arg(gpath));
        *error = "Resource dont exists";
        return false;
    }

    QFile inFile(gpath);
    if(!inFile.open(QIODevice::ReadOnly))
    {
        LOG_ERROR(QString("Can't read resource file: '%1'").arg(gpath));
        *error = "Can't read resource";
        return false;
    }

    QByteArray data = inFile.readAll();
    inFile.close();

    QJsonParseError parseError;
    QJsonObject loadedObj = QJsonDocument::fromJson(data,&parseError).object();
    if(loadedObj.isEmpty())
    {
        QString errStr = QString("Can't read resource: '%1'").arg(parseError.errorString());
        LOG_ERROR(errStr);
        *error = errStr;
        return false;
    }

    *obj = loadedObj;

    return true;
}

bool LocalDBHandler::setObjectToResource(QString resource, QJsonObject *obj, QString *error)
{
    if(!createDirectoryForResource(resource))
    {
        *error = "Can't create directory for resource.";
        return false;
    }

    QString gpath = getGlobalPath(resource);

    QFile outFile(gpath);
    if(!outFile.open(QIODevice::WriteOnly))
    {
        LOG_ERROR(QString("Can't open resource for writing: '%1'").arg(gpath));
        *error = "Can't write resource";
        return false;
    }

    QJsonDocument doc(*obj);

    outFile.write( doc.toJson() );
    return true;
}

bool LocalDBHandler::removeResource(QString resource, QString *error)
{
    QString gpath = getGlobalPath(resource);
    if(!pathExists(gpath)) return true;

    QDir dir(_dbDir);
    if(!dir.remove( resource ))
    {
        LOG_ERROR(QString("Can't remove resource: '%1'").arg(resource));
        *error = "Can't remove resource";
        return false;
    }

    return true;
}

bool LocalDBHandler::listResources(QString resource, QJsonObject *obj, QString *error)
{
    *obj = QJsonObject();
    if(resource.endsWith(".json"))
    {
        *error = "Can't list resource object!";
        return false;
    }

    QString gpath = getGlobalPath(resource);
    if(!pathExists(gpath))
    {
        *error = "Rsource directory don't exists.";
        return false;
    }

    QDirIterator dirIreator(gpath);
    QString fileName;

    QJsonArray resArray;
    QJsonArray dirArray;
    while(true)
    {
        fileName = dirIreator.next();
        if(fileName.isEmpty()) break;
        if(fileName.endsWith(".") || fileName.endsWith("..")) continue;
        if(!fileName.endsWith(".json"))
        {
            int index = fileName.lastIndexOf("/");
            if(index == -1)
                dirArray.append(QJsonValue(fileName));
            else
            {
                dirArray.append(QJsonValue(fileName.right(fileName.size() -  (index +1 ))));
            }
        }else
        {
            QFileInfo finfo(fileName);
            resArray.append(QJsonValue(finfo.fileName()));
        }
    }

    QJsonObject outObj;
    outObj.insert("Resources",QJsonValue(resArray));
    outObj.insert("ResourcesDirs",QJsonValue(dirArray));

    *obj = outObj;
    return true;
}

