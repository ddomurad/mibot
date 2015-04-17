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
    static ResourcesSet<SocketRes>                   *AllSockets();
    static ResourcesSet<SocketRes>                   *AllEnabledSockets();

    static UserRes                                   *User(QUuid id);
    static ResourcesSet<UserRes>                     *EnabledUsers();

    static PrivilegeRes                              *Privilege(QUuid id);

    static bool PushConnectionAudit(ConnectionAuditRes *res);
    AbstractRepository                              *Repository();
private:
    GlobalAccess();

    ResourceWrapper<SocketRes>                  *_socketsResWrapper;
    ResourceWrapper<PrivilegeRes>               *_privilegesResWrapper;
    ResourceWrapper<UserRes>                    *_usersResWrapper;
    ResourceWrapper<ConnectionAuditRes>         *_connectionAuditResWrapper;

    AbstractRepository                          *_repository;
};

}
#endif // GLOBALACCESS_H
