/****************************************************************************
** Meta object code from reading C++ file 'sudokucontroller.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../sudokucontroller.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sudokucontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
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
struct qt_meta_tag_ZN16SudokuControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto SudokuController::qt_create_metaobjectdata<qt_meta_tag_ZN16SudokuControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SudokuController",
        "0$7CD8z7xi17UNxdejiKvnTWANuw4",
        "backToMainHome",
        "",
        "onStartGame",
        "onBackToMainMenu",
        "onBackToHome"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'backToMainHome'
        QtMocHelpers::SignalData<void()>(2, 3, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onStartGame'
        QtMocHelpers::SlotData<void()>(4, 3, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBackToMainMenu'
        QtMocHelpers::SlotData<void()>(5, 3, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBackToHome'
        QtMocHelpers::SlotData<void()>(6, 3, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    int qt_metaObjectHashIndex = 1;
    return QtMocHelpers::metaObjectData<SudokuController, qt_meta_tag_ZN16SudokuControllerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_metaObjectHashIndex);
}
Q_CONSTINIT const QMetaObject SudokuController::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16SudokuControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16SudokuControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN16SudokuControllerE_t>.metaTypes,
    nullptr
} };

void SudokuController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SudokuController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->backToMainHome(); break;
        case 1: _t->onStartGame(); break;
        case 2: _t->onBackToMainMenu(); break;
        case 3: _t->onBackToHome(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (SudokuController::*)()>(_a, &SudokuController::backToMainHome, 0))
            return;
    }
}

const QMetaObject *SudokuController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SudokuController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16SudokuControllerE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int SudokuController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SudokuController::backToMainHome()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
