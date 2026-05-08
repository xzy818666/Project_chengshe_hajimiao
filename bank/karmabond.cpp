#include "karmabond.h"

const double KarmaBond::INTEREST_RATE = 0.04;

KarmaBond::KarmaBond(QObject *parent) : Asset(parent)
{
    m_priceHistory.append(1.0);
}

QString KarmaBond::name() const
{
    return "善行福缘";
}

QString KarmaBond::id() const
{
    return "karma_bond";
}

double KarmaBond::price() const
{
    return 1.0;
}

Asset::RiskLevel KarmaBond::riskLevel() const
{
    return Low;
}

void KarmaBond::update(double dt, double marketDrift, double marketVolatility)
{
    Q_UNUSED(dt);
    Q_UNUSED(marketDrift);
    Q_UNUSED(marketVolatility);
    m_priceHistory.append(1.0);
    if (m_priceHistory.size() > HISTORY_SIZE) {
        m_priceHistory.removeFirst();
    }
}
