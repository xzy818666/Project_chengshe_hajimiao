#include "portfolioadvisor.h"
#include "asset.h"
#include <QtMath>
#include <algorithm>

PortfolioAdvisor::PortfolioAdvisor(QObject *parent)
    : QObject(parent)
{
}

QList<PortfolioAdvisor::Advice> PortfolioAdvisor::advise(const QList<Asset*>& assets) const
{
    QList<Advice> result;
    QList<double> sharpes;
    double totalPositiveSharpe = 0.0;

    for (Asset* asset : assets) {
        const QVector<double>& history = asset->priceHistory();
        Advice a;
        a.assetId = asset->id();
        a.assetName = asset->name();
        a.expectedReturn = 0.0;
        a.risk = 0.0;
        a.weight = 0.0;

        if (history.size() >= 10) {
            double sharpe = calculateSharpe(history);
            sharpes.append(sharpe);
            a.expectedReturn = sharpe;
            a.risk = 0.0; // simplified
            if (sharpe > 0) {
                totalPositiveSharpe += sharpe;
            }
        } else {
            sharpes.append(0.0);
        }
        result.append(a);
    }

    for (int i = 0; i < result.size(); ++i) {
        if (totalPositiveSharpe > 0 && sharpes[i] > 0) {
            result[i].weight = sharpes[i] / totalPositiveSharpe;
            result[i].reason = QString("夏普比率 %1").arg(sharpes[i], 0, 'f', 2);
        } else if (assets[i]->riskLevel() == Asset::Low) {
            // 保底推荐债券
            result[i].weight = 0.1;
            result[i].reason = "无风险底仓";
        } else {
            result[i].weight = 0.0;
            result[i].reason = "近期表现不佳";
        }
    }

    return result;
}

double PortfolioAdvisor::calculateSharpe(const QVector<double>& history) const
{
    if (history.size() < 2) return 0.0;

    int window = qMin(history.size(), 50);
    int start = history.size() - window;

    double sumReturn = 0.0;
    double sumSq = 0.0;
    int count = 0;

    for (int i = start + 1; i < history.size(); ++i) {
        double prev = history[i - 1];
        double curr = history[i];
        if (prev <= 0) continue;
        double r = (curr - prev) / prev;
        sumReturn += r;
        sumSq += r * r;
        ++count;
    }

    if (count == 0) return 0.0;

    double mean = sumReturn / count;
    double variance = (sumSq / count) - (mean * mean);
    double stddev = qSqrt(qMax(0.0, variance));

    const double riskFreeRate = 0.04 / 86400.0; // per tick approx
    if (stddev < 1e-8) return mean > riskFreeRate ? 999.0 : 0.0;

    return (mean - riskFreeRate) / stddev;
}
