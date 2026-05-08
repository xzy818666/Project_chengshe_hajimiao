#include "meritindex.h"
#include <cstdlib>

MeritIndex::MeritIndex(QObject *parent) : Asset(parent), m_price(100.0)
{
    m_priceHistory.append(m_price);
}

QString MeritIndex::name() const
{
    return "众生法缘";
}

QString MeritIndex::id() const
{
    return "merit_index";
}

double MeritIndex::price() const
{
    return m_price;
}

Asset::RiskLevel MeritIndex::riskLevel() const
{
    return Medium;
}

void MeritIndex::update(double dt, double marketDrift, double marketVolatility)
{
    Q_UNUSED(dt);
    double drift = marketDrift * 0.0003;
    double volatility = 0.008 * marketVolatility;
    double random = (rand() / (double)RAND_MAX - 0.5) * 2;

    m_price *= (1 + drift + volatility * random);
    m_price = qMax(1.0, m_price);

    m_priceHistory.append(m_price);
    if (m_priceHistory.size() > HISTORY_SIZE) {
        m_priceHistory.removeFirst();
    }

    emit priceChanged(m_price);
}
