#include <mibLogger.h>
#include <mibGlobalAccess.h>

#include "inc/mibEnginesController.h"

using namespace mibot;

EnginesController::EnginesController(QObject *parent) :
    QObject(parent),
    _global_config(nullptr)
{
    _local_server = new QLocalServer(this);
}

EnginesController::~EnginesController()
{
    _local_server->close();

    if(ControllerObject::realGPIO != nullptr)
        delete ControllerObject::realGPIO;

    if(_global_config != nullptr)
        delete _global_config;

}

bool EnginesController::Init()
{
    DEFLOG_INFO("Initializing Engines Controller.");

    _global_config = GlobalAccess::AllGlobalConfigsForSubsystem("ec");

    if(_global_config == nullptr)
    {
        DEFLOG_ERROR("Can't load global configurations.");
        return false;
    }

    ControllerObject::realGPIO = new FakeRealPrintGPIO();

    if(!ControllerObject::realGPIO->Init() )
    {
        DEFLOG_ERROR(QString("Can't initialize gpio."));
        return false;
    }

    QString left_a = getConfig("gpio_left_a");
    QString left_b = getConfig("gpio_left_b");
    QString left_pwm = getConfig("gpio_left_pwm");

    QString right_a = getConfig("gpio_right_a");
    QString right_b = getConfig("gpio_right_b");
    QString right_pwm = getConfig("gpio_right_pwm");

    if( left_a.isEmpty() || left_b.isEmpty() || left_pwm.isEmpty()
            || right_a.isEmpty() || right_b.isEmpty() || right_pwm.isEmpty())
    {
        DEFLOG_ERROR("Invalid gpio configuration.");
        return false;
    }

    bool ok;
    int la =    left_a.toInt(&ok);      if(!ok) { DEFLOG_ERROR("Invalid left_a_gpio number.");      return false; }
    int lb =    left_b.toInt(&ok);      if(!ok) { DEFLOG_ERROR("Invalid left_b_gpio number.");      return false; }
    int lpwm =  left_pwm.toInt(&ok);    if(!ok) { DEFLOG_ERROR("Invalid left_pwm_gpio number.");    return false; }
    int ra =    right_a.toInt(&ok);     if(!ok) { DEFLOG_ERROR("Invalid right_a_gpio number.");      return false; }
    int rb =    right_b.toInt(&ok);     if(!ok) { DEFLOG_ERROR("Invalid right_b_gpio number.");      return false; }
    int rpwm =  right_pwm.toInt(&ok);   if(!ok) { DEFLOG_ERROR("Invalid right_pwm_gpio number.");    return false; }

    ControllerObject::realGPIO->SetPinOuputMode( la );
    ControllerObject::realGPIO->SetPinOuputMode( lb );
    ControllerObject::realGPIO->SetPinOuputMode( ra );
    ControllerObject::realGPIO->SetPinOuputMode( rb );
    ControllerObject::realGPIO->EnablePwm( lpwm );
    ControllerObject::realGPIO->EnablePwm( rpwm );

    emergency_break_timeout = getConfig("emergency_break_timeout").toInt(&ok);
    if(!ok) { DEFLOG_ERROR("Invalid emergency_break_timeout value .");    return false; }

    int update_ratio = getConfig("ctrl_update_ratio").toInt(&ok);
    if(!ok) { DEFLOG_ERROR("Invalid ctrl_update_ratio value .");    return false; }

    _update_timer = new QTimer(this);
    connect( _update_timer, SIGNAL(timeout()), this, SLOT(onUpdate()));

    DEFLOG_INFO(QString("Starting udpate itmer with ratio: %1 ms").arg( update_ratio ));
    _update_timer->start( update_ratio );

    QString local_server_name = getConfig("local_server_name");
    QFile server_file( QString("/tmp/%1").arg(local_server_name));
    if(server_file.exists())
    {
        if(!server_file.remove())
        {
            DEFLOG_ERROR( QString("Could not remove server file: /tmp/%1").arg(local_server_name));
            DEFLOG_ERROR( server_file.errorString() );
            return false;
        }else
        {
            DEFLOG_INFO( QString("Local server file removed: /tmp/%1").arg(local_server_name));
        }
    }

    connect(_local_server, SIGNAL(newConnection()), this, SLOT(onConnection()));

    if(_local_server->listen( local_server_name ))
    {
        DEFLOG_INFO(QString("Local server is listening on name '%1'.").arg(local_server_name));
    }
    else
    {
        DEFLOG_ERROR(QString("Local server can't start listening on name '%1'.").arg(local_server_name));
        DEFLOG_ERROR(_local_server->errorString());
        return false;
    }

    return true;
}

