/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Crossfile/MainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[22];
    char stringdata0[240];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 16), // "clipboardChanged"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 20), // "treeSelectionChanged"
QT_MOC_LITERAL(4, 50, 7), // "current"
QT_MOC_LITERAL(5, 58, 8), // "previous"
QT_MOC_LITERAL(6, 67, 3), // "cut"
QT_MOC_LITERAL(7, 71, 4), // "copy"
QT_MOC_LITERAL(8, 76, 5), // "paste"
QT_MOC_LITERAL(9, 82, 3), // "del"
QT_MOC_LITERAL(10, 86, 9), // "newFolder"
QT_MOC_LITERAL(11, 96, 4), // "view"
QT_MOC_LITERAL(12, 101, 12), // "showAboutBox"
QT_MOC_LITERAL(13, 114, 15), // "showPreferences"
QT_MOC_LITERAL(14, 130, 16), // "focusChangedSlot"
QT_MOC_LITERAL(15, 147, 8), // "QWidget*"
QT_MOC_LITERAL(16, 156, 3), // "now"
QT_MOC_LITERAL(17, 160, 18), // "toggleToDetailView"
QT_MOC_LITERAL(18, 179, 16), // "toggleToIconView"
QT_MOC_LITERAL(19, 196, 12), // "toggleHidden"
QT_MOC_LITERAL(20, 209, 15), // "showContextMenu"
QT_MOC_LITERAL(21, 225, 14) // "showProperties"

    },
    "MainWindow\0clipboardChanged\0\0"
    "treeSelectionChanged\0current\0previous\0"
    "cut\0copy\0paste\0del\0newFolder\0view\0"
    "showAboutBox\0showPreferences\0"
    "focusChangedSlot\0QWidget*\0now\0"
    "toggleToDetailView\0toggleToIconView\0"
    "toggleHidden\0showContextMenu\0"
    "showProperties"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x0a /* Public */,
       3,    2,   95,    2, 0x08 /* Private */,
       6,    0,  100,    2, 0x08 /* Private */,
       7,    0,  101,    2, 0x08 /* Private */,
       8,    0,  102,    2, 0x08 /* Private */,
       9,    0,  103,    2, 0x08 /* Private */,
      10,    0,  104,    2, 0x08 /* Private */,
      11,    0,  105,    2, 0x08 /* Private */,
      12,    0,  106,    2, 0x08 /* Private */,
      13,    0,  107,    2, 0x08 /* Private */,
      14,    2,  108,    2, 0x08 /* Private */,
      17,    0,  113,    2, 0x08 /* Private */,
      18,    0,  114,    2, 0x08 /* Private */,
      19,    0,  115,    2, 0x08 /* Private */,
      20,    1,  116,    2, 0x08 /* Private */,
      21,    0,  119,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex, QMetaType::QModelIndex,    4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15, 0x80000000 | 15,    2,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,    2,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clipboardChanged(); break;
        case 1: _t->treeSelectionChanged((*reinterpret_cast< QModelIndex(*)>(_a[1])),(*reinterpret_cast< QModelIndex(*)>(_a[2]))); break;
        case 2: _t->cut(); break;
        case 3: _t->copy(); break;
        case 4: _t->paste(); break;
        case 5: _t->del(); break;
        case 6: _t->newFolder(); break;
        case 7: _t->view(); break;
        case 8: _t->showAboutBox(); break;
        case 9: _t->showPreferences(); break;
        case 10: _t->focusChangedSlot((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        case 11: _t->toggleToDetailView(); break;
        case 12: _t->toggleToIconView(); break;
        case 13: _t->toggleHidden(); break;
        case 14: _t->showContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 15: _t->showProperties(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
