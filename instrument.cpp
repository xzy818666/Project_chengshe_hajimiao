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
    case BasicIncense: return 100;
    case ElectronicIncense: return 500;
    case TurboWoodenFish: return 2000;
    case QuantumPrayerBeads: return 8000;
    case AIChantingMachine: return 15000;
    case Cassock: return 0;
    case Compass: return 0;
    case Fuchen: return 0;
    case Ruyi: return 0;
    case AllSeeingEye: return 5000;
    default: return 0;
    }
}

double Instrument::clickReward() const
{
    switch (m_type) {
    case BasicWoodenFish: return 1;
    case TurboWoodenFish: return 5;
    case QuantumPrayerBeads: return 2;
    case AllSeeingEye: return 3;
    default: return 0;
    }
}

double Instrument::autoReward() const
{
    switch (m_type) {
    case BasicIncense: return 0.5;
    case ElectronicIncense: return 1;
    case AIChantingMachine: return 10;
    case Cassock: return 0;
    case Compass: return 0;
    case Fuchen: return 0;
    case Ruyi: return 0;
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
    case AllSeeingEye: return 0.05;
    default: return 0;
    }
}

int Instrument::duration() const
{
    switch (m_type) {
    case BasicIncense: return 60;
    case ElectronicIncense: return 60;
    case Cassock: return 0;
    case Compass: return 0;
    case Fuchen: return 0;
    case Ruyi: return 0;
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
    case BasicIncense: return "基础烧香";
    case ElectronicIncense: return "电子烧香";
    case TurboWoodenFish: return "涡轮增压木鱼";
    case QuantumPrayerBeads: return "量子佛珠";
    case AIChantingMachine: return "AI诵经机";
    case Cassock: return "袈裟";
    case Compass: return "罗盘";
    case Fuchen: return "拂尘";
    case Ruyi: return "如意";
    case AllSeeingEye: return "全视之眼";
    default: return "未知法器";
    }
}

QString Instrument::unstrikedImagePath() const
{
    switch (m_type) {
    case BasicWoodenFish: return ":/images/basic_muyu_unstriked.png";
    case TurboWoodenFish: return ":/images/turbo_muyu_unstriked.png";
    case QuantumPrayerBeads: return ":/images/quantum_beads_unstriked.png";
    case AllSeeingEye: return ":/images/AllSeeingEye_closed.png";
    default: return "";
    }
}

QString Instrument::strikedImagePath() const
{
    switch (m_type) {
    case BasicWoodenFish: return ":/images/basic_muyu_striked.png";
    case TurboWoodenFish: return ":/images/turbo_muyu_striked.png";
    case QuantumPrayerBeads: return ":/images/quantum_beads_striked.png";
    case AllSeeingEye: return ":/images/AllSeeingEye_open.png";
    default: return "";
    }
}

bool Instrument::isLotusInstrument() const
{
    return m_type == BasicIncense || m_type == ElectronicIncense || m_type == AIChantingMachine
        || m_type == Cassock || m_type == Compass || m_type == Fuchen || m_type == Ruyi;
}

bool Instrument::isCloudInstrument() const
{
    return !isLotusInstrument();
}

Instrument::Type Instrument::stringToType(const QString& str)
{
    if (str == "基础木鱼") return BasicWoodenFish;
    if (str == "基础烧香") return BasicIncense;
    if (str == "电子烧香") return ElectronicIncense;
    if (str == "涡轮增压木鱼") return TurboWoodenFish;
    if (str == "量子佛珠") return QuantumPrayerBeads;
    if (str == "AI诵经机") return AIChantingMachine;
    if (str == "袈裟") return Cassock;
    if (str == "罗盘") return Compass;
    if (str == "拂尘") return Fuchen;
    if (str == "如意") return Ruyi;
    if (str == "全视之眼") return AllSeeingEye;
    return BasicWoodenFish;
}
