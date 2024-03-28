/****************************************************************************
** Meta object code from reading C++ file 'note.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../client/note.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'note.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSNoteENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSNoteENDCLASS = QtMocHelpers::stringData(
    "Note",
    "socketClosed",
    "",
    "readyRead",
    "connected",
    "disconnected",
    "errorOccurred",
    "sendLocalOperationToServer",
    "receiveData",
    "onConnected",
    "onDisconnected",
    "onError",
    "socketActivated",
    "socket",
    "on_Note_finished",
    "result",
    "on_Save_clicked",
    "checked"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSNoteENDCLASS_t {
    uint offsetsAndSizes[36];
    char stringdata0[5];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[10];
    char stringdata4[10];
    char stringdata5[13];
    char stringdata6[14];
    char stringdata7[27];
    char stringdata8[12];
    char stringdata9[12];
    char stringdata10[15];
    char stringdata11[8];
    char stringdata12[16];
    char stringdata13[7];
    char stringdata14[17];
    char stringdata15[7];
    char stringdata16[16];
    char stringdata17[8];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSNoteENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSNoteENDCLASS_t qt_meta_stringdata_CLASSNoteENDCLASS = {
    {
        QT_MOC_LITERAL(0, 4),  // "Note"
        QT_MOC_LITERAL(5, 12),  // "socketClosed"
        QT_MOC_LITERAL(18, 0),  // ""
        QT_MOC_LITERAL(19, 9),  // "readyRead"
        QT_MOC_LITERAL(29, 9),  // "connected"
        QT_MOC_LITERAL(39, 12),  // "disconnected"
        QT_MOC_LITERAL(52, 13),  // "errorOccurred"
        QT_MOC_LITERAL(66, 26),  // "sendLocalOperationToServer"
        QT_MOC_LITERAL(93, 11),  // "receiveData"
        QT_MOC_LITERAL(105, 11),  // "onConnected"
        QT_MOC_LITERAL(117, 14),  // "onDisconnected"
        QT_MOC_LITERAL(132, 7),  // "onError"
        QT_MOC_LITERAL(140, 15),  // "socketActivated"
        QT_MOC_LITERAL(156, 6),  // "socket"
        QT_MOC_LITERAL(163, 16),  // "on_Note_finished"
        QT_MOC_LITERAL(180, 6),  // "result"
        QT_MOC_LITERAL(187, 15),  // "on_Save_clicked"
        QT_MOC_LITERAL(203, 7)   // "checked"
    },
    "Note",
    "socketClosed",
    "",
    "readyRead",
    "connected",
    "disconnected",
    "errorOccurred",
    "sendLocalOperationToServer",
    "receiveData",
    "onConnected",
    "onDisconnected",
    "onError",
    "socketActivated",
    "socket",
    "on_Note_finished",
    "result",
    "on_Save_clicked",
    "checked"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSNoteENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   92,    2, 0x06,    1 /* Public */,
       3,    0,   93,    2, 0x06,    2 /* Public */,
       4,    0,   94,    2, 0x06,    3 /* Public */,
       5,    0,   95,    2, 0x06,    4 /* Public */,
       6,    0,   96,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    0,   97,    2, 0x08,    6 /* Private */,
       8,    0,   98,    2, 0x08,    7 /* Private */,
       9,    0,   99,    2, 0x08,    8 /* Private */,
      10,    0,  100,    2, 0x08,    9 /* Private */,
      11,    0,  101,    2, 0x08,   10 /* Private */,
      12,    1,  102,    2, 0x08,   11 /* Private */,
      14,    1,  105,    2, 0x08,   13 /* Private */,
      16,    1,  108,    2, 0x08,   15 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::Bool,   17,

       0        // eod
};

Q_CONSTINIT const QMetaObject Note::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CLASSNoteENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSNoteENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSNoteENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Note, std::true_type>,
        // method 'socketClosed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'readyRead'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'connected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'disconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'errorOccurred'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendLocalOperationToServer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'receiveData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'socketActivated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_Note_finished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_Save_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void Note::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Note *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->socketClosed(); break;
        case 1: _t->readyRead(); break;
        case 2: _t->connected(); break;
        case 3: _t->disconnected(); break;
        case 4: _t->errorOccurred(); break;
        case 5: _t->sendLocalOperationToServer(); break;
        case 6: _t->receiveData(); break;
        case 7: _t->onConnected(); break;
        case 8: _t->onDisconnected(); break;
        case 9: _t->onError(); break;
        case 10: _t->socketActivated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 11: _t->on_Note_finished((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->on_Save_clicked((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Note::*)();
            if (_t _q_method = &Note::socketClosed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Note::*)();
            if (_t _q_method = &Note::readyRead; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Note::*)();
            if (_t _q_method = &Note::connected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Note::*)();
            if (_t _q_method = &Note::disconnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Note::*)();
            if (_t _q_method = &Note::errorOccurred; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject *Note::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Note::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSNoteENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Note::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void Note::socketClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Note::readyRead()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Note::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Note::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Note::errorOccurred()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
