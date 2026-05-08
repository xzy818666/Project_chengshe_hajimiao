/****************************************************************************
** Meta object code from reading C++ file 'wallet.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "wallet.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wallet.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN6WalletE_t {};
} // unnamed namespace

template <> constexpr inline auto Wallet::qt_create_metaobjectdata<qt_meta_tag_ZN6WalletE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Wallet",
        "meritChanged",
        "",
        "value",
        "debtChanged",
        "yezhangChanged",
        "assetsChanged",
        "assetBought",
        "assetId",
        "shares",
        "cost",
        "assetSold",
        "revenue",
        "profit",
        "borrowed",
        "amount",
        "repaid"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'meritChanged'
        QtMocHelpers::SignalData<void(double)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 3 },
        }}),
        // Signal 'debtChanged'
        QtMocHelpers::SignalData<void(double)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 3 },
        }}),
        // Signal 'yezhangChanged'
        QtMocHelpers::SignalData<void(double)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 3 },
        }}),
        // Signal 'assetsChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'assetBought'
        QtMocHelpers::SignalData<void(const QString &, double, double)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 }, { QMetaType::Double, 9 }, { QMetaType::Double, 10 },
        }}),
        // Signal 'assetSold'
        QtMocHelpers::SignalData<void(const QString &, double, double, double)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 }, { QMetaType::Double, 9 }, { QMetaType::Double, 12 }, { QMetaType::Double, 13 },
        }}),
        // Signal 'borrowed'
        QtMocHelpers::SignalData<void(double)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 15 },
        }}),
        // Signal 'repaid'
        QtMocHelpers::SignalData<void(double)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 15 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Wallet, qt_meta_tag_ZN6WalletE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Wallet::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6WalletE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6WalletE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6WalletE_t>.metaTypes,
    nullptr
} };

void Wallet::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Wallet *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->meritChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 1: _t->debtChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 2: _t->yezhangChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 3: _t->assetsChanged(); break;
        case 4: _t->assetBought((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3]))); break;
        case 5: _t->assetSold((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4]))); break;
        case 6: _t->borrowed((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 7: _t->repaid((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Wallet::*)(double )>(_a, &Wallet::meritChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Wallet::*)(double )>(_a, &Wallet::debtChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Wallet::*)(double )>(_a, &Wallet::yezhangChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Wallet::*)()>(_a, &Wallet::assetsChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Wallet::*)(const QString & , double , double )>(_a, &Wallet::assetBought, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Wallet::*)(const QString & , double , double , double )>(_a, &Wallet::assetSold, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Wallet::*)(double )>(_a, &Wallet::borrowed, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (Wallet::*)(double )>(_a, &Wallet::repaid, 7))
            return;
    }
}

const QMetaObject *Wallet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Wallet::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6WalletE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Wallet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void Wallet::meritChanged(double _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void Wallet::debtChanged(double _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void Wallet::yezhangChanged(double _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void Wallet::assetsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Wallet::assetBought(const QString & _t1, double _t2, double _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1, _t2, _t3);
}

// SIGNAL 5
void Wallet::assetSold(const QString & _t1, double _t2, double _t3, double _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 6
void Wallet::borrowed(double _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void Wallet::repaid(double _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}
QT_WARNING_POP
