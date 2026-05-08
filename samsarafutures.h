#ifndef SAMSARAFUTURES_H
#define SAMSARAFUTURES_H

#include "asset.h"

class SamsaraFutures : public Asset
{
    Q_OBJECT
public:
    explicit SamsaraFutures(QObject *parent = nullptr);

    QString name() const override;
    QString id() const override;
    double price() const override;
    RiskLevel riskLevel() const override;

    void update(double dt, double marketDrift = 0, double marketVolatility = 1) override;

    double leverage() const;
    double maintenanceMargin() const;

private:
    double m_price;
    static const double LEVERAGE;
    static const double VOLATILITY_MULTIPLIER;
    static const double MAINTENANCE_MARGIN;
};

#endif // SAMSARAFUTURES_H
