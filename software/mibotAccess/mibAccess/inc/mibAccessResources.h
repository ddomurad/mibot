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

class MIBACCESSSHARED_EXPORT UsersCertificateRes : public AbstractResource
{
public:
    UsersCertificateRes();
    ~UsersCertificateRes();

    QUuid User();
    QString FileName();
};

class MIBACCESSSHARED_EXPORT CertificateSocketBoundRes : public AbstractResource
{
public:
    CertificateSocketBoundRes();
    ~CertificateSocketBoundRes();

    QUuid Socket();
    QUuid Certificate();
};

class MIBACCESSSHARED_EXPORT GlobalConfigRes : public AbstractResource
{
public:
    GlobalConfigRes();
    ~GlobalConfigRes();

    QString Subsystem();
    QString Key();
    QString Value();
};

}

#endif // ASSECCRESOURCES_H
