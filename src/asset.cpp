#include "asset.h"

const int Asset::HISTORY_SIZE = 200;

Asset::Asset(QObject *parent) : QObject(parent)
{
}

double Asset::calculateReturn(double shares, double dt) const
{
    Q_UNUSED(shares);
    Q_UNUSED(dt);
    return 0;
}

const QVector<double>& Asset::priceHistory() const
{
    return m_priceHistory;
}

double Asset::change24h() const
{
    if (m_priceHistory.size() < 2) return 0;
    double first = m_priceHistory.first();
    double last = m_priceHistory.last();
    return ((last - first) / first) * 100;
}