void EnginesController::onConnection()
{
    while(_local_server->hasPendingConnections())
    {
        DEFLOG_INFO("Incomming connection.");

        QLocalSocket * socket = _local_server->nextPendingConnection();
        socket->setParent(this);

        ControllerObject *connection = new ControllerObject(this, emergency_break_timeout);
        connection->socket = socket;
        connection->status = new ControllerStatusWrapper( new ControllerStatus );
        connection->fakeGPIO = new FakePrintGPIO();
        (connection->controllers)[0] = new CarMotorController();
        (connection->controllers)[1] = new TankMotorController();

        connection->status->Status()->SetSimMode( true );

        connect( socket, SIGNAL(disconnected()), this, SLOT(onDisconnection()));
        connect( socket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(onError(QLocalSocket::LocalSocketError)));
        connect( socket, SIGNAL(readyRead()), connection, SLOT(onRead()) );

        _connections.append( connection );
    }
}

void EnginesController::onDisconnection()
{
    int connection_index = getConnectionIndex( (QLocalSocket*)sender() );
    if(connection_index == -1)
    {
        DEFLOG_WARNING("Local socket is disconnected, but there is no connection state object.");
        return;
    }

    ControllerObject * state = _connections[connection_index];

    DEFLOG_INFO(QString("Disconnection socket (Master: %1, Simulation: %2).")
                .arg(state->status->IsThisMaster())
                .arg(state->status->IsSimulationMode()));

    _connections.removeAt(connection_index);

    if(state->status->IsThisMaster() && _connections.length() != 0)
        _connections[0]->status->Status()->SetAsMaster();

    delete state;
}

void EnginesController::onError(QLocalSocket::LocalSocketError e)
{
    DEFLOG_ERROR( QString("Local connection error: (%1)").arg(e));
}

void EnginesController::onUpdate()
{
    float time = float(_elapsed_timer.restart())/1000.0f;
    for( int i=0; i<_connections.length(); i++)
        _connections[i]->onUpdate(time);
}

//void EnginesController::onDataRead()
//{
//    int connection_index = getConnectionIndex( (QLocalSocket*)sender() );
//    ControllerObject * state = _connections[connection_index];
//    QByteArray arr =  state->socket->readAll();

//    for( int i = 0; i < arr.length(); i++)
//    {
//        state->status->Status()->PushByte( arr.at(i) );
//        uchar sdl = state->status->Status()->SendDataLength();
//        if(sdl > 0)
//        {
//            uchar *data = state->status->Status()->FlushSendData();

//            state->socket->write((const char*) data, (uint)sdl);
//        }
//    }
//}

QString EnginesController::getConfig(QString key)
{
    if(_global_config == nullptr) return QString();

    for( int i=0;i<_global_config->Count(); i++)
    {
        if( _global_config->At(i)->Key() == key)
            return _global_config->At(i)->Value();
    }

    return QString();
}

int EnginesController::getConnectionIndex(QLocalSocket *socket)
{
    for(int i=0; i<_connections.length(); i++)
    {
        if(_connections[i]->socket == socket)
            return i;
    }
    return -1;
}


ControllerObject::ControllerObject(QObject *parent, uint emergency_break) :
    QObject(parent)
{
    socket = nullptr;
    status = nullptr;
    controllers[0] = nullptr;
    controllers[1] = nullptr;
    fakeGPIO = nullptr;
    last_controllers = nullptr;

    emergency_break_timeout = emergency_break;
    emergency_break_timer.start();
}

ControllerObject::~ControllerObject()
{
    socket->close();

    if(fakeGPIO != nullptr) delete fakeGPIO;
    if(controllers[0] != nullptr) delete controllers[0];
    if(controllers[1] != nullptr) delete controllers[1];
    if(status != nullptr) delete status;
    if(socket != nullptr) delete socket;
}

void ControllerObject::onUpdate(float dt)
{
    GPIO * gpio;
    if(status->IsThisMaster() && !status->IsSimulationMode())
        gpio = realGPIO;
    else
        gpio = fakeGPIO;

    if(emergency_break_timer.elapsed() > emergency_break_timeout )
    {
        status->SetEmergencyBreak();
        qDebug() << "emergency break";
    }

    if(status->CarModel())
    {
        controllers[0]->Update( gpio, status, dt );
        last_controllers = controllers[0];
    }
    else if(status->TankModel())
    {
        controllers[1]->Update( gpio, status, dt );
        last_controllers = controllers[1];
    }
    else
    {
        if(last_controllers != nullptr)
        last_controllers->Update( gpio, status, dt );
    }

}

void ControllerObject::onRead()
{
    emergency_break_timer.restart();

    QByteArray arr =  socket->readAll();

    for( int i = 0; i < arr.length(); i++)
    {
        status->Status()->PushByte( arr.at(i) );
        uchar sdl = status->Status()->SendDataLength();
        if(sdl > 0)
        {
            uchar *data = status->Status()->FlushSendData();

            socket->write((const char*) data, (uint)sdl);
        }
    }
}

GPIO * ControllerObject::realGPIO = nullptr;

