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
        DEFLOG_ERROR("Can't read database configuration.");
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

UserRes *GlobalAccess::User(QUuid id)
{
    UserRes * res = GlobalAccess::get()._usersResWrapper->getByID( id );
    if(res == nullptr)
        return nullptr;

    res->PrivilegesObj = GlobalAccess::get()._privilegesResWrapper->getByID(res->Privileges());

    return res;
}

bool GlobalAccess::PushConnectionAudit(ConnectionAuditRes *res)
{
    if(!GlobalAccess::get()._connectionAuditResWrapper->addNew(res) )
    {
        DEFLOG_ERROR("Can't add new object into connection audit repository.");
        return false;
    }

    return true;
}

GlobalAccess::GlobalAccess():
    _socketsResWrapper(nullptr),
    _privilegesResWrapper(nullptr),
    _repository(nullptr)
{}
