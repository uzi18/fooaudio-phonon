/****************************************************************************
** Meta object code from reading C++ file 'footabwidget.hpp'
**
** Created: Sat Feb 7 19:45:21 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "footabwidget.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'footabwidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FooTabWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      44,   13,   13,   13, 0x05,
      71,   69,   13,   13, 0x05,
     119,  116,   13,   13, 0x05,
     165,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     190,   13,   13,   13, 0x0a,
     210,   13,   13,   13, 0x0a,
     230,   69,   13,   13, 0x0a,
     247,   13,   13,   13, 0x08,
     272,   13,   13,   13, 0x08,
     296,   69,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FooTabWidget[] = {
    "FooTabWidget\0\0mouseDoubleClickTab(QWidget*)\0"
    "mouseDoubleClickTabBar()\0,\0"
    "tabBarContextMenu(QPoint,QContextMenuEvent*)\0"
    ",,\0tabContextMenu(int,QPoint,QContextMenuEvent*)\0"
    "currentChanged(QWidget*)\0setCurrentPage(int)\0"
    "removeCurrentPage()\0moveTab(int,int)\0"
    "mouseDoubleClickTab(int)\0"
    "tab_currentChanged(int)\0"
    "tab_contextMenu(QContextMenuEvent*,int)\0"
};

const QMetaObject FooTabWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FooTabWidget,
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
    return QWidget::qt_metacast(_clname);
}

int FooTabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: mouseDoubleClickTab((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 1: mouseDoubleClickTabBar(); break;
        case 2: tabBarContextMenu((*reinterpret_cast< QPoint(*)>(_a[1])),(*reinterpret_cast< QContextMenuEvent*(*)>(_a[2]))); break;
        case 3: tabContextMenu((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QPoint(*)>(_a[2])),(*reinterpret_cast< QContextMenuEvent*(*)>(_a[3]))); break;
        case 4: currentChanged((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 5: setCurrentPage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: removeCurrentPage(); break;
        case 7: moveTab((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: mouseDoubleClickTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: tab_currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: tab_contextMenu((*reinterpret_cast< QContextMenuEvent*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        }
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void FooTabWidget::mouseDoubleClickTab(QWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FooTabWidget::mouseDoubleClickTabBar()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void FooTabWidget::tabBarContextMenu(QPoint _t1, QContextMenuEvent * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FooTabWidget::tabContextMenu(int _t1, QPoint _t2, QContextMenuEvent * _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void FooTabWidget::currentChanged(QWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
