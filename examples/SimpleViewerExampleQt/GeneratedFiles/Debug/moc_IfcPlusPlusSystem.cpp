/****************************************************************************
** Meta object code from reading C++ file 'IfcPlusPlusSystem.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/IfcPlusPlusSystem.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'IfcPlusPlusSystem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_IfcPlusPlusSystem_t {
    QByteArrayData data[9];
    char stringdata0[186];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IfcPlusPlusSystem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IfcPlusPlusSystem_t qt_meta_stringdata_IfcPlusPlusSystem = {
    {
QT_MOC_LITERAL(0, 0, 17), // "IfcPlusPlusSystem"
QT_MOC_LITERAL(1, 18, 21), // "signalObjectsSelected"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 42), // "std::map<int,shared_ptr<Build..."
QT_MOC_LITERAL(4, 84, 11), // "map_objects"
QT_MOC_LITERAL(5, 96, 23), // "signalObjectsUnselected"
QT_MOC_LITERAL(6, 120, 18), // "signalModelCleared"
QT_MOC_LITERAL(7, 139, 23), // "signalModelLoadingStart"
QT_MOC_LITERAL(8, 163, 22) // "signalModelLoadingDone"

    },
    "IfcPlusPlusSystem\0signalObjectsSelected\0"
    "\0std::map<int,shared_ptr<BuildingEntity> >&\0"
    "map_objects\0signalObjectsUnselected\0"
    "signalModelCleared\0signalModelLoadingStart\0"
    "signalModelLoadingDone"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IfcPlusPlusSystem[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       5,    1,   42,    2, 0x06 /* Public */,
       6,    0,   45,    2, 0x06 /* Public */,
       7,    0,   46,    2, 0x06 /* Public */,
       8,    0,   47,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void IfcPlusPlusSystem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IfcPlusPlusSystem *_t = static_cast<IfcPlusPlusSystem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalObjectsSelected((*reinterpret_cast< std::map<int,shared_ptr<BuildingEntity> >(*)>(_a[1]))); break;
        case 1: _t->signalObjectsUnselected((*reinterpret_cast< std::map<int,shared_ptr<BuildingEntity> >(*)>(_a[1]))); break;
        case 2: _t->signalModelCleared(); break;
        case 3: _t->signalModelLoadingStart(); break;
        case 4: _t->signalModelLoadingDone(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (IfcPlusPlusSystem::*_t)(std::map<int,shared_ptr<BuildingEntity> > & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IfcPlusPlusSystem::signalObjectsSelected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (IfcPlusPlusSystem::*_t)(std::map<int,shared_ptr<BuildingEntity> > & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IfcPlusPlusSystem::signalObjectsUnselected)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (IfcPlusPlusSystem::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IfcPlusPlusSystem::signalModelCleared)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (IfcPlusPlusSystem::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IfcPlusPlusSystem::signalModelLoadingStart)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (IfcPlusPlusSystem::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IfcPlusPlusSystem::signalModelLoadingDone)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject IfcPlusPlusSystem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IfcPlusPlusSystem.data,
      qt_meta_data_IfcPlusPlusSystem,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *IfcPlusPlusSystem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IfcPlusPlusSystem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_IfcPlusPlusSystem.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "osgGA::GUIEventHandler"))
        return static_cast< osgGA::GUIEventHandler*>(this);
    return QObject::qt_metacast(_clname);
}

int IfcPlusPlusSystem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void IfcPlusPlusSystem::signalObjectsSelected(std::map<int,shared_ptr<BuildingEntity> > & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void IfcPlusPlusSystem::signalObjectsUnselected(std::map<int,shared_ptr<BuildingEntity> > & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void IfcPlusPlusSystem::signalModelCleared()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void IfcPlusPlusSystem::signalModelLoadingStart()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void IfcPlusPlusSystem::signalModelLoadingDone()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
