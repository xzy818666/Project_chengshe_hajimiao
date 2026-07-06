#ifndef MERITINDEX_H
#define MERITINDEX_H

#include "asset.h"

class MeritIndex : public Asset
{
    Q_OBJECT
public:
    explicit MeritIndex(QObject *parent = nullptr);

    QString name() const override;
    QString id() const override;
    double price() const override;
    RiskLevel riskLevel() const override;

    void update(double dt, double marketDrift = 0, double marketVolatility = 1) override;

private:
    double m_price;
};

#endif // MERITINDEX_H
