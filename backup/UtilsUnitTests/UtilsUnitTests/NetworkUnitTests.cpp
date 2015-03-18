#include "NetworkUnitTests.h"

using namespace mibot;
using namespace mibot::ut;

NetworkUnitTests::NetworkUnitTests() :
    QObject(nullptr)
{}

NetworkUnitTests::~NetworkUnitTests()
{}

void NetworkUnitTests::SelectServerKeyFileToServerWithoutPassphrase()
{
    Assert.IsTrue when( server->LoadServerKeyFile("../UtilsUnitTests/res/server_key_no_passphrase.key") );
}

void NetworkUnitTests::TryAddNotExistingServerKeyFileToServerWithoutPassphrase()
{
    Assert.IsFalse when( server->LoadServerKeyFile("./some_name.key") );

    QString error = server->NextError();
    Assert.AreEqual<QString> when("Can't open given SslKey.", error);

    error = server->NextError();
    Assert.AreEqual<QString> when(QString(), error);
}

void NetworkUnitTests::TryAddInvalidServerKeyFileToServerWithoutPassphrase()
{
    Assert.IsFalse when( server->LoadServerKeyFile("../UtilsUnitTests/res/invalid_no_passphere.key") );

    QString error = server->NextError();
    Assert.AreEqual<QString> when("Can't open given SslKey.", error);

    error = server->NextError();
    Assert.AreEqual<QString> when(QString(), error);
}

void NetworkUnitTests::SelectServerKeyFileToServerWithPassphrase()
{
    Assert.IsTrue when( server->LoadServerKeyFile("../UtilsUnitTests/res/server_key.key", "pass") );
}

void NetworkUnitTests::TryAddNotExistingServerKeyFileToServerWithPassphrase()
{
    Assert.IsFalse when( server->LoadServerKeyFile("./some_name.key","pass") );

    QString error = server->NextError();
    Assert.AreEqual<QString> when("Can't open given SslKey.", error);

    error = server->NextError();
    Assert.AreEqual<QString> when(QString(), error);
}

void NetworkUnitTests::TryAddInvalidServerKeyFileToServerWithPassphrase()
{
    Assert.IsFalse when( server->LoadServerKeyFile("../UtilsUnitTests/res/invalid_server_key.key", "pass") );

    QString error = server->NextError();
    Assert.AreEqual<QString> when("Can't open given SslKey.", error);

    error = server->NextError();
    Assert.AreEqual<QString> when(QString(), error);
}

void NetworkUnitTests::TryAddServerKeyFileToServerWithInvalidPassphrase()
{
    Assert.IsFalse when( server->LoadServerKeyFile("../UtilsUnitTests/res/server_key.key", "pass1") );

    QString error = server->NextError();
    Assert.AreEqual<QString> when("Can't open given SslKey.", error);

    error = server->NextError();
    Assert.AreEqual<QString> when(QString(), error);
}

void NetworkUnitTests::SelectServerCertificateFile()
{
    Assert.IsTrue when( server->LoadServerCertificate("../UtilsUnitTests/res/server_certificate.crt") );
}

void NetworkUnitTests::TryAddNotExistingServerCertificateFile()
{
    Assert.IsFalse when( server->LoadServerCertificate("../some_file.crt") );

    QString error = server->NextError();
    Assert.AreEqual<QString> when("Can't open given SslCertificate.", error);

    error = server->NextError();
    Assert.AreEqual<QString> when(QString(), error);
}

void NetworkUnitTests::TryAddInvalidServerCertificateFile()
{
    Assert.IsFalse when( server->LoadServerCertificate("../UtilsUnitTests/res/server_invalid_certificate.crt") );

    QString error = server->NextError();
    Assert.AreEqual<QString> when("Can't open given SslCertificate.", error);

    error = server->NextError();
    Assert.AreEqual<QString> when(QString(), error);
}

