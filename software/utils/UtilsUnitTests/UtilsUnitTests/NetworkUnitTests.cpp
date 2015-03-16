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
    Assert.IsTrue( server->LoadServerKeyFile("../UtilsUnitTests/res/server_key_no_passphrase.key") );
}

void NetworkUnitTests::TryAddNotExistingServerKeyFileToServerWithoutPassphrase()
{
    Assert.IsFale( server->LoadServerKeyFile("./some_name.key") );

    QString error = server->NextError();
    Assert.AreEqual<QString>("Can't open given SslKey.", error);

    error = server->NextError();
    Assert.AreEqual<QString>(QString(), error);
}

void NetworkUnitTests::TryAddInvalidServerKeyFileToServerWithoutPassphrase()
{
    Assert.IsFale( server->LoadServerKeyFile("../UtilsUnitTests/res/invalid_no_passphere.key") );

    QString error = server->NextError();
    Assert.AreEqual<QString>("Can't open given SslKey.", error);

    error = server->NextError();
    Assert.AreEqual<QString>(QString(), error);
}

void NetworkUnitTests::SelectServerKeyFileToServerWithPassphrase()
{
    Assert.IsTrue( server->LoadServerKeyFile("../UtilsUnitTests/res/server_key.key", "pass") );
}

void NetworkUnitTests::TryAddNotExistingServerKeyFileToServerWithPassphrase()
{
    Assert.IsFale( server->LoadServerKeyFile("./some_name.key","pass") );

    QString error = server->NextError();
    Assert.AreEqual<QString>("Can't open given SslKey.", error);

    error = server->NextError();
    Assert.AreEqual<QString>(QString(), error);
}

void NetworkUnitTests::TryAddInvalidServerKeyFileToServerWithPassphrase()
{
    Assert.IsFale( server->LoadServerKeyFile("../UtilsUnitTests/res/invalid_server_key.key", "pass") );

    QString error = server->NextError();
    Assert.AreEqual<QString>("Can't open given SslKey.", error);

    error = server->NextError();
    Assert.AreEqual<QString>(QString(), error);
}

void NetworkUnitTests::TryAddServerKeyFileToServerWithInvalidPassphrase()
{
    Assert.IsFale( server->LoadServerKeyFile("../UtilsUnitTests/res/server_key.key", "pass1") );

    QString error = server->NextError();
    Assert.AreEqual<QString>("Can't open given SslKey.", error);

    error = server->NextError();
    Assert.AreEqual<QString>(QString(), error);
}

void NetworkUnitTests::SelectServerCertificateFile()
{
    Assert.IsTrue( server->LoadServerCertificate("../UtilsUnitTests/res/server_certificate.crt") );
}

void NetworkUnitTests::TryAddNotExistingServerCertificateFile()
{
    Assert.IsFale( server->LoadServerCertificate("../some_file.crt") );

    QString error = server->NextError();
    Assert.AreEqual<QString>("Can't open given SslCertificate.", error);

    error = server->NextError();
    Assert.AreEqual<QString>(QString(), error);
}

void NetworkUnitTests::TryAddInvalidServerCertificateFile()
{
    Assert.IsFale( server->LoadServerCertificate("../UtilsUnitTests/res/server_invalid_certificate.crt") );

    QString error = server->NextError();
    Assert.AreEqual<QString>("Can't open given SslCertificate.", error);

    error = server->NextError();
    Assert.AreEqual<QString>(QString(), error);
}

void NetworkUnitTests::ReadInfoFromServerCertificate()
{
    Assert.IsTrue( server->LoadServerCertificate("../UtilsUnitTests/res/server_certificate.crt") );
    CertificateInfo ci = server->ServerCertificateInfo();

    Assert.IsTrue( ci.IsValid );

    Assert.AreEqual<QString>( "pl", ci.Country[0] );
    Assert.AreEqual<QString>( "_test_common_name", ci.Common[0] );
    Assert.AreEqual<QString>( "test_city", ci.Locality[0] );
    Assert.AreEqual<QString>( "test_organ", ci.Organization[0] );
    Assert.AreEqual<QString>( "test_name", ci.OrganizationUnit[0] );
    Assert.AreEqual<QString>( "test_email@e.com", ci.EmailAddress[0] );

    Assert.AreEqual<int>( 356, ci.EffectiveDate.daysTo(ci.ExpiryDate) );
}

void NetworkUnitTests::AddClientCertificateFile()
{
    Assert.AreEqual<int>(0, server->ClientCertificateCount());
    Assert.IsTrue( server->AddClientCertificate("../UtilsUnitTests/res/client_certificate.crt") );
    Assert.AreEqual<int>(1, server->ClientCertificateCount());
}

void NetworkUnitTests::TryAddNotExistingClientCertificateFile()
{
    Assert.AreEqual<int>(0, server->ClientCertificateCount());
    Assert.IsFale( server->AddClientCertificate("../client.crt") );
    Assert.AreEqual<int>(0, server->ClientCertificateCount());

    QString error = server->NextError();
    Assert.AreEqual<QString>( "Can't open given SslCertificate.", error );

    error = server->NextError();
    Assert.AreEqual<QString>( QString(), error );
}

void NetworkUnitTests::TestInit()
{
    server = new SslServer();
}

void NetworkUnitTests::TestClean()
{
    delete server;
}

