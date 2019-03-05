/****************************************************************************
** Meta object code from reading C++ file 'IfcTreeWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui/IfcTreeWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'IfcTreeWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_IfcTreeWidget_t {
    QByteArrayData data[15];
    char stringdata0[249];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IfcTreeWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IfcTreeWidget_t qt_meta_stringdata_IfcTreeWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "IfcTreeWidget"
QT_MOC_LITERAL(1, 14, 19), // "slotObjectsSelected"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 42), // "std::map<int,shared_ptr<Build..."
QT_MOC_LITERAL(4, 78, 3), // "map"
QT_MOC_LITERAL(5, 82, 30), // "slotTreewidgetSelectionChanged"
QT_MOC_LITERAL(6, 113, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(7, 130, 7), // "current"
QT_MOC_LITERAL(8, 138, 8), // "previous"
QT_MOC_LITERAL(9, 147, 29), // "slotTreeWidgetItemDoubleClick"
QT_MOC_LITERAL(10, 177, 4), // "item"
QT_MOC_LITERAL(11, 182, 6), // "column"
QT_MOC_LITERAL(12, 189, 16), // "slotModelCleared"
QT_MOC_LITERAL(13, 206, 21), // "slotModelLoadingStart"
QT_MOC_LITERAL(14, 228, 20) // "slotModelLoadingDone"

    },
    "IfcTreeWidget\0slotObjectsSelected\0\0"
    "std::map<int,shared_ptr<BuildingEntity> >&\0"
    "map\0slotTreewidgetSelectionChanged\0"
    "QTreeWidgetItem*\0current\0previous\0"
    "slotTreeWidgetItemDoubleClick\0item\0"
    "column\0slotModelCleared\0slotModelLoadingStart\0"
    "slotModelLoadingDone"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IfcTreeWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x0a /* Public */,
       5,    2,   52,    2, 0x0a /* Public */,
       5,    0,   57,    2, 0x0a /* Public */,
       9,    2,   58,    2, 0x0a /* Public */,
      12,    0,   63,    2, 0x0a /* Public */,
      13,    0,   64,    2, 0x0a /* Public */,
      14,    0,   65,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 6,    7,    8,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6, QMetaType::Int,   10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void IfcTreeWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IfcTreeWidget *_t = static_cast<IfcTreeWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotObjectsSelected((*reinterpret_cast< std::map<int,shared_ptr<BuildingEntity> >(*)>(_a[1]))); break;
        case 1: _t->slotTreewidgetSelectionChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        case 2: _t->slotTreewidgetSelectionChanged(); break;
        case 3: _t->slotTreeWidgetItemDoubleClick((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->slotModelCleared(); break;
        case 5: _t->slotModelLoadingStart(); break;
        case 6: _t->slotModelLoadingDone(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject IfcTreeWidget::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_IfcTreeWidget.data,
      qt_meta_data_IfcTreeWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *IfcTreeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IfcTreeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_IfcTreeWidget.stringdata0))
        return static_cast<void*>(this);
    return QTreeWidget::qt_metacast(_clname);
}

int IfcTreeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
