#include "cultivationlevel.h"
#include <QtMath>

CultivationLevel::CultivationLevel(QObject *parent) : QObject(parent),
    m_level(1), m_exp(0)
{
    m_maxExp = calculateMaxExp(m_level);
}

int CultivationLevel::level() const { return m_level; }
int CultivationLevel::exp() const { return m_exp; }
int CultivationLevel::maxExp() const { return m_maxExp; }

double CultivationLevel::progress() const
{
    if (m_maxExp <= 0) return 0.0;
    return qMin(1.0, static_cast<double>(m_exp) / m_maxExp);
}

int CultivationLevel::calculateMaxExp(int level) const
{
    // 经验曲线：每级所需经验呈指数增长
    // Lv1: 100, Lv2: 150, Lv3: 225, Lv4: 338, Lv5: 507...
    return static_cast<int>(100 * qPow(1.5, level - 1));
}

void CultivationLevel::addExp(int amount)
{
    if (amount <= 0) return;
    m_exp += amount;
    emit expChanged(m_exp, m_maxExp);
    checkLevelUp();
}

void CultivationLevel::checkLevelUp()
{
    while (m_exp >= m_maxExp && m_level < 99) {
        m_exp -= m_maxExp;
        m_level++;
        m_maxExp = calculateMaxExp(m_level);
        emit levelUp(m_level, levelTitle());
        emit expChanged(m_exp, m_maxExp);
    }
}

void CultivationLevel::setLevel(int level)
{
    m_level = qMax(1, qMin(99, level));
    m_maxExp = calculateMaxExp(m_level);
}

void CultivationLevel::setExp(int exp)
{
    m_exp = qMax(0, exp);
}

double CultivationLevel::efficiencyBonus() const
{
    return m_level * 0.02; // 每级 +2%
}

double CultivationLevel::critRateBonus() const
{
    return m_level * 0.01; // 每级 +1%
}

double CultivationLevel::maxLeverageBonus() const
{
    return m_level * 0.1; // 每级 +0.1×
}

double CultivationLevel::autoIncomeBonus() const
{
    return m_level * 0.03; // 每级 +3%
}

QString CultivationLevel::levelTitle() const
{
    // 修仙等级称号
    static const QString titles[] = {
        "凡人", "居士", "沙弥", "行者", "罗汉",
        "菩萨", "佛陀", "摩诃萨", "佛", "无上佛"
    };
    int idx = qMin(9, m_level / 10);
    return titles[idx];
}
