/****************************************************************************
** Meta object code from reading C++ file 'TabView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui/TabView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TabView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TabView_t {
    QByteArrayData data[9];
    char stringdata0[131];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TabView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TabView_t qt_meta_stringdata_TabView = {
    {
QT_MOC_LITERAL(0, 0, 7), // "TabView"
QT_MOC_LITERAL(1, 8, 20), // "slotToggleSceneLight"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 18), // "slotCullFrontFaces"
QT_MOC_LITERAL(4, 49, 5), // "state"
QT_MOC_LITERAL(5, 55, 17), // "slotCullBackFaces"
QT_MOC_LITERAL(6, 73, 18), // "slotSetNumVertices"
QT_MOC_LITERAL(7, 92, 14), // "slotShowCurves"
QT_MOC_LITERAL(8, 107, 23) // "slotIgnoreProfileRadius"

    },
    "TabView\0slotToggleSceneLight\0\0"
    "slotCullFrontFaces\0state\0slotCullBackFaces\0"
    "slotSetNumVertices\0slotShowCurves\0"
    "slotIgnoreProfileRadius"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TabView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08 /* Private */,
       3,    1,   45,    2, 0x08 /* Private */,
       5,    1,   48,    2, 0x08 /* Private */,
       6,    1,   51,    2, 0x08 /* Private */,
       7,    1,   54,    2, 0x08 /* Private */,
       8,    1,   57,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void TabView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TabView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotToggleSceneLight(); break;
        case 1: _t->slotCullFrontFaces((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->slotCullBackFaces((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->slotSetNumVertices((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->slotShowCurves((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->slotIgnoreProfileRadius((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TabView::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_TabView.data,
    qt_meta_data_TabView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TabView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TabView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TabView.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TabView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
