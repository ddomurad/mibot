#include <QtCore>

#include "inc/mibAccessResources.h"

using namespace mibot;

PrivilegeRes::PrivilegeRes() :
    AbstractResource("privileges")
{
    AddField("alias",QVariant::String);
    AddField("level",QVariant::Int);
}

PrivilegeRes::~PrivilegeRes()
{}

QString PrivilegeRes::Alias()
{
    return Get("alias")->toString();
}

int PrivilegeRes::Level()
{
    return Get("level")->toInt();
}

SocketRes::SocketRes() :
    AbstractResource("sockets"),
    PrivilegesObj(nullptr)
{
    AddField("alias",QVariant::String);
    AddField("privileges",QVariant::Uuid);
    AddField("port",QVariant::Int);
    AddField("strategy",QVariant::String);
    AddField("use_ssl",QVariant::Bool);
    AddField("enabled",QVariant::Bool);
}

SocketRes::~SocketRes()
{
    if(PrivilegesObj != nullptr)
        delete PrivilegesObj;
}


QString SocketRes::Alias()
{
    return Get("alias")->toString();
}

QUuid SocketRes::Privileges()
{
    return Get("privileges")->toUuid();
}

int SocketRes::Port()
{
    return Get("port")->toInt();
}

QString SocketRes::Strategy()
{
    return Get("strategy")->toString();
}

bool SocketRes::UseSsl()
{
    return Get("use_ssl")->toBool();
}

bool SocketRes::IsEnabled()
{
    return Get("enabled")->toBool();
}

UserRes::UserRes() :
    AbstractResource("users"),
    PrivilegesObj(nullptr)
{
    AddField("alias",QVariant::String);
    AddField("privileges",QVariant::Uuid);
}

UserRes::~UserRes()
{
    if(PrivilegesObj != nullptr)
        delete PrivilegesObj;
}

QUuid UserRes::Privileges()
{
    return Get("privileges")->toUuid();
}

QString UserRes::Alias()
{
    return Get("alias")->toString();
}



ConnectionAuditRes::ConnectionAuditRes() :
    AbstractResource("connections_audit")
{
    AddField("connection_id", QVariant::Uuid);
    AddField("user", QVariant::Uuid);
    AddField("socket", QVariant::Uuid);
    AddField("from", QVariant::String);
    AddField("entry_type", QVariant::String);
    AddField("entry_details", QVariant::String);
    AddField("entry_time", QVariant::DateTime);
}

ConnectionAuditRes::~ConnectionAuditRes()
{}

QUuid ConnectionAuditRes::ConnectionId()
{
    return Get("connection_id")->toUuid();
}

QUuid ConnectionAuditRes::User()
{
    return Get("user")->toUuid();
}

QUuid ConnectionAuditRes::Socket()
{
    return Get("socket")->toUuid();
}

QString ConnectionAuditRes::FromAddr()
{
    return Get("from")->toString();
}

QString ConnectionAuditRes::EntryType()
{
    return Get("entry_type")->toString();
}

QString ConnectionAuditRes::EntryDetails()
{
    return Get("entry_details")->toString();
}

QDateTime ConnectionAuditRes::EntryTime()
{
    return Get("entry_time")->toDateTime();
}

void ConnectionAuditRes::SetConnectionId(QUuid v)
{
    Get("connection_id")->setValue(v);
}

void ConnectionAuditRes::SetUser(QUuid v)
{
    Get("user")->setValue(v);
}

void ConnectionAuditRes::SetFromAddr(QString v)
{
    Get("from")->setValue(v);
}

void ConnectionAuditRes::SeteSocket(QUuid v)
{
    Get("socket")->setValue(v);
}

void ConnectionAuditRes::SetEntryType(QString v)
{
    Get("entry_type")->setValue(v);
}

void ConnectionAuditRes::SetEntryTime(QDateTime v)
{
    Get("entry_time")->setValue(v);
}

void ConnectionAuditRes::SetEntryDetails(QString v)
{
    Get("entry_details")->setValue(v);
}


UsersCertificateRes::UsersCertificateRes() :
    AbstractResource("users_certificates")
{
    AddField("user",QVariant::Uuid);
    AddField("fname",QVariant::String);
}

UsersCertificateRes::~UsersCertificateRes()
{}

QUuid UsersCertificateRes::User()
{
    return Get("user")->toUuid();
}

QString UsersCertificateRes::FileName()
{
    return Get("fname")->toString();
}


CertificateSocketBoundRes::CertificateSocketBoundRes() :
    AbstractResource("certificate_socket_bound")
{
    AddField("socket", QVariant::Uuid);
    AddField("certificate", QVariant::Uuid);
}

CertificateSocketBoundRes::~CertificateSocketBoundRes()
{

}

QUuid CertificateSocketBoundRes::Socket()
{
    return Get("socket")->toUuid();
}

QUuid CertificateSocketBoundRes::Certificate()
{
    return Get("certificate")->toUuid();
}


GlobalConfigRes::GlobalConfigRes():
    AbstractResource("cfg_strings")
{
    AddField("subsystem", QVariant::String);
    AddField("key",      QVariant::String);
    AddField("value",    QVariant::String);
}

GlobalConfigRes::~GlobalConfigRes()
{}

QString GlobalConfigRes::Subsystem()
{
    return Get("subsystem")->toString();
}

QString GlobalConfigRes::Key()
{
    return Get("key")->toString();
}

QString GlobalConfigRes::Value()
{
    return Get("value")->toString();
}
