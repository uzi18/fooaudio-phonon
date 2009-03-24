/****************************************************************************
** Meta object code from reading C++ file 'footabwidget.hpp'
**
** Created: Tue Mar 24 13:34:15 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "footabwidget.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'footabwidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FooTabWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      28,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,   13,   13,   13, 0x0a,
      59,   53,   13,   13, 0x0a,
      73,   13,   13,   13, 0x2a,
      84,   13,   13,   13, 0x0a,
     104,   13,   13,   13, 0x0a,
     114,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FooTabWidget[] = {
    "FooTabWidget\0\0tabsChanged()\0lastTabClosed()\0"
    "newTab()\0index\0closeTab(int)\0closeTab()\0"
    "closeOtherTabs(int)\0nextTab()\0"
    "previousTab()\0"
};

const QMetaObject FooTabWidget::staticMetaObject = {
    { &QTabWidget::staticMetaObject, qt_meta_stringdata_FooTabWidget,
      qt_meta_data_FooTabWidget, 0 }
};

const QMetaObject *FooTabWidget::metaObject() const
{
    return &staticMetaObject;
}

void *FooTabWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FooTabWidget))
        return static_cast<void*>(const_cast< FooTabWidget*>(this));
    return QTabWidget::qt_metacast(_clname);
}

int FooTabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: tabsChanged(); break;
        case 1: lastTabClosed(); break;
        case 2: newTab(); break;
        case 3: closeTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: closeTab(); break;
        case 5: closeOtherTabs((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: nextTab(); break;
        case 7: previousTab(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void FooTabWidget::tabsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void FooTabWidget::lastTabClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
