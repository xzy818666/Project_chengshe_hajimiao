#include "samsarafutures.h"
#include <cstdlib>

const double SamsaraFutures::LEVERAGE = 2.0;
const double SamsaraFutures::VOLATILITY_MULTIPLIER = 3.0;
const double SamsaraFutures::MAINTENANCE_MARGIN = 0.30;

SamsaraFutures::SamsaraFutures(QObject *parent) : Asset(parent), m_price(100.0)
{
    m_priceHistory.append(m_price);
}

QString SamsaraFutures::name() const
{
    return "轮回孽缘";
}

QString SamsaraFutures::id() const
{
    return "samsara_futures";
}

double SamsaraFutures::price() const
{
    return m_price;
}

Asset::RiskLevel SamsaraFutures::riskLevel() const
{
    return High;
}

void SamsaraFutures::update(double dt, double marketDrift, double marketVolatility)
{
    Q_UNUSED(dt);
    double drift = marketDrift * 0.001 * LEVERAGE;
    double volatility = 0.02 * marketVolatility * VOLATILITY_MULTIPLIER;
    double random = (rand() / (double)RAND_MAX - 0.5) * 2;

    m_price *= (1 + drift + volatility * random);
    m_price = qMax(1.0, m_price);

    m_priceHistory.append(m_price);
    if (m_priceHistory.size() > HISTORY_SIZE) {
        m_priceHistory.removeFirst();
    }

    emit priceChanged(m_price);
}

double SamsaraFutures::leverage() const
{
    return LEVERAGE;
}

double SamsaraFutures::maintenanceMargin() const
{
    return MAINTENANCE_MARGIN;
}
