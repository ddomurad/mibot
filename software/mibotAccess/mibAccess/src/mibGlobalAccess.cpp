#include <QDebug>
#include <mibLogger.h>

#include "inc/mibGlobalAccess.h"
#include "mibSqlRepository.h"

using namespace mibot;


GlobalAccess::~GlobalAccess()
{
    if(_repository != nullptr) delete _repository;
    if(_socketsResWrapper != nullptr) delete _socketsResWrapper;
    if(_privilegesResWrapper != nullptr) delete _privilegesResWrapper;
}

GlobalAccess &GlobalAccess::get()
{
    static GlobalAccess globalAccess;

    return globalAccess;
}

bool GlobalAccess::Init(QJsonObject &jobj)
{
    if(jobj["Database"].isNull())
    {
        qDebug() << "Can't read database configuration.";
        return false;
    }

    QJsonObject dbobject = jobj["Database"].toObject();
    SqlRepository * sqlRepo = new SqlRepository;

    if( !sqlRepo->Open(dbobject) )
    {
        delete sqlRepo;
        return false;
    }

    GlobalAccess::get()._repository = sqlRepo;

    GlobalAccess::get()._socketsResWrapper =
            new ResourceWrapper<SocketRes>(GlobalAccess::get()._repository);
    GlobalAccess::get()._privilegesResWrapper =
            new ResourceWrapper<PrivilegeRes>(GlobalAccess::get()._repository);
    GlobalAccess::get()._usersResWrapper =
            new ResourceWrapper<UserRes>(GlobalAccess::get()._repository);
    GlobalAccess::get()._connectionAuditResWrapper =
            new ResourceWrapper<ConnectionAuditRes>(GlobalAccess::get()._repository);

    return GlobalAccess::get()._repository->IsOpen();
}

SocketRes *GlobalAccess::Socket(QUuid id)
{
    SocketRes * res = GlobalAccess::get()._socketsResWrapper->getByID(id);
    if(res == nullptr)
        return nullptr;

    res->PrivilegesObj = GlobalAccess::get()._privilegesResWrapper->getByID(res->Privileges());

    return res;
}

ResourcesSet<SocketRes> *GlobalAccess::AllSockets()
{
    ResourcesSet<SocketRes> * res = GlobalAccess::get()._socketsResWrapper->getAll( );

    for(int i=0; i<res->Count(); i++)
        res->At(i)->PrivilegesObj = GlobalAccess::Privilege(res->At(i)->Privileges());

    return res;
}

ResourcesSet<SocketRes> *GlobalAccess::AllEnabledSockets()
{
    QVariant enabled(true);
    ResourcesSet<SocketRes> * res = GlobalAccess::get()._socketsResWrapper->getAllByParam("enabled",&enabled);

    for(int i=0; i<res->Count(); i++)
        res->At(i)->PrivilegesObj = GlobalAccess::Privilege(res->At(i)->Privileges());

    return res;
}

UserRes *GlobalAccess::User(QUuid id)
{
    UserRes * res = GlobalAccess::get()._usersResWrapper->getByID( id );
    if(res == nullptr)
        return nullptr;

    res->PrivilegesObj = GlobalAccess::get()._privilegesResWrapper->getByID(res->Privileges());
    return res;
}

ResourcesSet<UserRes> *GlobalAccess::EnabledUsers()
{
    QVariant enabled(true);
    ResourcesSet<UserRes> * res = GlobalAccess::get()._usersResWrapper->getAllByParam("enabled",&enabled);
    return res;
}

PrivilegeRes *GlobalAccess::Privilege(QUuid id)
{
    return GlobalAccess::get()._privilegesResWrapper->getByID( id );
}

bool GlobalAccess::PushConnectionAudit(ConnectionAuditRes *res)
{
    if(!GlobalAccess::get()._connectionAuditResWrapper->addNew(res) )
    {
        LOG_ERROR("Can't add new object into connection audit repository.");
        return false;
    }

    return true;
}

AbstractRepository *GlobalAccess::Repository()
{
    return _repository;
}

GlobalAccess::GlobalAccess():
    _socketsResWrapper(nullptr),
    _privilegesResWrapper(nullptr),
    _repository(nullptr)
{}
