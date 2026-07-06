#include "dharmafund.h"
#include <cstdlib>

DharmaFund::DharmaFund(QObject *parent) : Asset(parent), m_price(100.0)
{
    m_priceHistory.append(m_price);
}

QString DharmaFund::name() const
{
    return "因果私缘";
}

QString DharmaFund::id() const
{
    return "dharma_fund";
}

double DharmaFund::price() const
{
    return m_price;
}

Asset::RiskLevel DharmaFund::riskLevel() const
{
    return MediumLow;
}

void DharmaFund::update(double dt, double marketDrift, double marketVolatility)
{
    Q_UNUSED(dt);
    double drift = marketDrift * 0.0005;
    double volatility = 0.01 * marketVolatility;
    double random = (rand() / (double)RAND_MAX - 0.5) * 2;

    m_price *= (1 + drift + volatility * random);
    m_price = qMax(1.0, m_price);

    m_priceHistory.append(m_price);
    if (m_priceHistory.size() > HISTORY_SIZE) {
        m_priceHistory.removeFirst();
    }

    emit priceChanged(m_price);
}
