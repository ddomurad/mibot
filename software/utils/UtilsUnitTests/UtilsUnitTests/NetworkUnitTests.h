#ifndef NETWORKUNITTESTS_H
#define NETWORKUNITTESTS_H

#include <mibLogger.h>
#include <mibNetwork.h>

#include <QObject>
#include <mibUnitTest.h>

namespace mibot
{
namespace ut
{

class NetworkUnitTests : public QObject, public UnitTest
{
    Q_OBJECT
public:
    NetworkUnitTests();
    ~NetworkUnitTests();

public slots:
    void SelectServerKeyFileToServerWithoutPassphrase();
    void TryAddNotExistingServerKeyFileToServerWithoutPassphrase();
    void TryAddInvalidServerKeyFileToServerWithoutPassphrase();

    void SelectServerKeyFileToServerWithPassphrase();
    void TryAddNotExistingServerKeyFileToServerWithPassphrase();
    void TryAddInvalidServerKeyFileToServerWithPassphrase();
    void TryAddServerKeyFileToServerWithInvalidPassphrase();

    void SelectServerCertificateFile();
    void TryAddNotExistingServerCertificateFile();
    void TryAddInvalidServerCertificateFile();

    void ReadInfoFromServerCertificate();

    void AddClientCertificateFile();
    void TryAddNotExistingClientCertificateFile();



public:
    void TestInit();
    void TestClean();

private:
    SslServer * server;
};

}
}
#endif // NETWORKUNITTESTS_H