void NetworkUnitTests::ReadInfoFromServerCertificate()
{
    Assert.IsTrue when( server->LoadServerCertificate("../UtilsUnitTests/res/server_certificate.crt") );
    CertificateInfo ci = server->ServerCertificateInfo();

    Assert.IsTrue when( ci.IsValid );

    Assert.AreEqual<QString> when( "pl", ci.Country[0] );
    Assert.AreEqual<QString> when( "_test_common_name", ci.Common[0] );
    Assert.AreEqual<QString> when( "test_city", ci.Locality[0] );
    Assert.AreEqual<QString> when( "test_organ", ci.Organization[0] );
    Assert.AreEqual<QString> when( "test_name", ci.OrganizationUnit[0] );
    Assert.AreEqual<QString> when( "test_email@e.com", ci.EmailAddress[0] );

    Assert.AreEqual<int> when( 356, ci.EffectiveDate.daysTo(ci.ExpiryDate) );
}

void NetworkUnitTests::AddClientCertificateFile()
{
    Assert.AreEqual<int> when(0, server->ClientCertificateCount());
    Assert.IsTrue when( server->AddClientCertificate("../UtilsUnitTests/res/client_certificate.crt") );
    Assert.AreEqual<int> when(1, server->ClientCertificateCount());
}

void NetworkUnitTests::TryAddNotExistingClientCertificateFile()
{
    Assert.AreEqual<int> when(0, server->ClientCertificateCount());
    Assert.IsFalse when( server->AddClientCertificate("../client.crt") );
    Assert.AreEqual<int> when(0, server->ClientCertificateCount());

    QString error = server->NextError();
    Assert.AreEqual<QString> when( "Can't open given SslCertificate.", error );

    error = server->NextError();
    Assert.AreEqual<QString> when( QString(), error );
}

void NetworkUnitTests::StartListenForClientsOnAnyIp()
{
    loadCertificates(server);

    server->StartServer(QHostAddress::Any, serverPort);
    Assert.IsFalse when( server->AreErrors() );
}

void NetworkUnitTests::StartListenForClientsOnAnyIp4()
{
    loadCertificates(server);
    server->StartServer(QHostAddress::AnyIPv4, serverPort);
    Assert.IsFalse when( server->AreErrors() );
}

void NetworkUnitTests::StartListenForClientsOnAnyIp6()
{
    loadCertificates(server);
    server->StartServer(QHostAddress::AnyIPv6, serverPort);
    Assert.IsFalse when( server->AreErrors() );
}

void NetworkUnitTests::CanStart2ListenersOnTheDiferentPortsAnyIp()
{
    loadCertificates(server);
    server->StartServer(QHostAddress::Any, serverPort);
    Assert.IsFalse when( server->AreErrors() );

    auto server2 = new SslServer();
    loadCertificates(server2);
    server2->StartServer(QHostAddress::Any, serverPort + 1);
    Assert.IsFalse when( server2->AreErrors() );

    delete server2;
}

void NetworkUnitTests::CanNotStart2ListenersOnTheSamePortsAnyIp()
{
    loadCertificates(server);
    server->StartServer(QHostAddress::Any, serverPort);
    Assert.IsFalse when( server->AreErrors() );

    auto server2 = new SslServer();

    loadCertificates(server2);
    server2->StartServer(QHostAddress::Any, serverPort);
    Assert.IsTrue when( server2->AreErrors() );

    delete server2;
}

void NetworkUnitTests::SslErrorsAreGeneratedWhenConnectionWithoutAnyCertificateAndKey()
{
    loadCertificates(server);

    server->StartServer(QHostAddress::Any, serverPort);
    Assert.IsFalse when( server->AreErrors() );

    QThread::sleep(1);

    QSslSocket clientSocket;
    clientSocket.connectToHostEncrypted( "192.168.0.4", serverPort);

    QThread::sleep(1);

    Assert.AreNotEqual<QString> when(QString(), server->NextError());
}

void NetworkUnitTests::TestInit()
{
    server = new SslServer();
    serverPort = 10400;
}

void NetworkUnitTests::TestClean()
{
    server->StopServer();
    delete server;
}

void NetworkUnitTests::loadCertificates(SslServer * s)
{
    Assert.IsTrue when( s->LoadServerCertificate("../UtilsUnitTests/res/server_certificate.crt") );

    Assert.IsTrue when( s->AddClientCertificate("../UtilsUnitTests/res/client_certificate.crt") );
    Assert.AreEqual<int> when(1, s->ClientCertificateCount());
}

