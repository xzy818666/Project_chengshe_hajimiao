#include "instrument.h"

Instrument::Instrument(Type type) : m_type(type)
{
}

QString Instrument::name() const
{
    return typeToString(m_type);
}

double Instrument::price() const
{
    switch (m_type) {
    case BasicWoodenFish: return 0;
    case ElectronicIncense: return 500;
    case TurboWoodenFish: return 2000;
    case QuantumPrayerBeads: return 8000;
    case AIChantingMachine: return 15000;
    default: return 0;
    }
}

double Instrument::clickReward() const
{
    switch (m_type) {
    case BasicWoodenFish: return 1;
    case TurboWoodenFish: return 5;
    case QuantumPrayerBeads: return 2;
    default: return 0;
    }
}

double Instrument::autoReward() const
{
    switch (m_type) {
    case ElectronicIncense: return 1;
    case AIChantingMachine: return 10;
    default: return 0;
    }
}

double Instrument::maintenanceCost() const
{
    switch (m_type) {
    case AIChantingMachine: return 1;
    default: return 0;
    }
}

double Instrument::critChance() const
{
    switch (m_type) {
    case QuantumPrayerBeads: return 0.10;
    default: return 0;
    }
}

int Instrument::duration() const
{
    switch (m_type) {
    case ElectronicIncense: return 60;
    default: return 0;
    }
}

Instrument::Type Instrument::type() const
{
    return m_type;
}

bool Instrument::hasCrit() const
{
    return critChance() > 0;
}

bool Instrument::hasAutoReward() const
{
    return autoReward() > 0;
}

bool Instrument::hasMaintenanceCost() const
{
    return maintenanceCost() > 0;
}

QString Instrument::typeToString(Type type)
{
    switch (type) {
    case BasicWoodenFish: return "基础木鱼";
    case ElectronicIncense: return "电子烧香";
    case TurboWoodenFish: return "涡轮增压木鱼";
    case QuantumPrayerBeads: return "量子佛珠";
    case AIChantingMachine: return "AI诵经机";
    default: return "未知法器";
    }
}

QString Instrument::unstrikedImagePath() const
{
    switch (m_type) {
    case BasicWoodenFish: return ":/images/basic_muyu_unstriked.jpg";
    case TurboWoodenFish: return ":/images/turbo_muyu_unstriked.jpg";
    case QuantumPrayerBeads: return ":/images/quantum_beads_unstriked.jpg";
    default: return "";
    }
}

QString Instrument::strikedImagePath() const
{
    switch (m_type) {
    case BasicWoodenFish: return ":/images/basic_muyu_striked.jpg";
    case TurboWoodenFish: return ":/images/turbo_muyu_striked.jpg";
    case QuantumPrayerBeads: return ":/images/quantum_beads_striked.jpg";
    default: return "";
    }
}

bool Instrument::isLotusInstrument() const
{
    return m_type == ElectronicIncense || m_type == AIChantingMachine;
}

bool Instrument::isCloudInstrument() const
{
    return !isLotusInstrument();
}

Instrument::Type Instrument::stringToType(const QString& str)
{
    if (str == "基础木鱼") return BasicWoodenFish;
    if (str == "电子烧香") return ElectronicIncense;
    if (str == "涡轮增压木鱼") return TurboWoodenFish;
    if (str == "量子佛珠") return QuantumPrayerBeads;
    if (str == "AI诵经机") return AIChantingMachine;
    return BasicWoodenFish;
}
