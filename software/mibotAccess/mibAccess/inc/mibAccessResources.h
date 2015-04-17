#ifndef ASSECCRESOURCES_H
#define ASSECCRESOURCES_H

#include "mibAccessGlobals.h"
#include "mibAbstractResource.h"

namespace mibot
{

class MIBACCESSSHARED_EXPORT PrivilegeRes : public AbstractResource
{
public:
    PrivilegeRes();
    ~PrivilegeRes();

    QString Alias();
    int Level();
};

class MIBACCESSSHARED_EXPORT SocketRes : public AbstractResource
{
public:
    SocketRes();
    ~SocketRes();

    QString Alias();
    QUuid   Privileges();
    int     Port();
    QString Strategy();
    QString StrategyConfig();
    bool    UseSsl();
    bool    IsEnabled();

    PrivilegeRes * PrivilegesObj;
};

class MIBACCESSSHARED_EXPORT UserRes : public AbstractResource
{
public:
    UserRes();
    ~UserRes();

    QUuid Privileges();
    QString Alias();
    QString CertificateName();
    bool    IsEnabled();

    PrivilegeRes * PrivilegesObj;
};

class MIBACCESSSHARED_EXPORT ConnectionAuditRes : public AbstractResource
{
public:
    ConnectionAuditRes();
    ~ConnectionAuditRes();

    QUuid   ConnectionId();
    QUuid   User();
    QUuid   Socket();
    QString FromAddr();
    QString EntryType();
    QString EntryDetails();
    QDateTime EntryTime();

    void SetConnectionId(QUuid v);
    void SetUser(QUuid v);
    void SetFromAddr(QString v);
    void SeteSocket(QUuid v);
    void SetEntryType(QString v);
    void SetEntryTime(QDateTime v);
    void SetEntryDetails(QString v);
};

}

#endif // ASSECCRESOURCES_H
