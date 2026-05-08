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
    const int DAY_TICKS = 100; // 10 seconds = 100 ticks at 100ms interval
    int window = qMin(m_priceHistory.size(), DAY_TICKS);
    if (window < 2) return 0;
    int start = m_priceHistory.size() - window;
    double first = m_priceHistory[start];
    double last = m_priceHistory.last();
    return ((last - first) / first) * 100;
}
