/****************************************************************************
** Meta object code from reading C++ file 'footabwidget.hpp'
**
** Created: Mon Mar 23 00:20:25 2009
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
      17,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      28,   13,   13,   13, 0x05,
      44,   13,   13,   13, 0x05,
      69,   13,   13,   13, 0x05,
      99,   13,   13,   13, 0x05,
     120,   13,   13,   13, 0x05,
     138,   13,   13,   13, 0x05,
     177,   13,   13,   13, 0x05,
     218,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     257,   13,   13,   13, 0x0a,
     272,  266,   13,   13, 0x0a,
     286,   13,   13,   13, 0x2a,
     297,   13,   13,   13, 0x0a,
     317,   13,   13,   13, 0x0a,
     327,   13,   13,   13, 0x0a,
     341,   13,   13,   13, 0x08,
     363,  361,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FooTabWidget[] = {
    "FooTabWidget\0\0tabsChanged()\0lastTabClosed()\0"
    "setCurrentTitle(QString)\0"
    "showStatusBarMessage(QString)\0"
    "linkHovered(QString)\0loadProgress(int)\0"
    "menuBarVisibilityChangeRequested(bool)\0"
    "statusBarVisibilityChangeRequested(bool)\0"
    "toolBarVisibilityChangeRequested(bool)\0"
    "newTab()\0index\0closeTab(int)\0closeTab()\0"
    "closeOtherTabs(int)\0nextTab()\0"
    "previousTab()\0currentChanged(int)\0,\0"
    "moveTab(int,int)\0"
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
        case 2: setCurrentTitle((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: showStatusBarMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: linkHovered((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: loadProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: menuBarVisibilityChangeRequested((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: statusBarVisibilityChangeRequested((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: toolBarVisibilityChangeRequested((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: newTab(); break;
        case 10: closeTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: closeTab(); break;
        case 12: closeOtherTabs((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: nextTab(); break;
        case 14: previousTab(); break;
        case 15: currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: moveTab((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 17;
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

// SIGNAL 2
void FooTabWidget::setCurrentTitle(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FooTabWidget::showStatusBarMessage(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void FooTabWidget::linkHovered(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void FooTabWidget::loadProgress(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void FooTabWidget::menuBarVisibilityChangeRequested(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void FooTabWidget::statusBarVisibilityChangeRequested(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void FooTabWidget::toolBarVisibilityChangeRequested(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_END_MOC_NAMESPACE
