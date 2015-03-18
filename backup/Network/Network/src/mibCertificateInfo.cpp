#include "inc/mibCertificateInfo.h"

using namespace mibot;

CertificateInfo::CertificateInfo()
{}

CertificateInfo::CertificateInfo(QSslCertificate &cert)
{
    Common =            cert.issuerInfo( QSslCertificate::CommonName );
    Organization =      cert.issuerInfo( QSslCertificate::Organization );
    OrganizationUnit =  cert.issuerInfo( QSslCertificate::OrganizationalUnitName );
    Locality =          cert.issuerInfo( QSslCertificate::LocalityName );
    Province =          cert.issuerInfo( QSslCertificate::StateOrProvinceName );
    EmailAddress =      cert.issuerInfo( QSslCertificate::EmailAddress );
    Country =           cert.issuerInfo( QSslCertificate::CountryName );

    EffectiveDate = cert.effectiveDate();
    ExpiryDate =    cert.expiryDate();
    IsValid =      !cert.isNull();
}


CertificateInfo::~CertificateInfo() {}

