/****************************************************************************
** Meta object code from reading C++ file 'home.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../home.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'home.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN4HomeE_t {};
} // unnamed namespace

template <> constexpr inline auto Home::qt_create_metaobjectdata<qt_meta_tag_ZN4HomeE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Home",
        "0$D6PMQnpC3i+zM/x8GDhSUM+uIQY",
        "startSnakeGame",
        "",
        "difficulty",
        "backToMainHome",
        "onStartClicked",
        "onExitClicked",
        "onDifficultyChanged",
        "id",
        "updateHighScoresDisplay",
        "titleScale"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'startSnakeGame'
        QtMocHelpers::SignalData<void(int)>(2, 3, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 4 },
        }}),
        // Signal 'backToMainHome'
        QtMocHelpers::SignalData<void()>(5, 3, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onStartClicked'
        QtMocHelpers::SlotData<void()>(6, 3, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onExitClicked'
        QtMocHelpers::SlotData<void()>(7, 3, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDifficultyChanged'
        QtMocHelpers::SlotData<void(int)>(8, 3, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'updateHighScoresDisplay'
        QtMocHelpers::SlotData<void()>(10, 3, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'titleScale'
        QtMocHelpers::PropertyData<float>(11, QMetaType::Float, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    int qt_metaObjectHashIndex = 1;
    return QtMocHelpers::metaObjectData<Home, qt_meta_tag_ZN4HomeE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_metaObjectHashIndex);
}
Q_CONSTINIT const QMetaObject Home::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4HomeE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4HomeE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4HomeE_t>.metaTypes,
    nullptr
} };

void Home::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Home *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->startSnakeGame((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->backToMainHome(); break;
        case 2: _t->onStartClicked(); break;
        case 3: _t->onExitClicked(); break;
        case 4: _t->onDifficultyChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->updateHighScoresDisplay(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Home::*)(int )>(_a, &Home::startSnakeGame, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Home::*)()>(_a, &Home::backToMainHome, 1))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<float*>(_v) = _t->titleScale(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setTitleScale(*reinterpret_cast<float*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *Home::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Home::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4HomeE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Home::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void Home::startSnakeGame(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void Home::backToMainHome()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
