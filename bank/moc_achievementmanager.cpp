/****************************************************************************
** Meta object code from reading C++ file 'achievementmanager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "achievementmanager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'achievementmanager.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN18AchievementManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto AchievementManager::qt_create_metaobjectdata<qt_meta_tag_ZN18AchievementManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "AchievementManager",
        "achievementUnlocked",
        "",
        "title",
        "description",
        "onMeritChanged",
        "merit",
        "onNetWorthChanged",
        "netWorth",
        "onAssetBought",
        "assetId",
        "shares",
        "cost",
        "onAssetSold",
        "revenue",
        "profit",
        "onBorrowed",
        "amount",
        "onYezhangChanged",
        "yezhang",
        "onClickReport",
        "sessionClicks",
        "checkHoldings",
        "QMap<QString,double>",
        "assets",
        "checkPortfolioLoss",
        "prices"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'achievementUnlocked'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 4 },
        }}),
        // Slot 'onMeritChanged'
        QtMocHelpers::SlotData<void(double)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 6 },
        }}),
        // Slot 'onNetWorthChanged'
        QtMocHelpers::SlotData<void(double)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 8 },
        }}),
        // Slot 'onAssetBought'
        QtMocHelpers::SlotData<void(const QString &, double, double)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 }, { QMetaType::Double, 11 }, { QMetaType::Double, 12 },
        }}),
        // Slot 'onAssetSold'
        QtMocHelpers::SlotData<void(const QString &, double, double, double)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 }, { QMetaType::Double, 11 }, { QMetaType::Double, 14 }, { QMetaType::Double, 15 },
        }}),
        // Slot 'onBorrowed'
        QtMocHelpers::SlotData<void(double)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 17 },
        }}),
        // Slot 'onYezhangChanged'
        QtMocHelpers::SlotData<void(double)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 19 },
        }}),
        // Slot 'onClickReport'
        QtMocHelpers::SlotData<void(int)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 21 },
        }}),
        // Slot 'checkHoldings'
        QtMocHelpers::SlotData<void(const QMap<QString,double> &)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 23, 24 },
        }}),
        // Slot 'checkPortfolioLoss'
        QtMocHelpers::SlotData<void(const QMap<QString,double> &, const QMap<QString,double> &)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 23, 24 }, { 0x80000000 | 23, 26 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<AchievementManager, qt_meta_tag_ZN18AchievementManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject AchievementManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18AchievementManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18AchievementManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN18AchievementManagerE_t>.metaTypes,
    nullptr
} };

void AchievementManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AchievementManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->achievementUnlocked((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->onMeritChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 2: _t->onNetWorthChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 3: _t->onAssetBought((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3]))); break;
        case 4: _t->onAssetSold((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4]))); break;
        case 5: _t->onBorrowed((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 6: _t->onYezhangChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 7: _t->onClickReport((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->checkHoldings((*reinterpret_cast<std::add_pointer_t<QMap<QString,double>>>(_a[1]))); break;
        case 9: _t->checkPortfolioLoss((*reinterpret_cast<std::add_pointer_t<QMap<QString,double>>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QMap<QString,double>>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (AchievementManager::*)(const QString & , const QString & )>(_a, &AchievementManager::achievementUnlocked, 0))
            return;
    }
}

const QMetaObject *AchievementManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AchievementManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18AchievementManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int AchievementManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void AchievementManager::achievementUnlocked(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}
QT_WARNING_POP
