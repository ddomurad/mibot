/****************************************************************************
** Meta object code from reading C++ file 'LoggerUnitTests.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UtilsUnitTests/LoggerUnitTests.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LoggerUnitTests.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_mibot__ut__LoggerUnitTests_t {
    QByteArrayData data[14];
    char stringdata[450];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_mibot__ut__LoggerUnitTests_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_mibot__ut__LoggerUnitTests_t qt_meta_stringdata_mibot__ut__LoggerUnitTests = {
    {
QT_MOC_LITERAL(0, 0, 26), // "mibot::ut::LoggerUnitTests"
QT_MOC_LITERAL(1, 27, 16), // "CanCreateChannel"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 25), // "CanCreateMultipleChannels"
QT_MOC_LITERAL(4, 71, 33), // "FirstCreatedChannelIsSetAsDef..."
QT_MOC_LITERAL(5, 105, 26), // "DefaultChannelCanBeChanged"
QT_MOC_LITERAL(6, 132, 23), // "MessageIsWritenToOutput"
QT_MOC_LITERAL(7, 156, 43), // "MessageIsWritenToOutputWhenTh..."
QT_MOC_LITERAL(8, 200, 45), // "MessageIsNOTWritenToOutputWhe..."
QT_MOC_LITERAL(9, 246, 37), // "FileOutputCorectlyWriteMessag..."
QT_MOC_LITERAL(10, 284, 41), // "SimpleFormaterCorrectlyTransl..."
QT_MOC_LITERAL(11, 326, 38), // "SimpleFormaterCorrectlyFormat..."
QT_MOC_LITERAL(12, 365, 38), // "SimpleFormaterCorrectlyFormat..."
QT_MOC_LITERAL(13, 404, 45) // "SimpleConsoleFormaterCorrectl..."

    },
    "mibot::ut::LoggerUnitTests\0CanCreateChannel\0"
    "\0CanCreateMultipleChannels\0"
    "FirstCreatedChannelIsSetAsDefault\0"
    "DefaultChannelCanBeChanged\0"
    "MessageIsWritenToOutput\0"
    "MessageIsWritenToOutputWhenTheLevelIsHigher\0"
    "MessageIsNOTWritenToOutputWhenTheLevelIsLower\0"
    "FileOutputCorectlyWriteMessagesToFile\0"
    "SimpleFormaterCorrectlyTranslateLogLevels\0"
    "SimpleFormaterCorrectlyFormatsMessages\0"
    "SimpleFormaterCorrectlyFormatsFilePath\0"
    "SimpleConsoleFormaterCorrectlyFormatsMessages"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mibot__ut__LoggerUnitTests[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x0a /* Public */,
       3,    0,   75,    2, 0x0a /* Public */,
       4,    0,   76,    2, 0x0a /* Public */,
       5,    0,   77,    2, 0x0a /* Public */,
       6,    0,   78,    2, 0x0a /* Public */,
       7,    0,   79,    2, 0x0a /* Public */,
       8,    0,   80,    2, 0x0a /* Public */,
       9,    0,   81,    2, 0x0a /* Public */,
      10,    0,   82,    2, 0x0a /* Public */,
      11,    0,   83,    2, 0x0a /* Public */,
      12,    0,   84,    2, 0x0a /* Public */,
      13,    0,   85,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void mibot::ut::LoggerUnitTests::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LoggerUnitTests *_t = static_cast<LoggerUnitTests *>(_o);
        switch (_id) {
        case 0: _t->CanCreateChannel(); break;
        case 1: _t->CanCreateMultipleChannels(); break;
        case 2: _t->FirstCreatedChannelIsSetAsDefault(); break;
        case 3: _t->DefaultChannelCanBeChanged(); break;
        case 4: _t->MessageIsWritenToOutput(); break;
        case 5: _t->MessageIsWritenToOutputWhenTheLevelIsHigher(); break;
        case 6: _t->MessageIsNOTWritenToOutputWhenTheLevelIsLower(); break;
        case 7: _t->FileOutputCorectlyWriteMessagesToFile(); break;
        case 8: _t->SimpleFormaterCorrectlyTranslateLogLevels(); break;
        case 9: _t->SimpleFormaterCorrectlyFormatsMessages(); break;
        case 10: _t->SimpleFormaterCorrectlyFormatsFilePath(); break;
        case 11: _t->SimpleConsoleFormaterCorrectlyFormatsMessages(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject mibot::ut::LoggerUnitTests::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_mibot__ut__LoggerUnitTests.data,
      qt_meta_data_mibot__ut__LoggerUnitTests,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *mibot::ut::LoggerUnitTests::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mibot::ut::LoggerUnitTests::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_mibot__ut__LoggerUnitTests.stringdata))
        return static_cast<void*>(const_cast< LoggerUnitTests*>(this));
    if (!strcmp(_clname, "UnitTest"))
        return static_cast< UnitTest*>(const_cast< LoggerUnitTests*>(this));
    return QObject::qt_metacast(_clname);
}

int mibot::ut::LoggerUnitTests::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
