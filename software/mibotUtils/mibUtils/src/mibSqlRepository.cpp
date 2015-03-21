#include "inc/mibSqlRepository.h"
#include "inc/mibLogger.h"

#include "QtSql/QSqlQuery"
#include "QtSql/QSqlRecord"
#include "QtSql/QSqlError"

using namespace mibot;

SqlRepository::SqlRepository() :
    AbstractRepository()
{}

SqlRepository::~SqlRepository()
{
    Close();
}

bool SqlRepository::Open(QJsonObject &config)
{
    _db = QSqlDatabase::addDatabase("QPSQL");

    _db.setHostName( config["Host"].toString() );
    _db.setPort( config["Port"].toInt() );
    _db.setDatabaseName( config["Base"].toString());
    _db.setUserName( config["User"].toString() );
    _db.setPassword( config["Password"].toString() );

    QString logStr = QString("Connecting to mysql database server: (h='%1:%4', u='%2', db='%3')")
                    .arg(_db.hostName(), _db.userName(), _db.databaseName())
                    .arg(_db.port());

    DEFLOG_INFO(logStr);
        if(!_db.open())
        DEFLOG_ERROR("MySql database connection failure.");
    else
        DEFLOG_INFO("MySql database connection success.");

    return _db.isOpen();
}

void SqlRepository::Close()
{
    if(_db.isOpen())
        _db.close();
}

bool SqlRepository::IsOpen()
{
    return _db.isOpen() && _db.isValid();
}

GetResult SqlRepository::GetResourceByID(QUuid id, AbstractResource *res)
{
    if(!IsOpen()) return GetResult::Error;
    QStringList fields = res->Fields();
    QString id_str = id.toString().mid(1,36);

    QStringList fields_string;
    for( QString field : fields )
        fields_string.append( QString("\"%1\"").arg( field) );

    QString query_string = QString("SELECT %1 FROM \"%2\" WHERE id = '%3'")
            .arg(fields_string.join(','), res->ClassName(), id_str);

    QSqlQuery query;

    if( !query.exec( query_string ) )
    {
        DEFLOG_ERROR("SQL QUERY ERROR: " + query_string);
        return GetResult::Error;
    }
    else
        DEFLOG_DEBUG("SQL QUERY: " + query_string);

    if(query.size() == 0)
    {
        DEFLOG_DEBUG("No resource found with id: " + id_str);
        return GetResult::NoResource;
    }

    if(query.size() != 1)
        DEFLOG_WARNING("Multiple resources found under id: " + id_str);

    query.next();

    for(int i=0; i<fields.length(); i++)
        res->Get( fields[i] )->setValue<QString>(query.value(i).toString());

    return GetResult::Ok;
}

bool SqlRepository::AddNewResource(AbstractResource *res)
{
    if(res == nullptr) return false;
    if(!IsOpen()) return false;
    QStringList fields = res->Fields();

    QStringList fields_string;
    QStringList values_strings;

    for( QString field : fields )
    {
        fields_string.append(QString("\"%1\"").arg(field));
        values_strings.append( asSQLAcceptableValue( res->Get(field) , field) );
    }

    QString query_string = QString("INSERT INTO \"%1\" (%2) VALUES(%3)")
            .arg(res->ClassName(), fields_string.join(','), values_strings.join(','));

    QSqlQuery query;

    if( !query.exec( query_string ) )
    {
        DEFLOG_ERROR("SQL QUERY ERROR: " + query_string);
        DEFLOG_ERROR("SQL QUERY ERROR: " + query.lastError().text());
        return false;
    }
    else
        DEFLOG_DEBUG("SQL QUERY: " + query_string);


    return true;
}

QString SqlRepository::asSQLAcceptableValue(QVariant *v, QString field)
{
    if(v->type() == QVariant::String)
        return QString("'%1'").arg(v->toString().replace('\'',"''"));

    if(v->type() == QVariant::Uuid)
        return QString("'%1'").arg(v->toString().mid(1,36));

    if(v->type() == QVariant::Int
            || v->type() == QVariant::Double
            || v->type() == QVariant::UInt
            || v->type() == QVariant::LongLong
            || v->type() == QVariant::ULongLong)
        return v->toString();


    if(v->type() == QVariant::DateTime)
        return QString("'%1'").arg(v->toString());

    DEFLOG_WARNING( QString("Unsuported type for sql writig. (%1: %2)")
                    .arg(field, v->type()));

    return v->toString();
}

