/****************************************************************************
** Meta object code from reading C++ file 'TabReadWrite.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui/TabReadWrite.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TabReadWrite.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TabReadWrite_t {
    QByteArrayData data[21];
    char stringdata0[344];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TabReadWrite_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TabReadWrite_t qt_meta_stringdata_TabReadWrite = {
    {
QT_MOC_LITERAL(0, 0, 12), // "TabReadWrite"
QT_MOC_LITERAL(1, 13, 10), // "slotTxtOut"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 3), // "txt"
QT_MOC_LITERAL(4, 29, 17), // "slotTxtOutWarning"
QT_MOC_LITERAL(5, 47, 15), // "slotTxtOutError"
QT_MOC_LITERAL(6, 63, 17), // "slotProgressValue"
QT_MOC_LITERAL(7, 81, 14), // "progress_value"
QT_MOC_LITERAL(8, 96, 11), // "std::string"
QT_MOC_LITERAL(9, 108, 13), // "progress_type"
QT_MOC_LITERAL(10, 122, 15), // "slotClearTxtOut"
QT_MOC_LITERAL(11, 138, 15), // "slotLoadIfcFile"
QT_MOC_LITERAL(12, 154, 8), // "QString&"
QT_MOC_LITERAL(13, 163, 4), // "path"
QT_MOC_LITERAL(14, 168, 27), // "slotRecentFilesIndexChanged"
QT_MOC_LITERAL(15, 196, 22), // "updateRecentFilesCombo"
QT_MOC_LITERAL(16, 219, 26), // "slotAddOtherIfcFileClicked"
QT_MOC_LITERAL(17, 246, 28), // "slotLoadRecentIfcFileClicked"
QT_MOC_LITERAL(18, 275, 23), // "slotSetWritePathClicked"
QT_MOC_LITERAL(19, 299, 20), // "slotWriteFileClicked"
QT_MOC_LITERAL(20, 320, 23) // "slotClearRecentIfcFiles"

    },
    "TabReadWrite\0slotTxtOut\0\0txt\0"
    "slotTxtOutWarning\0slotTxtOutError\0"
    "slotProgressValue\0progress_value\0"
    "std::string\0progress_type\0slotClearTxtOut\0"
    "slotLoadIfcFile\0QString&\0path\0"
    "slotRecentFilesIndexChanged\0"
    "updateRecentFilesCombo\0"
    "slotAddOtherIfcFileClicked\0"
    "slotLoadRecentIfcFileClicked\0"
    "slotSetWritePathClicked\0slotWriteFileClicked\0"
    "slotClearRecentIfcFiles"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TabReadWrite[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x0a /* Public */,
       4,    1,   82,    2, 0x0a /* Public */,
       5,    1,   85,    2, 0x0a /* Public */,
       6,    2,   88,    2, 0x0a /* Public */,
      10,    0,   93,    2, 0x0a /* Public */,
      11,    1,   94,    2, 0x0a /* Public */,
      14,    1,   97,    2, 0x0a /* Public */,
      15,    0,  100,    2, 0x0a /* Public */,
      16,    0,  101,    2, 0x08 /* Private */,
      17,    0,  102,    2, 0x08 /* Private */,
      18,    0,  103,    2, 0x08 /* Private */,
      19,    0,  104,    2, 0x08 /* Private */,
      20,    0,  105,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Double, 0x80000000 | 8,    7,    9,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TabReadWrite::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TabReadWrite *_t = static_cast<TabReadWrite *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotTxtOut((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->slotTxtOutWarning((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->slotTxtOutError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->slotProgressValue((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 4: _t->slotClearTxtOut(); break;
        case 5: _t->slotLoadIfcFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->slotRecentFilesIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->updateRecentFilesCombo(); break;
        case 8: _t->slotAddOtherIfcFileClicked(); break;
        case 9: _t->slotLoadRecentIfcFileClicked(); break;
        case 10: _t->slotSetWritePathClicked(); break;
        case 11: _t->slotWriteFileClicked(); break;
        case 12: _t->slotClearRecentIfcFiles(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TabReadWrite::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TabReadWrite.data,
      qt_meta_data_TabReadWrite,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TabReadWrite::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TabReadWrite::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TabReadWrite.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TabReadWrite::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
