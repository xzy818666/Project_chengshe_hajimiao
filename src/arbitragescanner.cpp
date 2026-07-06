#include "arbitragescanner.h"
#include "asset.h"
#include <QtMath>

ArbitrageScanner::ArbitrageScanner(QObject *parent)
    : QObject(parent)
{
}

Asset* ArbitrageScanner::findAsset(const QList<Asset*>& assets, const QString& id) const
{
    for (Asset* a : assets) {
        if (a->id() == id) return a;
    }
    return nullptr;
}

QList<ArbitrageScanner::Opportunity> ArbitrageScanner::scan(const QList<Asset*>& assets) const
{
    QList<Opportunity> result;

    Asset* dharma = findAsset(assets, "dharma_fund");
    Asset* merit = findAsset(assets, "merit_index");
    Asset* bond = findAsset(assets, "karma_bond");

    if (dharma && merit && bond) {
        double theoretical = 0.6 * merit->price() + 0.4 * bond->price();
        double actual = dharma->price();
        if (theoretical > 0) {
            double deviation = qAbs(actual - theoretical) / theoretical;
            if (deviation > 0.02) {
                Opportunity op;
                op.assetId = dharma->id();
                op.deviation = deviation;
                if (actual > theoretical) {
                    op.description = QString("因果私缘高估 %1%，建议卖出").arg(deviation * 100, 0, 'f', 1);
                } else {
                    op.description = QString("因果私缘低估 %1%，建议买入").arg(deviation * 100, 0, 'f', 1);
                }
                result.append(op);
            }
        }
    }

    return result;
}
