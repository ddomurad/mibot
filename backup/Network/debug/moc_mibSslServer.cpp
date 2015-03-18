/****************************************************************************
** Meta object code from reading C++ file 'mibSslServer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Network/inc/mibSslServer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mibSslServer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_mibot__SslPoweredTcpServer_t {
    QByteArrayData data[1];
    char stringdata[27];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_mibot__SslPoweredTcpServer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_mibot__SslPoweredTcpServer_t qt_meta_stringdata_mibot__SslPoweredTcpServer = {
    {
QT_MOC_LITERAL(0, 0, 26) // "mibot::SslPoweredTcpServer"

    },
    "mibot::SslPoweredTcpServer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mibot__SslPoweredTcpServer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void mibot::SslPoweredTcpServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject mibot::SslPoweredTcpServer::staticMetaObject = {
    { &QTcpServer::staticMetaObject, qt_meta_stringdata_mibot__SslPoweredTcpServer.data,
      qt_meta_data_mibot__SslPoweredTcpServer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *mibot::SslPoweredTcpServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mibot::SslPoweredTcpServer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_mibot__SslPoweredTcpServer.stringdata))
        return static_cast<void*>(const_cast< SslPoweredTcpServer*>(this));
    return QTcpServer::qt_metacast(_clname);
}

int mibot::SslPoweredTcpServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_mibot__SslServer_t {
    QByteArrayData data[16];
    char stringdata[198];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_mibot__SslServer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_mibot__SslServer_t qt_meta_stringdata_mibot__SslServer = {
    {
QT_MOC_LITERAL(0, 0, 16), // "mibot::SslServer"
QT_MOC_LITERAL(1, 17, 11), // "StartServer"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 12), // "QHostAddress"
QT_MOC_LITERAL(4, 43, 4), // "addr"
QT_MOC_LITERAL(5, 48, 9), // "u_int16_t"
QT_MOC_LITERAL(6, 58, 4), // "port"
QT_MOC_LITERAL(7, 63, 10), // "StopServer"
QT_MOC_LITERAL(8, 74, 11), // "onSslErrors"
QT_MOC_LITERAL(9, 86, 16), // "QList<QSslError>"
QT_MOC_LITERAL(10, 103, 6), // "errors"
QT_MOC_LITERAL(11, 110, 13), // "onAcceptError"
QT_MOC_LITERAL(12, 124, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(13, 153, 5), // "error"
QT_MOC_LITERAL(14, 159, 20), // "onIncomingConnection"
QT_MOC_LITERAL(15, 180, 17) // "onClientEncrypted"

    },
    "mibot::SslServer\0StartServer\0\0"
    "QHostAddress\0addr\0u_int16_t\0port\0"
    "StopServer\0onSslErrors\0QList<QSslError>\0"
    "errors\0onAcceptError\0QAbstractSocket::SocketError\0"
    "error\0onIncomingConnection\0onClientEncrypted"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mibot__SslServer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x0a /* Public */,
       7,    0,   49,    2, 0x0a /* Public */,
       8,    1,   50,    2, 0x08 /* Private */,
      11,    1,   53,    2, 0x08 /* Private */,
      14,    0,   56,    2, 0x08 /* Private */,
      15,    0,   57,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void mibot::SslServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SslServer *_t = static_cast<SslServer *>(_o);
        switch (_id) {
        case 0: _t->StartServer((*reinterpret_cast< QHostAddress(*)>(_a[1])),(*reinterpret_cast< u_int16_t(*)>(_a[2]))); break;
        case 1: _t->StopServer(); break;
        case 2: _t->onSslErrors((*reinterpret_cast< QList<QSslError>(*)>(_a[1]))); break;
        case 3: _t->onAcceptError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 4: _t->onIncomingConnection(); break;
        case 5: _t->onClientEncrypted(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QSslError> >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    }
}

const QMetaObject mibot::SslServer::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_mibot__SslServer.data,
      qt_meta_data_mibot__SslServer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *mibot::SslServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mibot::SslServer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_mibot__SslServer.stringdata))
        return static_cast<void*>(const_cast< SslServer*>(this));
    return QThread::qt_metacast(_clname);
}

int mibot::SslServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
