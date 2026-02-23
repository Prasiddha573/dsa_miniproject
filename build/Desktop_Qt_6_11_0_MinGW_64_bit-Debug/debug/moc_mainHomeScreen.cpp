/****************************************************************************
** Meta object code from reading C++ file 'mainHomeScreen.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainHomeScreen.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainHomeScreen.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN14MainHomeScreenE_t {};
} // unnamed namespace

template <> constexpr inline auto MainHomeScreen::qt_create_metaobjectdata<qt_meta_tag_ZN14MainHomeScreenE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainHomeScreen",
        "0$+HM2pXLYtsHOTps22G/i6hmwuwY",
        "navigateToSnake",
        "",
        "navigateToMinesweeper",
        "navigateToSudoku",
        "onGameButtonClicked",
        "id",
        "onExitClicked",
        "titleScale"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'navigateToSnake'
        QtMocHelpers::SignalData<void()>(2, 3, QMC::AccessPublic, QMetaType::Void),
        // Signal 'navigateToMinesweeper'
        QtMocHelpers::SignalData<void()>(4, 3, QMC::AccessPublic, QMetaType::Void),
        // Signal 'navigateToSudoku'
        QtMocHelpers::SignalData<void()>(5, 3, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onGameButtonClicked'
        QtMocHelpers::SlotData<void(int)>(6, 3, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 7 },
        }}),
        // Slot 'onExitClicked'
        QtMocHelpers::SlotData<void()>(8, 3, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'titleScale'
        QtMocHelpers::PropertyData<float>(9, QMetaType::Float, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    int qt_metaObjectHashIndex = 1;
    return QtMocHelpers::metaObjectData<MainHomeScreen, qt_meta_tag_ZN14MainHomeScreenE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_metaObjectHashIndex);
}
Q_CONSTINIT const QMetaObject MainHomeScreen::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14MainHomeScreenE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14MainHomeScreenE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14MainHomeScreenE_t>.metaTypes,
    nullptr
} };

void MainHomeScreen::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainHomeScreen *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->navigateToSnake(); break;
        case 1: _t->navigateToMinesweeper(); break;
        case 2: _t->navigateToSudoku(); break;
        case 3: _t->onGameButtonClicked((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->onExitClicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (MainHomeScreen::*)()>(_a, &MainHomeScreen::navigateToSnake, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (MainHomeScreen::*)()>(_a, &MainHomeScreen::navigateToMinesweeper, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (MainHomeScreen::*)()>(_a, &MainHomeScreen::navigateToSudoku, 2))
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

const QMetaObject *MainHomeScreen::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainHomeScreen::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14MainHomeScreenE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MainHomeScreen::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
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
void MainHomeScreen::navigateToSnake()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MainHomeScreen::navigateToMinesweeper()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MainHomeScreen::navigateToSudoku()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
