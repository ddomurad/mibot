#ifndef CERTIFICATEINFO_H
#define CERTIFICATEINFO_H

#include <QtNetwork>

namespace mibot
{

class CertificateInfo
{
public:
    CertificateInfo();
    CertificateInfo(QSslCertificate*);
    ~CertificateInfo();

    QStringList Country;
    QStringList Province;
    QStringList Locality;
    QStringList Organization;
    QStringList OrganizationUnit;
    QStringList Common;
    QStringList EmailAddress;

    QDateTime EffectiveDate;
    QDateTime ExpiryDate;

    bool      IsValid;

};

}
#endif // CERTIFICATEINFO_H
