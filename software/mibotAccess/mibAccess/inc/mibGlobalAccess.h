#ifndef GLOBALACCESS_H
#define GLOBALACCESS_H

#include "mibAccessGlobals.h"

#include <mibAbstractResource.h>
#include <mibAbstractRepository.h>
#include <mibResourceWrapper.h>
#include "mibAccessResources.h"

namespace mibot
{

class MIBACCESSSHARED_EXPORT GlobalAccess
{
public:
    ~GlobalAccess();
    static GlobalAccess & get();

    static bool Init(QJsonObject &jobj);

    static SocketRes                                 *Socket(QUuid id);
    static UserRes                                   *User(QUuid id);

    static UsersCertificateRes                       *UserCertificate(QUuid id);
    static CertificateSocketBoundRes                 *CertificateSocketBound(QUuid id);
    static ResourcesSet<CertificateSocketBoundRes>   *CertificateSocketBoundsSesBySocket(QUuid socketId);

    static ResourcesSet<GlobalConfigRes>             *AllGlobalConfigsForSubsystem(QString subsystem);
    static ResourcesSet<GlobalConfigRes>             *GlobalConfigsByKey(QString key);

    static bool PushConnectionAudit(ConnectionAuditRes *res);

private:
    GlobalAccess();

    ResourceWrapper<SocketRes>                  *_socketsResWrapper;
    ResourceWrapper<PrivilegeRes>               *_privilegesResWrapper;
    ResourceWrapper<UserRes>                    *_usersResWrapper;
    ResourceWrapper<ConnectionAuditRes>         *_connectionAuditResWrapper;

    ResourceWrapper<UsersCertificateRes>        *_usersCertificateResWrapper;
    ResourceWrapper<CertificateSocketBoundRes>  *_certificateSocketBoundResWrapper;
    ResourceWrapper<GlobalConfigRes>            *_globalConfigResWrapper;

    AbstractRepository                          *_repository;
};

}
#endif // GLOBALACCESS_H
