/****************************************************************************
** Meta object code from reading C++ file 'footabbar.hpp'
**
** Created: Tue Mar 24 13:34:12 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "footabbar.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'footabbar.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FooTabBar[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      20,   10,   10,   10, 0x05,
      34,   10,   10,   10, 0x05,
      48,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      68,   10,   10,   10, 0x08,
      79,   10,   10,   10, 0x08,
      90,   10,   10,   10, 0x08,
     107,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FooTabBar[] = {
    "FooTabBar\0\0newTab()\0cloneTab(int)\0"
    "closeTab(int)\0closeOtherTabs(int)\0"
    "cloneTab()\0closeTab()\0closeOtherTabs()\0"
    "contextMenuRequested(QPoint)\0"
};

const QMetaObject FooTabBar::staticMetaObject = {
    { &QTabBar::staticMetaObject, qt_meta_stringdata_FooTabBar,
      qt_meta_data_FooTabBar, 0 }
};

const QMetaObject *FooTabBar::metaObject() const
{
    return &staticMetaObject;
}

void *FooTabBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FooTabBar))
        return static_cast<void*>(const_cast< FooTabBar*>(this));
    return QTabBar::qt_metacast(_clname);
}

int FooTabBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: newTab(); break;
        case 1: cloneTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: closeTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: closeOtherTabs((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: cloneTab(); break;
        case 5: closeTab(); break;
        case 6: closeOtherTabs(); break;
        case 7: contextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void FooTabBar::newTab()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void FooTabBar::cloneTab(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FooTabBar::closeTab(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FooTabBar::closeOtherTabs(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
