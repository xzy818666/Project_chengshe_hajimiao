#include "instrumentresonance.h"

InstrumentResonance::InstrumentResonance(QObject *parent) : QObject(parent)
{
    initializeResonanceTable();
}

void InstrumentResonance::initializeResonanceTable()
{
    // 组合1：烈火焚心 — 涡轮增压木鱼 + 电子烧香
    m_resonanceTable.append({
        "烈火焚心",
        "涡轮增压木鱼在电子烧香加持下火力全开，点击收益 ×1.5，自动收益 +2/秒",
        "🔥",
        Instrument::TurboWoodenFish,
        {Instrument::ElectronicIncense},
        false,
        1.5, 2.0, 0.0, 0.0, 0.0,
        0
    });

    // 组合2：珠联璧合 — 量子佛珠 + 如意
    m_resonanceTable.append({
        "珠联璧合",
        "量子佛珠与如意产生量子纠缠，暴击率额外 +5%，暴击倍数 ×1.5",
        "✨",
        Instrument::QuantumPrayerBeads,
        {Instrument::Ruyi},
        false,
        1.0, 0.0, 0.05, 1.5, 0.0,
        0
    });

    // 组合3：洞察先机 — 全视之眼 + 罗盘
    m_resonanceTable.append({
        "洞察先机",
        "全视之眼配合罗盘，可洞察市场短期趋势，效率 +15%",
        "👁",
        Instrument::AllSeeingEye,
        {Instrument::Compass},
        false,
        1.0, 0.0, 0.0, 0.0, 0.15,
        0
    });

    // 组合4：初心 — 基础木鱼 + 基础烧香
    m_resonanceTable.append({
        "初心",
        "回归初心，返璞归真。基础木鱼与基础烧香共鸣，点击收益 ×2.0",
        "🌱",
        Instrument::BasicWoodenFish,
        {Instrument::BasicIncense},
        false,
        2.0, 0.0, 0.0, 0.0, 0.0,
        0
    });

    // 组合5：佛光普照 — AI诵经机 + 袈裟
    m_resonanceTable.append({
        "佛光普照",
        "AI诵经机诵读佛经，袈裟加持，自动收益 +5/秒，效率 +20%",
        "🙏",
        Instrument::AIChantingMachine,
        {Instrument::Cassock},
        false,
        1.0, 5.0, 0.0, 0.0, 0.20,
        0
    });

    // 组合6：三清合一 — 拂尘 + 如意 + 罗盘
    m_resonanceTable.append({
        "三清合一",
        "三件道家法器同时装备，触发道家共鸣。点击收益 +2/击，暴击率 +8%",
        "☯",
        Instrument::None,  // 不限制主动法器
        {Instrument::Fuchen, Instrument::Ruyi, Instrument::Compass},
        true,  // 需要同时装备三件
        1.0, 2.0, 0.08, 0.0, 0.0,
        0
    });

    // 组合7：万法归宗 — 全视之眼 + 量子佛珠 + AI诵经机（终极组合）
    m_resonanceTable.append({
        "万法归宗",
        "三大顶级法器共鸣，万法归一。点击收益 ×3.0，暴击率 +15%，暴击倍数 ×2.0",
        "🌀",
        Instrument::AllSeeingEye,
        {Instrument::QuantumPrayerBeads, Instrument::AIChantingMachine},
        true,
        3.0, 0.0, 0.15, 2.0, 0.0,
        30  // 持续30秒（终极效果有冷却）
    });
}

void InstrumentResonance::updateResonance(const Instrument& activeInstrument,
                                           const QList<Instrument>& auxiliaryInstruments)
{
    QList<ResonanceInfo> newActive;

    for (const auto& resonance : m_resonanceTable) {
        if (checkResonanceTriggered(resonance, activeInstrument, auxiliaryInstruments)) {
            newActive.append(resonance);
        }
    }

    // 检测新激活的共鸣
    for (const auto& resonance : newActive) {
        bool wasActive = false;
        for (const auto& old : m_activeResonances) {
            if (old.name == resonance.name) {
                wasActive = true;
                break;
            }
        }
        if (!wasActive) {
            emit resonanceActivated(resonance.name, resonance.description);
        }
    }

    // 检测失效的共鸣
    for (const auto& old : m_activeResonances) {
        bool stillActive = false;
        for (const auto& resonance : newActive) {
            if (old.name == resonance.name) {
                stillActive = true;
                break;
            }
        }
        if (!stillActive) {
            emit resonanceDeactivated(old.name);
        }
    }

    m_activeResonances = newActive;
}

bool InstrumentResonance::checkResonanceTriggered(const ResonanceInfo& resonance,
                                                   const Instrument& active,
                                                   const QList<Instrument>& auxiliaries) const
{
    // 检查主动法器条件
    if (resonance.requiredActive != Instrument::None &&
        resonance.requiredActive != active.type()) {
        return false;
    }

    // 检查辅助法器条件
    if (resonance.requireAllAuxiliary) {
        // 需要同时装备所有指定的辅助法器
        for (Instrument::Type required : resonance.requiredAuxiliary) {
            bool found = false;
            for (const auto& aux : auxiliaries) {
                if (aux.type() == required) {
                    found = true;
                    break;
                }
            }
            if (!found) return false;
        }
    } else {
        // 至少装备一个指定的辅助法器
        bool found = false;
        for (Instrument::Type required : resonance.requiredAuxiliary) {
            for (const auto& aux : auxiliaries) {
                if (aux.type() == required) {
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        if (!found) return false;
    }

    return true;
}

QList<InstrumentResonance::ResonanceInfo> InstrumentResonance::activeResonances() const
{
    return m_activeResonances;
}

double InstrumentResonance::totalClickMultiplier() const
{
    double total = 1.0;
    for (const auto& r : m_activeResonances) {
        total *= r.clickMultiplier;
    }
    return total;
}

double InstrumentResonance::totalAutoIncomeBonus() const
{
    double total = 0.0;
    for (const auto& r : m_activeResonances) {
        total += r.autoIncomeBonus;
    }
    return total;
}

double InstrumentResonance::totalCritRateBonus() const
{
    double total = 0.0;
    for (const auto& r : m_activeResonances) {
        total += r.critRateBonus;
    }
    return total;
}

double InstrumentResonance::totalCritMultiplierBonus() const
{
    double total = 0.0;
    for (const auto& r : m_activeResonances) {
        total += r.critMultiplierBonus;
    }
    return total;
}

double InstrumentResonance::totalEfficiencyBonus() const
{
    double total = 0.0;
    for (const auto& r : m_activeResonances) {
        total += r.efficiencyBonus;
    }
    return total;
}

QString InstrumentResonance::getActiveResonanceText() const
{
    if (m_activeResonances.isEmpty()) return "无共鸣";

    QStringList parts;
    for (const auto& r : m_activeResonances) {
        parts.append(r.icon + " " + r.name);
    }
    return parts.join("  ");
}
